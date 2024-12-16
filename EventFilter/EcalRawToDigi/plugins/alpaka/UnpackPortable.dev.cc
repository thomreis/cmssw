#include <alpaka/alpaka.hpp>

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalConstants.h"
#include "EventFilter/EcalRawToDigi/interface/ElectronicsIdGPU.h"
#include "EventFilter/EcalRawToDigi/interface/DCCRawDataDefinitions.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "HeterogeneousCore/AlpakaInterface/interface/workdivision.h"

#include "UnpackPortable.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::raw {

  using namespace ::ecal::raw;
  using namespace cms::alpakatools;

  ////////////////////////////////////////////////////////////////////////////
  // digis unpack kernel
  ////////////////////////////////////////////////////////////////////////////
  class Kernel_unpack {
  public:
    template <typename TAcc, typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
    ALPAKA_FN_ACC void operator()(TAcc const& acc,
                                  unsigned char const* __restrict__ data,
                                  uint32_t const* __restrict__ offsets,
                                  int const* __restrict__ feds,
                                  EcalDigiDeviceCollection::View digisEB,
                                  EcalDigiDeviceCollection::View digisEE,
                                  EcalIdDeviceCollection::View integrityGainErrorsEB,
                                  EcalIdDeviceCollection::View integrityGainErrorsEE,
                                  EcalIdDeviceCollection::View integrityGainSwitchErrorsEB,
                                  EcalIdDeviceCollection::View integrityGainSwitchErrorsEE,
                                  EcalIdDeviceCollection::View integrityChIdErrorsEB,
                                  EcalIdDeviceCollection::View integrityChIdErrorsEE,
                                  EcalIdDeviceCollection::View integrityTTIdErrors,
                                  EcalIdDeviceCollection::View integrityZSXtalIdErrors,
                                  EcalIdDeviceCollection::View integrityBlockSizeErrors,
                                  EcalPnDiodeDigiDeviceCollection::View pnDiodeDigis,
                                  EcalElectronicsMappingDevice::ConstView eid2did,
                                  uint32_t const nbytesTotal) const {
      constexpr auto kSampleSize = ecalPh1::sampleSize;

      // indices
      auto const ifed = alpaka::getIdx<alpaka::Grid, alpaka::Blocks>(acc)[0u];
      auto const threadIdx = alpaka::getIdx<alpaka::Block, alpaka::Threads>(acc)[0u];

      // offset in bytes
      auto const offset = offsets[ifed];
      // fed id
      auto const fed = feds[ifed];
      auto const dcc = fed2dcc(fed);
      auto const isBarrel = is_barrel(dcc);
      // size
      auto const gridDim = alpaka::getWorkDiv<alpaka::Grid, alpaka::Blocks>(acc)[0u];
      auto const size = ifed == gridDim - 1 ? nbytesTotal - offset : offsets[ifed + 1] - offset;
      auto* samples = isBarrel ? digisEB.data()->data() : digisEE.data()->data();
      auto* ids = isBarrel ? digisEB.id() : digisEE.id();
      auto* pChannelsCounter = isBarrel ? &digisEB.size() : &digisEE.size();

      // offset to the right raw buffer
      uint64_t const* buffer = reinterpret_cast<uint64_t const*>(data + offset);

      // dump first 3 bits for each 64-bit word
      //print_first3bits(buffer, size / 8);

      //
      // fed header
      //
      auto const fed_header = buffer[0];
      uint32_t bx = (fed_header >> H_BX_B) & H_BX_MASK;
      uint32_t lv1 = (fed_header >> H_L1_B) & H_L1_MASK;
      uint32_t triggerType = (fed_header >> H_TTYPE_B) & H_TTYPE_MASK;

      // determine the number of FE channels from the trigger type
      uint32_t numbChannels(0);
      if (triggerType == PHYSICTRIGGER) {
        numbChannels = NUMB_FE;
      } else if (triggerType == CALIBRATIONTRIGGER) {
        numbChannels = NUMB_FE + 2;  // FE + 2 MEM blocks
      } else {
        // unsupported trigger type
        return;
      }

      // 9 for fed + dcc header
      // 36 for 4 EE TCC blocks or 18 for 1 EB TCC block
      // 6 for SR block size

      // dcc header w2
      auto const w2 = buffer[2];
      uint8_t const fov = (w2 >> H_FOV_B) & H_FOV_MASK;

      // make a list of channels with data from DCC header channels status
      // this could be done for each block instead of each thread since it defined per FED
      uint8_t exp_ttids[NUMB_FE + 2];  // FE + 2 MEM blocks
      uint8_t ch = 1;
      uint8_t nCh = 0;
      for (uint8_t i = 4; i < HEADERLENGTH; ++i) {  // data words with channel status info
        for (uint8_t j = 0; j < 14; ++j, ++ch) {    // channel status fields in one data word
          const uint8_t shift = j * 4;              //each channel has 4 bits
          const int chStatus = (buffer[i] >> shift) & H_CHSTATUS_MASK;
          const bool regular = (chStatus == CH_DISABLED || chStatus == CH_SUPPRESS);
          const bool problematic =
              (chStatus == CH_TIMEOUT || chStatus == CH_HEADERERR || chStatus == CH_LINKERR ||
               chStatus == CH_LENGTHERR || chStatus == CH_IFIFOFULL || chStatus == CH_L1AIFIFOFULL);
          if (!(regular || problematic)) {
            exp_ttids[nCh] = ch;
            ++nCh;
          }
        }
      }

      //
      // print block headers
      //
      uint8_t const ntccblockwords = isBarrel ? 18 : 36;

      // tower block
      auto const* tower_blocks_start = buffer + HEADERLENGTH + ntccblockwords + SRP_BLOCKLENGTH;
      auto const* trailer = buffer + (size / 8 - 1);
      auto const* current_tower_block = tower_blocks_start;
      uint8_t iCh = 0;
      uint8_t next_tower_id = exp_ttids[iCh];
      while (current_tower_block < trailer && iCh < numbChannels) {
        auto const w = *current_tower_block;
        uint8_t ttid = w & TOWER_ID_MASK;
        uint16_t bxlocal = (w >> TOWER_BX_B) & TOWER_BX_MASK;
        uint16_t lv1local = (w >> TOWER_L1_B) & TOWER_L1_MASK;
        uint16_t block_length = (w >> TOWER_LENGTH_B) & TOWER_LENGTH_MASK;

        // fast forward to the next good tower id (in case of recovery from an earlier header corruption)
        while (exp_ttids[iCh] < next_tower_id) {
          ++iCh;
        }
        ++iCh;

        // check if the tower id in the tower header is the one expected
        // if not try to find the next good header, point the current_tower_block to it, and extract its tower id
        // or break if there is none
        if (ttid != next_tower_id) {
          next_tower_id = find_next_tower_block(current_tower_block, trailer, bx, lv1);
          if (next_tower_id < TOWER_ID_MASK) {
            continue;
          } else {
            break;
          }
        }

        // prepare for the next iteration
        next_tower_id = exp_ttids[iCh];

        uint16_t const dccbx = bx & 0xfff;
        uint16_t const dccl1 = lv1 & 0xfff;
        // fov>=1 is required to support simulated data for which bx==bxlocal==0
        if (fov >= 1 && !is_synced_towerblock(dccbx, bxlocal, dccl1, lv1local)) {
          current_tower_block += block_length;
          continue;
        }

        // go through all the channels
        // get the next channel coordinates
        uint32_t const nchannels = (block_length - 1) / 3;

        bool bad_block = false;
        auto& ch_with_bad_block = alpaka::declareSharedVar<uint32_t, __COUNTER__>(acc);
        if (once_per_block(acc)) {
          ch_with_bad_block = std::numeric_limits<uint32_t>::max();
        }
        // make sure the shared memory is initialised for all threads
        alpaka::syncBlockThreads(acc);

        auto const threadsPerBlock = alpaka::getWorkDiv<alpaka::Block, alpaka::Threads>(acc)[0u];
        // 1 threads per channel in this block
        // All threads enter the loop regardless if they will treat channel indices channel >= nchannels.
        // The threads with excess indices perform no operations but also reach the syncBlockThreads() inside the loop.
        for (uint32_t i = 0; i < nchannels; i += threadsPerBlock) {
          auto const channel = i + threadIdx;

          uint64_t wdata;
          uint8_t stripid;
          uint8_t xtalid;

          // threads must be inside the range (no break here because of syncBlockThreads() afterwards)
          if (channel < nchannels && channel < ch_with_bad_block) {
            // inc the channel's counter and get the pos where to store
            wdata = current_tower_block[1 + channel * 3];
            stripid = wdata & 0x7;
            xtalid = (wdata >> 4) & 0x7;

            // check if the stripid and xtalid are in the allowed range and if not skip the rest of the block
            if (stripid < ElectronicsIdGPU::MIN_STRIPID || stripid > ElectronicsIdGPU::MAX_STRIPID ||
                xtalid < ElectronicsIdGPU::MIN_XTALID || xtalid > ElectronicsIdGPU::MAX_XTALID) {
              bad_block = true;
            }
            if (channel > 0) {
              // check if the stripid has increased or that the xtalid has increased from the previous data word. If not something is wrong and the rest of the block is skipped.
              auto const prev_channel = channel - 1;
              auto const prevwdata = current_tower_block[1 + prev_channel * 3];
              uint8_t const laststripid = prevwdata & 0x7;
              uint8_t const lastxtalid = (prevwdata >> 4) & 0x7;
              if ((stripid == laststripid && xtalid <= lastxtalid) || (stripid < laststripid)) {
                bad_block = true;
              }
            }
          }

          // check if this thread has the lowest bad block
          if (bad_block && channel < ch_with_bad_block) {
            alpaka::atomicMin(acc, &ch_with_bad_block, channel, alpaka::hierarchy::Threads{});
          }

          // make sure that all threads that have to have set the ch_with_bad_block shared memory
          alpaka::syncBlockThreads(acc);

          // threads outside of the range or bad block detected in this thread or one working on a lower block -> stop this loop iteration here
          if (channel >= nchannels || channel >= ch_with_bad_block) {
            continue;
          }

          ElectronicsIdGPU eid{dcc, ttid, stripid, xtalid};
          auto const didraw = isBarrel ? compute_ebdetid(eid) : eid2did[eid.linearIndex()].rawid();
          // skip channels with an invalid detid
          if (didraw == 0)
            continue;

          // get samples
          uint16_t sampleValues[kSampleSize];
          sampleValues[0] = (wdata >> 16) & 0x3fff;
          sampleValues[1] = (wdata >> 32) & 0x3fff;
          sampleValues[2] = (wdata >> 48) & 0x3fff;
          auto const wdata1 = current_tower_block[2 + channel * 3];
          sampleValues[3] = wdata1 & 0x3fff;
          sampleValues[4] = (wdata1 >> 16) & 0x3fff;
          sampleValues[5] = (wdata1 >> 32) & 0x3fff;
          sampleValues[6] = (wdata1 >> 48) & 0x3fff;
          auto const wdata2 = current_tower_block[3 + channel * 3];
          sampleValues[7] = wdata2 & 0x3fff;
          sampleValues[8] = (wdata2 >> 16) & 0x3fff;
          sampleValues[9] = (wdata2 >> 32) & 0x3fff;

          // check gain
          bool isSaturation = true;
          short firstGainZeroSampID{-1}, firstGainZeroSampADC{-1};
          for (uint32_t si = 0; si < kSampleSize; ++si) {
            if (gainId(sampleValues[si]) == 0) {
              firstGainZeroSampID = si;
              firstGainZeroSampADC = adc(sampleValues[si]);
              break;
            }
          }
          if (firstGainZeroSampID != -1) {
            unsigned int plateauEnd = std::min(kSampleSize, (unsigned int)(firstGainZeroSampID + 5));
            for (unsigned int s = firstGainZeroSampID; s < plateauEnd; s++) {
              if (!(gainId(sampleValues[s]) == 0 && adc(sampleValues[s]) == firstGainZeroSampADC)) {
                isSaturation = false;
                break;
              }  //it's not saturation
            }
            // get rid of channels which are stuck in gain0
            if (firstGainZeroSampID < 3) {
              isSaturation = false;
            }
            if (!isSaturation)
              continue;
          } else {  // there is no zero gainId sample
            // gain switch check
            short numGain = 1;
            bool gainSwitchError = false;
            for (unsigned int si = 1; si < kSampleSize; ++si) {
              if ((gainId(sampleValues[si - 1]) > gainId(sampleValues[si])) && numGain < 5)
                gainSwitchError = true;
              if (gainId(sampleValues[si - 1]) == gainId(sampleValues[si]))
                numGain++;
              else
                numGain = 1;
            }
            if (gainSwitchError)
              continue;
          }

          auto const pos = alpaka::atomicAdd(acc, pChannelsCounter, 1u, alpaka::hierarchy::Threads{});

          // store to global
          ids[pos] = didraw;
          std::memcpy(&samples[pos * kSampleSize], sampleValues, kSampleSize * sizeof(uint16_t));
        }

        current_tower_block += block_length;
      }
    }

  private:
    ALPAKA_FN_INLINE ALPAKA_FN_ACC void print_raw_buffer(uint8_t const* const buffer,
                                                         uint32_t const nbytes,
                                                         uint32_t const nbytes_per_row = 20) const {
      for (uint32_t i = 0; i < nbytes; ++i) {
        if (i % nbytes_per_row == 0 && i > 0)
          printf("\n");
        printf("%02X ", buffer[i]);
      }
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC void print_first3bits(uint64_t const* buffer, uint32_t size) const {
      for (uint32_t i = 0; i < size; ++i) {
        uint8_t const b61 = (buffer[i] >> 61) & 0x1;
        uint8_t const b62 = (buffer[i] >> 62) & 0x1;
        uint8_t const b63 = (buffer[i] >> 63) & 0x1;
        printf("[word: %u] %u%u%u\n", i, b63, b62, b61);
      }
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool is_barrel(uint8_t dccid) const {
      return dccid >= ElectronicsIdGPU::MIN_DCCID_EBM && dccid <= ElectronicsIdGPU::MAX_DCCID_EBP;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint8_t fed2dcc(int fed) const { return static_cast<uint8_t>(fed - 600); }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC int zside_for_eb(ElectronicsIdGPU const& eid) const {
      int dcc = eid.dccId();
      return ((dcc >= ElectronicsIdGPU::MIN_DCCID_EBM && dcc <= ElectronicsIdGPU::MAX_DCCID_EBM)) ? -1 : 1;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint8_t find_next_tower_block(uint64_t const*& current_tower_block,
                                                                 uint64_t const* trailer,
                                                                 uint32_t const bx,
                                                                 uint32_t const lv1) const {
      const auto* next_tower_block = current_tower_block + 1;  // move forward to skip the broken header

      // expected LV1, BX, #TS
      const uint64_t lv1local = ((lv1 - 1) & TOWER_L1_MASK);
      const uint64_t bxlocal = (bx != 3564) ? bx : 0;
      // The CPU unpacker also checks the # time samples expected in the header
      // but those are currently not available here

      // construct tower header and mask
      const uint64_t sign = 0xC0000000C0000000 + (lv1local << TOWER_L1_B) + (bxlocal << TOWER_BX_B);
      const uint64_t mask =
          0xC0001000D0000000 + (uint64_t(TOWER_L1_MASK) << TOWER_L1_B) + (uint64_t(TOWER_BX_MASK) << TOWER_BX_B);

      while (next_tower_block < trailer) {
        if ((*next_tower_block & mask) == sign) {
          current_tower_block = next_tower_block;
          return uint8_t(*next_tower_block & TOWER_ID_MASK);
        } else {
          ++next_tower_block;
        }
      }
      return TOWER_ID_MASK;  // return the maximum value
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool is_synced_towerblock(uint16_t const dccbx,
                                                             uint16_t const bx,
                                                             uint16_t const dccl1,
                                                             uint16_t const l1) const {
      bool const bxsync = (bx == 0 && dccbx == 3564) || (bx == dccbx && dccbx != 3564);
      bool const l1sync = (l1 == ((dccl1 - 1) & 0xfff));
      return bxsync && l1sync;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool right_tower_for_eb(int tower) const {
      // for EB, two types of tower (LVRB top/bottom)
      return (tower > 12 && tower < 21) || (tower > 28 && tower < 37) || (tower > 44 && tower < 53) ||
             (tower > 60 && tower < 69);
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint32_t compute_ebdetid(ElectronicsIdGPU const& eid) const {
      // as in Geometry/EcalMaping/.../EcalElectronicsMapping
      auto const dcc = eid.dccId();
      auto const tower = eid.towerId();
      auto const strip = eid.stripId();
      auto const xtal = eid.xtalId();

      int smid = 0;
      int iphi = 0;
      bool EBPlus = (zside_for_eb(eid) > 0);
      bool EBMinus = !EBPlus;

      if (zside_for_eb(eid) < 0) {
        smid = dcc + 19 - ElectronicsIdGPU::DCCID_PHI0_EBM;
        iphi = (smid - 19) * ElectronicsIdGPU::kCrystalsInPhi;
        iphi += 5 * ((tower - 1) % ElectronicsIdGPU::kTowersInPhi);
      } else {
        smid = dcc + 1 - ElectronicsIdGPU::DCCID_PHI0_EBP;
        iphi = (smid - 1) * ElectronicsIdGPU::kCrystalsInPhi;
        iphi += 5 * (ElectronicsIdGPU::kTowersInPhi - ((tower - 1) % ElectronicsIdGPU::kTowersInPhi) - 1);
      }

      bool RightTower = right_tower_for_eb(tower);
      int ieta = 5 * ((tower - 1) / ElectronicsIdGPU::kTowersInPhi) + 1;
      if (RightTower) {
        ieta += (strip - 1);
        if (strip % 2 == 1) {
          if (EBMinus)
            iphi += (xtal - 1) + 1;
          else
            iphi += (4 - (xtal - 1)) + 1;
        } else {
          if (EBMinus)
            iphi += (4 - (xtal - 1)) + 1;
          else
            iphi += (xtal - 1) + 1;
        }
      } else {
        ieta += 4 - (strip - 1);
        if (strip % 2 == 1) {
          if (EBMinus)
            iphi += (4 - (xtal - 1)) + 1;
          else
            iphi += (xtal - 1) + 1;
        } else {
          if (EBMinus)
            iphi += (xtal - 1) + 1;
          else
            iphi += (4 - (xtal - 1)) + 1;
        }
      }

      if (zside_for_eb(eid) < 0)
        ieta = -ieta;

      DetId did{DetId::Ecal, EcalBarrel};
      return did.rawId() | ((ieta > 0) ? (0x10000 | (ieta << 9)) : ((-ieta) << 9)) | (iphi & 0x1FF);
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC int adc(uint16_t sample) const { return sample & 0xfff; }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC int gainId(uint16_t sample) const { return (sample >> 12) & 0x3; }
  };

  ////////////////////////////////////////////////////////////////////////////
  // SR unpack kernel
  ////////////////////////////////////////////////////////////////////////////
  class Kernel_unpack_srflags {
  public:
    template <typename TAcc, typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
    ALPAKA_FN_ACC void operator()(TAcc const& acc,
                                  unsigned char const* __restrict__ data,
                                  uint32_t const* __restrict__ offsets,
                                  int const* __restrict__ feds,
                                  EcalSrFlagDeviceCollection::View srFlagsEB,
                                  EcalSrFlagDeviceCollection::View srFlagsEE,
                                  EcalSrMap const* srMap) const {
      auto const nFeds = alpaka::getWorkDiv<alpaka::Grid, alpaka::Blocks>(acc)[0u];

      for (auto ifed : cms::alpakatools::independent_groups(acc, nFeds)) {
        // offset in bytes
        auto const offset = offsets[ifed];
        // fed id
        auto const fed = feds[ifed];
        auto const dcc = fed2dcc(fed);
        auto const isBarrel = is_barrel(dcc);
        auto* srs = isBarrel ? srFlagsEB.flag() : srFlagsEE.flag();
        auto* srIds = isBarrel ? srFlagsEB.id() : srFlagsEE.id();
        auto* srCounter = isBarrel ? &srFlagsEB.size() : &srFlagsEE.size();

        // offset to the right raw buffer
        uint64_t const* buffer = reinterpret_cast<uint64_t const*>(data + offset);
        uint8_t const ntccblockwords = isBarrel ? 18 : 36;

        // selective readout block
        auto const* srp_block = buffer + HEADERLENGTH + ntccblockwords;
        auto const srp_header = *srp_block;
        //const uint8_t srp_id = srp_header & SRP_ID_MASK;
        uint8_t const n_sr_flags = (srp_header >> SRP_NFLAGS_B) & SRP_NFLAGS_MASK;
        // use 16 bit data words for SR flags
        uint16_t const* wp16 = reinterpret_cast<const uint16_t*>(srp_block + 1);

        for (auto idx : cms::alpakatools::independent_group_elements(acc, n_sr_flags)) {
          uint32_t wIdx;
          uint32_t srDetId = 0;
          if (isBarrel) {
            auto const kTowersInPhi = ElectronicsIdGPU::kTowersInPhi;

            auto srpIdx = idx;
            uint8_t const fov = (buffer[2] >> H_FOV_B) & H_FOV_MASK;
            // invert order of SR flags inside a 16 bit word for EB+
            if (NUMB_SM_EB_PLU_MIN <= dcc && dcc <= NUMB_SM_EB_PLU_MAX && fov >= 1) {
              auto u = idx % kTowersInPhi;
              u = kTowersInPhi - u;
              srpIdx = (idx / kTowersInPhi) * kTowersInPhi + u - 1;
            }
            auto const zside = zside_for_eb(dcc);
            auto const smId = (zside > 0) ? dcc - 27 : dcc + 9;
            int const etaTT = srpIdx / kTowersInPhi + 1;  // between 1 and 17
            int phiTT;
            if (zside > 0)
              phiTT = (smId - 1) * kTowersInPhi + kTowersInPhi - (srpIdx % kTowersInPhi) - 2;
            else
              phiTT = (smId - 19) * kTowersInPhi + srpIdx % kTowersInPhi - 1;
            if (phiTT <= 0)
              phiTT += 72;
            DetId tdetid{DetId::Ecal, EcalTriggerTower};
            srDetId = tdetid.rawId() | ((zside > 0) << 15) | 0x4000 | (etaTT << 7) | (phiTT & 0x7F);
            wIdx = idx;
          } else {
            auto const zside = zside_for_ee(dcc);
            auto const eeDccIdx = (zside > 0) ? dcc - 37 : dcc - 1;
            auto const srFlagOffset = srMap->srFlagOffsets[eeDccIdx];
            srDetId = srMap->scDetIds[srFlagOffset + idx];
            wIdx = srMap->srFlagWordIdx[srFlagOffset + idx];
          }
          auto const w16 = *(wp16 + wIdx / 4);  // 4 SR flags per 16 bit word
          uint8_t const flag = (w16 >> (3 * (wIdx % 4))) & SRP_SRFLAG_MASK;
          auto const pos = alpaka::atomicAdd(acc, srCounter, 1u, alpaka::hierarchy::Threads{});
          srIds[pos] = srDetId;
          srs[pos] = flag;
        }
      }
    }

  private:
    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool is_barrel(uint8_t dccid) const {
      return dccid >= ElectronicsIdGPU::MIN_DCCID_EBM && dccid <= ElectronicsIdGPU::MAX_DCCID_EBP;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint8_t fed2dcc(int fed) const { return static_cast<uint8_t>(fed - 600); }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC int zside_for_eb(int dcc) const {
      return (dcc >= ElectronicsIdGPU::MIN_DCCID_EBM && dcc <= ElectronicsIdGPU::MAX_DCCID_EBM) ? -1 : 1;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC int zside_for_ee(int dcc) const {
      return (dcc >= ElectronicsIdGPU::MIN_DCCID_EEM && dcc <= ElectronicsIdGPU::MAX_DCCID_EEM) ? -1 : 1;
    }
  };

  ////////////////////////////////////////////////////////////////////////////
  // Header unpack kernel
  ////////////////////////////////////////////////////////////////////////////
  class Kernel_unpack_headers {
  public:
    template <typename TAcc, typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
    ALPAKA_FN_ACC void operator()(TAcc const& acc,
                                  unsigned char const* __restrict__ data,
                                  uint32_t const* __restrict__ offsets,
                                  int const* __restrict__ feds,
                                  EcalDCCHeaderBlockDeviceCollection::View dccHeaders) const {
      // indices
      auto const ifed = alpaka::getIdx<alpaka::Grid, alpaka::Blocks>(acc)[0u];

      // offset in bytes
      auto const offset = offsets[ifed];
      // fed id
      auto const fed = feds[ifed];
      auto const dcc = fed2dcc(fed);
      auto const isBarrel = is_barrel(dcc);
      uint8_t const ntccblockwords = isBarrel ? 18 : 36;
      auto const gridDim = alpaka::getWorkDiv<alpaka::Grid, alpaka::Blocks>(acc)[0u];

      // offset to the right raw buffer
      uint64_t const* buffer = reinterpret_cast<uint64_t const*>(data + offset);

      // fed header
      auto const fed_header = buffer[0];
      uint32_t const fedId = (fed_header >> H_FEDID_B) & H_FEDID_MASK;
      if (!(fedId == static_cast<uint32_t>(fed))) {
        // mismatch with expected FED ID
      }
      dccHeaders.fedId()[ifed] = static_cast<int32_t>(fedId);
      dccHeaders.dccId()[ifed] = static_cast<int32_t>(fed2dcc(fedId));
      dccHeaders.BX()[ifed] = static_cast<int32_t>((fed_header >> H_BX_B) & H_BX_MASK);
      dccHeaders.LV1event()[ifed] = static_cast<int32_t>((fed_header >> H_L1_B) & H_L1_MASK);
      int16_t const triggerType = (fed_header >> H_TTYPE_B) & H_TTYPE_MASK;
      dccHeaders.basicTriggerType()[ifed] = triggerType;

      auto const w1 = buffer[1];
      dccHeaders.dccErrors()[ifed] = static_cast<int32_t>((w1 >> H_ERRORS_B) & H_ERRORS_MASK);
      dccHeaders.runNumber()[ifed] = static_cast<int32_t>((w1 >> H_RNUMB_B) & H_RNUMB_MASK);

      auto const w2 = buffer[2];
      int16_t const runType = w2 & H_RTYPE_MASK;
      uint16_t const detailedTrgType = (w2 >> H_DET_TTYPE_B) & H_DET_TTYPE_MASK;
      decodeRunType(triggerType, detailedTrgType, runType, ifed, dccHeaders);

      auto const w3 = buffer[3];
      dccHeaders.orbitNumber()[ifed] = (w3 >> H_ORBITCOUNTER_B) & H_ORBITCOUNTER_MASK;
      dccHeaders.selectiveReadout()[ifed] = (w3 >> H_SR_B) & B_MASK;
      dccHeaders.zeroSuppression()[ifed] = (w3 >> H_ZS_B) & B_MASK;
      dccHeaders.testZeroSuppression()[ifed] = (w3 >> H_TZS_B) & B_MASK;
      dccHeaders.srpStatus()[ifed] = (w3 >> H_SRCHSTATUS_B) & H_CHSTATUS_MASK;
      auto* tccChStatus = dccHeaders.tccStatus()->data();
      tccChStatus[0] = (w3 >> H_TCC1CHSTATUS_B) & H_CHSTATUS_MASK;
      tccChStatus[1] = (w3 >> H_TCC2CHSTATUS_B) & H_CHSTATUS_MASK;
      tccChStatus[2] = (w3 >> H_TCC3CHSTATUS_B) & H_CHSTATUS_MASK;
      tccChStatus[3] = (w3 >> H_TCC4CHSTATUS_B) & H_CHSTATUS_MASK;

      // FE channel Status data
      auto* feChStatus = dccHeaders.feStatus()->data();
      uint8_t ch(0);
      uint8_t shift(0);
      for (uint8_t i = 4; i < HEADERLENGTH; ++i) {                    // 4 data words with channel status info
        for (uint8_t j = 0; j < 14 && ch < MAX_TT_SIZE; ++j, ++ch) {  // 14 channel status fields in one data word
          shift = j * 4;                                              //each channel has 4 bits
          feChStatus[ch] = (buffer[i] >> shift) & H_CHSTATUS_MASK;
        }
      }
      dccHeaders.feStatusSize()[ifed] = ch;

      // TCC headers information
      auto* tccBx = dccHeaders.tccBx()->data();
      auto* tccLv1 = dccHeaders.tccLv1()->data();
      auto const* tcc_blocks_start = buffer + HEADERLENGTH;
      auto const* sr_block_start = buffer + HEADERLENGTH + ntccblockwords;  // SR block follows the TCC blocks
      auto const* current_tcc_block = tcc_blocks_start;
      uint8_t ctr(0);
      while (current_tcc_block < sr_block_start) {
        auto const tcc_header = *current_tcc_block;

        tccBx[ctr] = (tcc_header >> TCC_BX_B) & TCC_BX_MASK;
        tccLv1[ctr] = (tcc_header >> TCC_L1_B) & TCC_L1_MASK;

        uint8_t const nTTs = (tcc_header >> TCC_TT_B) & TCC_TT_MASK;
        uint8_t const nTSamples = (tcc_header >> TCC_TS_B) & TCC_TS_MASK;
        uint16_t const block_length =
            (nTTs * nTSamples) % 4 > 0 ? 1 + (nTTs * nTSamples) / 4 : 2 + (nTTs * nTSamples) / 4;
        current_tcc_block += block_length;
        ++ctr;
      }
      dccHeaders.tccBxSize()[ifed] = ctr;
      dccHeaders.tccLv1Size()[ifed] = ctr;

      // selective readout block
      auto const* srp_block = buffer + HEADERLENGTH + ntccblockwords;
      auto const srp_header = *srp_block;
      dccHeaders.srpBx()[ifed] = (srp_header >> SRP_BX_B) & SRP_BX_MASK;
      dccHeaders.srpLv1()[ifed] = (srp_header >> SRP_L1_B) & SRP_L1_MASK;

      // FE headers information

      dccHeaders.zs()[ifed] = -1;  // TODO seems unused in CPU unpacker
    }

  private:
    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool is_barrel(uint8_t dccid) const {
      return dccid >= ElectronicsIdGPU::MIN_DCCID_EBM && dccid <= ElectronicsIdGPU::MAX_DCCID_EBP;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint8_t fed2dcc(int fed) const { return static_cast<uint8_t>(fed - 600); }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC void decodeRunType(int16_t trigType,
                                                      int16_t detTrigType,
                                                      int16_t runType,
                                                      uint32_t ifed,
                                                      EcalDCCHeaderBlockDeviceCollection::View dccHeaders) const {
      constexpr unsigned int kWhichHalfOffset = 64;    //2^6
      constexpr unsigned int kTypeOffset = 256;        //2^8
      constexpr unsigned int kSubTypeOffset = 2048;    //2^11
      constexpr unsigned int kSettingOffset = 131072;  //2^17
      constexpr unsigned int kGainModeOffset = 16384;  //2^14

      constexpr unsigned int kTwoBitsMask = 3;
      constexpr unsigned int kThreeBitsMask = 7;
      constexpr unsigned int kThirdBitMask = 4;

      dccHeaders.rtHalf()[ifed] = static_cast<int>((runType / kWhichHalfOffset) & kTwoBitsMask);
      auto const type = static_cast<int>((runType / kTypeOffset) & kThreeBitsMask);
      auto const sequence = static_cast<int>((runType / kSubTypeOffset) & kThreeBitsMask);
      dccHeaders.mgpaGain()[ifed] = static_cast<int>((runType / kGainModeOffset) & kTwoBitsMask);
      dccHeaders.memGain()[ifed] = static_cast<int>((runType / kGainModeOffset) & kThirdBitMask) / kThirdBitMask;

      if (type == 0 && sequence <= 1) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::COSMIC;
      } else if (type == 0 && sequence == 2) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::BEAMH4;
      } else if (type == 0 && sequence == 3) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::BEAMH2;
      } else if (type == 0 && sequence == 4) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::MTCC;
      } else if (type == 1 && sequence == 0) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::LASER_STD;
      } else if (type == 1 && sequence == 1) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::LASER_POWER_SCAN;
      } else if (type == 1 && sequence == 2) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::LASER_DELAY_SCAN;
      } else if (type == 2 && sequence == 0) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::TESTPULSE_SCAN_MEM;
      } else if (type == 2 && sequence == 1) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::TESTPULSE_MGPA;
      } else if (type == 3 && sequence == 0) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::PEDESTAL_STD;
      } else if (type == 3 && sequence == 1) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::PEDESTAL_OFFSET_SCAN;
      } else if (type == 3 && sequence == 2) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::PEDESTAL_25NS_SCAN;
      } else if (type == 4 && sequence == 0) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::LED_STD;
      } else if (type == 5 && sequence == 0) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::PHYSICS_GLOBAL;
      } else if (type == 5 && sequence == 1) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::COSMICS_GLOBAL;
      } else if (type == 5 && sequence == 2) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::HALO_GLOBAL;
      } else if (type == 6 && sequence == 0) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::PHYSICS_LOCAL;
      } else if (type == 6 && sequence == 1) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::COSMICS_LOCAL;
      } else if (type == 6 && sequence == 2) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::HALO_LOCAL;
      } else if (type == 6 && sequence == 3) {
        dccHeaders.runType()[ifed] = EcalDCCHeaderBlock::CALIB_LOCAL;
      }
      // decoding of settings depends on whether run is global or local
      if (type == 5 || type == 6) {
        decodeSettingGlobal(trigType, detTrigType, ifed, dccHeaders);
      } else {
        decodeSetting(int(runType / kSettingOffset), ifed, dccHeaders);
      }
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC void decodeSettingGlobal(int16_t trigType,
                                                            int16_t detTrigType,
                                                            uint32_t ifed,
                                                            EcalDCCHeaderBlockDeviceCollection::View dccHeaders) const {
      dccHeaders.dccInTTCCommand()[ifed] = (detTrigType >> H_DCCID_B) & H_DCCID_MASK;
      // if trigger type is physics, only dccIdInTTCCommand is meaningful
      if (trigType == PHYSICTRIGGER) {
        return;
      }

      // if calibration trigger (gap)
      if (trigType == CALIBRATIONTRIGGER) {
        // TCC commands are decoded both in global (type == 5) and in local (type == 6)
        // local or global calibration
        bool const isLocal(dccHeaders.runType()[ifed] == EcalDCCHeaderBlock::CALIB_LOCAL);

        EcalDCCHeaderBlock::EcalDCCEventSettings evtSettings;
        evtSettings.LaserPower = -1;
        evtSettings.LaserFilter = -1;
        evtSettings.wavelength = -1;
        evtSettings.delay = -1;
        evtSettings.MEMVinj = -1;
        evtSettings.mgpa_content = -1;
        evtSettings.ped_offset = -1;

        int const detailedTriggerTypeInTTCCommand = (detTrigType >> H_TR_TYPE_B) & H_TR_TYPE_MASK;
        int const wavelengthInTTCCommand = (detTrigType >> H_WAVEL_B) & H_WAVEL_MASK;

        dccHeaders.rtHalf()[ifed] = (detTrigType >> H_HALF_B) & H_HALF_MASK;
        if (detailedTriggerTypeInTTCCommand == EcalDCCHeaderBlock::TTC_LASER) {
          dccHeaders.runType()[ifed] = isLocal ? EcalDCCHeaderBlock::LASER_STD : EcalDCCHeaderBlock::LASER_GAP;
          evtSettings.wavelength = wavelengthInTTCCommand;
        } else if (detailedTriggerTypeInTTCCommand == EcalDCCHeaderBlock::TTC_LED) {
          dccHeaders.runType()[ifed] = isLocal ? EcalDCCHeaderBlock::LED_STD : EcalDCCHeaderBlock::LED_GAP;
          evtSettings.wavelength = wavelengthInTTCCommand;
        } else if (detailedTriggerTypeInTTCCommand == EcalDCCHeaderBlock::TTC_TESTPULSE) {
          dccHeaders.runType()[ifed] = isLocal ? EcalDCCHeaderBlock::TESTPULSE_MGPA : EcalDCCHeaderBlock::TESTPULSE_GAP;
        } else if (detailedTriggerTypeInTTCCommand == EcalDCCHeaderBlock::TTC_PEDESTAL) {
          dccHeaders.runType()[ifed] = isLocal ? EcalDCCHeaderBlock::PEDESTAL_STD : EcalDCCHeaderBlock::PEDESTAL_GAP;
        } else {
          dccHeaders.runType()[ifed] = -1;
        }
        dccHeaders.eventSettings()[ifed] = evtSettings;
      }
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC void decodeSetting(int setting,
                                                      uint32_t ifed,
                                                      EcalDCCHeaderBlockDeviceCollection::View dccHeaders) const {
      EcalDCCHeaderBlock::EcalDCCEventSettings evtSettings;
      evtSettings.LaserPower = -1;
      evtSettings.LaserFilter = -1;
      evtSettings.wavelength = -1;
      evtSettings.delay = -1;
      evtSettings.MEMVinj = -1;
      evtSettings.mgpa_content = -1;
      evtSettings.ped_offset = -1;

      auto const runType = dccHeaders.runType()[ifed];
      if (runType == EcalDCCHeaderBlock::LASER_STD || runType == EcalDCCHeaderBlock::LASER_POWER_SCAN) {
        evtSettings.LaserPower = (setting & 8128) / 64;
        evtSettings.LaserFilter = (setting & 56) / 8;
        evtSettings.wavelength = setting & 7;
      } else if (runType == EcalDCCHeaderBlock::LASER_DELAY_SCAN) {
        evtSettings.wavelength = setting & 7;
        evtSettings.delay = (setting & 2040) / 8;
      } else if (runType == EcalDCCHeaderBlock::TESTPULSE_SCAN_MEM) {
        evtSettings.MEMVinj = setting & 511;
      } else if (runType == EcalDCCHeaderBlock::TESTPULSE_MGPA) {
        evtSettings.mgpa_content = setting & 255;
      } else if (runType == EcalDCCHeaderBlock::PEDESTAL_OFFSET_SCAN) {
        evtSettings.ped_offset = setting;
      } else if (runType == EcalDCCHeaderBlock::PEDESTAL_25NS_SCAN) {
        evtSettings.delay = (setting & 255);
      } else if (runType == EcalDCCHeaderBlock::LED_STD) {
        evtSettings.wavelength = setting & 7;
      }
      dccHeaders.eventSettings()[ifed] = evtSettings;
    }
  };

  ////////////////////////////////////////////////////////////////////////////
  // TCC unpack kernel
  ////////////////////////////////////////////////////////////////////////////
  class Kernel_unpack_tcc {
  public:
    template <typename TAcc, typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
    ALPAKA_FN_ACC void operator()(TAcc const& acc,
                                  unsigned char const* __restrict__ data,
                                  uint32_t const* __restrict__ offsets,
                                  int const* __restrict__ feds,
                                  EcalTriggerPrimitiveDigiDeviceCollection::View tpDigis,
                                  EcalPseudoStripInputDigiDeviceCollection::View psDigis) const {
      constexpr auto kTpSampleSize = EcalTriggerPrimitiveDigi::MAXSAMPLES;
      constexpr auto kPsSampleSize = EcalTriggerPrimitiveDigi::MAXSAMPLES;

      auto const nFeds = alpaka::getWorkDiv<alpaka::Grid, alpaka::Blocks>(acc)[0u];

      for (auto ifed : cms::alpakatools::independent_groups(acc, nFeds)) {
        // offset in bytes
        auto const offset = offsets[ifed];
        // fed id
        auto const fed = feds[ifed];
        auto const dcc = fed2dcc(fed);
        auto const isBarrel = is_barrel(dcc);
        uint8_t const ntccblockwords = isBarrel ? 18 : 36;

        auto* tps = tpDigis.data()->data();
        auto* tpIds = tpDigis.id();
        auto* tpCounter = &tpDigis.size();

        auto* pss = psDigis.data()->data();
        auto* psIds = psDigis.id();
        auto* psCounter = &psDigis.size();

        // offset to the right raw buffer
        uint64_t const* buffer = reinterpret_cast<uint64_t const*>(data + offset);

        // DCC header to compare with TCC header fields
        auto const fed_header = buffer[0];
        uint32_t bx = (fed_header >> H_BX_B) & H_BX_MASK;
        uint32_t lv1 = (fed_header >> H_L1_B) & H_L1_MASK;
        uint8_t const dccfov = (buffer[2] >> H_FOV_B) & H_FOV_MASK;
        //bool tccChStatus[4];  // TODO implement status check
        //tccChStatus[0] = (buffer[3] >> H_TCC1CHSTATUS_B) & B_MASK;
        //tccChStatus[1] = (buffer[3] >> H_TCC2CHSTATUS_B) & B_MASK;
        //tccChStatus[2] = (buffer[3] >> H_TCC3CHSTATUS_B) & B_MASK;
        //tccChStatus[3] = (buffer[3] >> H_TCC4CHSTATUS_B) & B_MASK;

        // trigger concentrator card block
        auto const* tcc_blocks_start = buffer + HEADERLENGTH;
        auto const* sr_block_start = buffer + HEADERLENGTH + ntccblockwords;  // SR block follows the TCC blocks
        auto const* current_tcc_block = tcc_blocks_start;
        while (current_tcc_block < sr_block_start) {
          auto const tcc_header = *current_tcc_block;

          // check that this is a TCC block word
          if ((((tcc_header >> TCC_BLKIDENTIFIER_B) & TCC_BLKIDENTIFIER_MASK) != TCC_BLKIDENTIFIER) ||
              (((tcc_header >> (TCC_BLKIDENTIFIER_B + 32)) & TCC_BLKIDENTIFIER_MASK) != TCC_BLKIDENTIFIER)) {
            // TODO try next word
          }

          // TCC header information
          uint8_t const tccId = tcc_header & TCC_ID_MASK;
          bool const ps = (tcc_header >> TCC_PS_B) & B_MASK;
          //uint16_t const bxlocal = (tcc_header >> TCC_BX_B) & TCC_BX_MASK;
          bool const e0 = (tcc_header >> TCC_E0_B) & B_MASK;
          //uint16_t const lv1local = (tcc_header >> TCC_L1_B) & TCC_L1_MASK;
          bool const e1 = (tcc_header >> TCC_E1_B) & B_MASK;
          uint8_t const nTTs = (tcc_header >> TCC_TT_B) & TCC_TT_MASK;
          uint8_t const nTSamples = (tcc_header >> TCC_TS_B) & TCC_TS_MASK;

          uint16_t const block_length = isBarrel ? (nTTs * nTSamples) % 4 == 0 ? 1 + (nTTs * nTSamples) / 4 : 2 + (nTTs * nTSamples) / 4 : 9;

          if (ps) {
            // TODO not implemented
          }

          if (e0 || e1) {
            // data corruption detected. local BX or L1 mismatch with DCC header
            // TODO something
          }

          // check expected TCC id
          uint8_t exp_tccid = isBarrel ? dcc + TCCID_SMID_SHIFT_EB : 0;
          if (tccId != exp_tccid) {
            // TODO not the expected TCC ID
          }

          // use 16 bit data words for TPs
          uint16_t const* wtt = reinterpret_cast<const uint16_t*>(current_tcc_block + 1);

          // 1 element per TP in this block
          for (auto idx : cms::alpakatools::independent_group_elements(acc, nTTs)) {
            uint16_t sampleValues[kTpSampleSize];
            auto* tpsamples_start = wtt;
            auto tpIdx = idx;

            if (isBarrel) {
              // invert order of TPs inside a 64 bit word for EB+
              if (NUMB_SM_EB_PLU_MIN <= dcc && dcc <= NUMB_SM_EB_PLU_MAX) {
                auto const kTowersInPhi = ElectronicsIdGPU::kTowersInPhi;
                auto u = idx % kTowersInPhi;
                u = kTowersInPhi - u;
                tpIdx = (idx / kTowersInPhi) * kTowersInPhi + u - 1;
              }
              tpsamples_start += idx * nTSamples;
            } else {
              auto iloc = idx;

              /////////////////////////
              // MC raw data based on CMS NOTE 2005/021
              // (and raw data when FOV was unassigned, earlier than mid 2008)
              if (dccfov == dcc_FOV_1 || dccfov == dcc_FOV_2) {
                ///////////////////////////
                // real data since ever FOV was initialized; only 2 used >= June 09
                if (idx > 7 && isTCCExternal(tccId)) {
                  // skip blank TPs [9,16] for external TCC
                  iloc += 8;
                }
              }
              tpsamples_start += iloc * nTSamples;
            }
            for (size_t si = 0; si < kTpSampleSize; ++si) {
              sampleValues[si] = (si < nTSamples) ? *(tpsamples_start + si) & 0xffff : 0;  // only 12 bits are meaningful but the CPU unpacker also stores all 16
            }
            // store to global
            auto const pos = alpaka::atomicAdd(acc, tpCounter, 1u, alpaka::hierarchy::Threads{});
            //printf("%u %u %u %u %u %u %u %u %ld %lx %u\n", ifed, dcc, idx, tpIdx, pos, *tpCounter, nTTs, block_length, sr_block_start - current_tcc_block, *(current_tcc_block + 1 + idx / 4), sampleValues[0]);
            tpIds[pos] = getTrigTowerDetId(tccId, tpIdx, isBarrel);
            std::memcpy(&tps[pos * kTpSampleSize], sampleValues, kTpSampleSize * sizeof(uint16_t));
          }

          current_tcc_block += block_length;
        }
      }
    }

  private:
    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool is_barrel(uint8_t dccid) const {
      return dccid >= ElectronicsIdGPU::MIN_DCCID_EBM && dccid <= ElectronicsIdGPU::MAX_DCCID_EBP;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint8_t fed2dcc(int fed) const { return static_cast<uint8_t>(fed - 600); }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool isTCCExternal(uint32_t tccId) const {
      return ((NUMB_TCC_EE_MIN_EXT_MIN <= tccId && tccId <= NUMB_TCC_EE_MIN_EXT_MAX) ||
              (NUMB_TCC_EE_PLU_EXT_MIN <= tccId && tccId <= NUMB_TCC_EE_PLU_EXT_MAX));
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC int32_t zside(int tcc) const {
      //int EcalElectronicsMapping::zside(int dcctcc, int mode) const {
      if (tcc >= EcalElectronicsMapping::MIN_TCCID_EBM && tcc <= EcalElectronicsMapping::MAX_TCCID_EBM)
        return -1;
      if (tcc >= EcalElectronicsMapping::MIN_TCCID_EBP && tcc <= EcalElectronicsMapping::MAX_TCCID_EBP)
        return +1;
      if (tcc >= EcalElectronicsMapping::MIN_TCCID_EEM && tcc <= EcalElectronicsMapping::MAX_TCCID_EEM)
        return -1;
      if (tcc >= EcalElectronicsMapping::MIN_TCCID_EEP && tcc <= EcalElectronicsMapping::MAX_TCCID_EEP)
        return +1;
      return 0;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint32_t getTrigTowerDetId(int tccId, int iTT, bool isBarrel) const {
      auto const zIndex = zside(tccId);
      DetId tdetid{DetId::Ecal, EcalTriggerTower};
      uint32_t id = tdetid.rawId() | ((zIndex > 0) << 15);

      if (isBarrel) {
        auto const dccId =
            zIndex > 0 ? tccId - EcalElectronicsMapping::TCCID_PHI0_EBP + EcalElectronicsMapping::DCCID_PHI0_EBP
                       : tccId - EcalElectronicsMapping::TCCID_PHI0_EBM + EcalElectronicsMapping::DCCID_PHI0_EBM;
        auto const smId = zIndex > 0 ? dccId - 27 : dccId + 9;
        auto const jtower = iTT;
        auto const kTowersInPhi = ElectronicsIdGPU::kTowersInPhi;

        auto const etaTT = jtower / kTowersInPhi + 1;  // between 1 and 17
        auto phiTT = zIndex > 0 ? (smId - 1) * kTowersInPhi + kTowersInPhi - (jtower % kTowersInPhi) - 2
                                : (smId - 19) * kTowersInPhi + jtower % kTowersInPhi - 1;
        if (phiTT <= 0) {
          phiTT += 72;
        }

        id |= 0x4000 | (etaTT << 7) | (phiTT & 0x7F);
        return id;
      } else {
        auto const kTCCinPhi = EcalElectronicsMapping::kTCCinPhi;
        int iz = 0;
        if (tccId < EcalElectronicsMapping::TCCID_PHI0_EEM_OUT + kTCCinPhi)
          iz = -1;
        else if (tccId >= EcalElectronicsMapping::TCCID_PHI0_EEP_OUT)
          iz = +1;

        bool const inner = (iz < 0 && tccId >= EcalElectronicsMapping::TCCID_PHI0_EEM_IN &&
                            tccId < EcalElectronicsMapping::TCCID_PHI0_EEM_IN + kTCCinPhi) ||
                           (iz > 0 && tccId >= EcalElectronicsMapping::TCCID_PHI0_EEP_IN &&
                            tccId < EcalElectronicsMapping::TCCID_PHI0_EEP_IN + kTCCinPhi);
        bool const outer = !inner;

        int ieta = iTT / EcalElectronicsMapping::kEETowersInPhiPerTCC;
        int iphi = iTT % EcalElectronicsMapping::kEETowersInPhiPerTCC;
        if (inner)
          ieta += EcalElectronicsMapping::iEEEtaMinInner;
        else
          ieta += EcalElectronicsMapping::iEEEtaMinOuter;

        if (inner && iz < 0) {
          tccId -= EcalElectronicsMapping::TCCID_PHI0_EEM_IN;
        } else if (outer && iz < 0) {
          tccId -= EcalElectronicsMapping::TCCID_PHI0_EEM_OUT;
        } else if (inner && iz > 0) {
          tccId -= EcalElectronicsMapping::TCCID_PHI0_EEP_IN;
        } else if (outer && iz > 0) {
          tccId -= EcalElectronicsMapping::TCCID_PHI0_EEP_OUT;
        }

        iphi += EcalElectronicsMapping::kEETowersInPhiPerTCC * tccId;
        iphi = (iphi - 2 + 4 * EcalElectronicsMapping::kEETowersInPhiPerQuadrant) %
                   (4 * EcalElectronicsMapping::kEETowersInPhiPerQuadrant) +
               1;

        id |= (abs(ieta) << 7) | (iphi & 0x7F);
        return id;
      }
    }
  };

  ////////////////////////////////////////////////////////////////////////////
  // MEM unpack kernel
  ////////////////////////////////////////////////////////////////////////////
  class Kernel_unpack_mem {
  public:
    template <typename TAcc, typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
    ALPAKA_FN_ACC void operator()(TAcc const& acc,
                                  unsigned char const* __restrict__ data,
                                  uint32_t const* __restrict__ offsets,
                                  int const* __restrict__ feds,
                                  EcalIdDeviceCollection::View integrityMemTtIdErrors,
                                  EcalIdDeviceCollection::View integrityMemBlockSizeErrors,
                                  EcalIdDeviceCollection::View integrityMemChIdErrors,
                                  EcalIdDeviceCollection::View integrityMemGainErrors) const {
      // indices
      auto const ifed = alpaka::getIdx<alpaka::Grid, alpaka::Blocks>(acc)[0u];

      // offset in bytes
      auto const offset = offsets[ifed];
      // fed id
      auto const fed = feds[ifed];
      auto const dcc = fed2dcc(fed);
    }

  private:
    ALPAKA_FN_INLINE ALPAKA_FN_ACC bool is_barrel(uint8_t dccid) const {
      return dccid >= ElectronicsIdGPU::MIN_DCCID_EBM && dccid <= ElectronicsIdGPU::MAX_DCCID_EBP;
    }

    ALPAKA_FN_INLINE ALPAKA_FN_ACC uint8_t fed2dcc(int fed) const { return static_cast<uint8_t>(fed - 600); }
  };

  ////////////////////////////////////////////////////////////////////////////
  // Unpack digis
  ////////////////////////////////////////////////////////////////////////////
  void unpackFe(Queue& queue,
                InputDataDevice const& inputDevice,
                EcalDigiDeviceCollection& digisDevEB,
                EcalDigiDeviceCollection& digisDevEE,
                EcalIdDeviceCollection& integrityGainErrorsDevEB,
                EcalIdDeviceCollection& integrityGainErrorsDevEE,
                EcalIdDeviceCollection& integrityGainSwitchErrorsDevEB,
                EcalIdDeviceCollection& integrityGainSwitchErrorsDevEE,
                EcalIdDeviceCollection& integrityChIdErrorsDevEB,
                EcalIdDeviceCollection& integrityChIdErrorsDevEE,
                EcalIdDeviceCollection& integrityTTIdErrorsDev,
                EcalIdDeviceCollection& integrityZSXtalIdErrorsDev,
                EcalIdDeviceCollection& integrityBlockSizeErrorsDev,
                EcalPnDiodeDigiDeviceCollection& pnDiodeDigisDev,
                EcalElectronicsMappingDevice const& mapping,
                uint32_t const nfedsWithData,
                uint32_t const nbytesTotal) {
    auto workDiv = cms::alpakatools::make_workdiv<Acc1D>(nfedsWithData, 32);  // 32 channels per block
    alpaka::exec<Acc1D>(queue,
                        workDiv,
                        Kernel_unpack{},
                        inputDevice.data.data(),
                        inputDevice.offsets.data(),
                        inputDevice.feds.data(),
                        digisDevEB.view(),
                        digisDevEE.view(),
                        integrityGainErrorsDevEB.view(),
                        integrityGainErrorsDevEE.view(),
                        integrityGainSwitchErrorsDevEB.view(),
                        integrityGainSwitchErrorsDevEE.view(),
                        integrityChIdErrorsDevEB.view(),
                        integrityChIdErrorsDevEE.view(),
                        integrityTTIdErrorsDev.view(),
                        integrityZSXtalIdErrorsDev.view(),
                        integrityBlockSizeErrorsDev.view(),
                        pnDiodeDigisDev.view(),
                        mapping.const_view(),
                        nbytesTotal);
  }

  ////////////////////////////////////////////////////////////////////////////
  // Unpack SR flags
  ////////////////////////////////////////////////////////////////////////////
  void unpackSrFlags(Queue& queue,
                     InputDataDevice const& inputDevice,
                     EcalSrFlagDeviceCollection& srFlagsDevEB,
                     EcalSrFlagDeviceCollection& srFlagsDevEE,
                     EcalSrMappingDevice const& srMapping,
                     uint32_t const nfedsWithData) {
    auto workDiv = cms::alpakatools::make_workdiv<Acc1D>(nfedsWithData, 32);  // 32 channels per block
    alpaka::exec<Acc1D>(queue,
                        workDiv,
                        Kernel_unpack_srflags{},
                        inputDevice.data.data(),
                        inputDevice.offsets.data(),
                        inputDevice.feds.data(),
                        srFlagsDevEB.view(),
                        srFlagsDevEE.view(),
                        srMapping.const_data());
  }

  ////////////////////////////////////////////////////////////////////////////
  // Unpack headers
  ////////////////////////////////////////////////////////////////////////////
  void unpackHeaders(Queue& queue,
                     InputDataDevice const& inputDevice,
                     EcalDCCHeaderBlockDeviceCollection& dccHeaders,
                     uint32_t const nfedsWithData) {
    auto workDiv = cms::alpakatools::make_workdiv<Acc1D>(nfedsWithData, 32);  // 32 channels per block
    alpaka::exec<Acc1D>(queue,
                        workDiv,
                        Kernel_unpack_headers{},
                        inputDevice.data.data(),
                        inputDevice.offsets.data(),
                        inputDevice.feds.data(),
                        dccHeaders.view());
  }

  ////////////////////////////////////////////////////////////////////////////
  // Unpack TCC
  ////////////////////////////////////////////////////////////////////////////
  void unpackTcc(Queue& queue,
                 InputDataDevice const& inputDevice,
                 EcalTriggerPrimitiveDigiDeviceCollection& tpDigisDev,
                 EcalPseudoStripInputDigiDeviceCollection& psDigisDev,
                 uint32_t const nfedsWithData) {
    auto workDiv = cms::alpakatools::make_workdiv<Acc1D>(nfedsWithData, 32);  // 32 channels per block
    alpaka::exec<Acc1D>(queue,
                        workDiv,
                        Kernel_unpack_tcc{},
                        inputDevice.data.data(),
                        inputDevice.offsets.data(),
                        inputDevice.feds.data(),
                        tpDigisDev.view(),
                        psDigisDev.view());
  }

  ////////////////////////////////////////////////////////////////////////////
  // Unpack MEM
  ////////////////////////////////////////////////////////////////////////////
  void unpackMem(Queue& queue,
                 InputDataDevice const& inputDevice,
                 EcalIdDeviceCollection& integrityMemTtIdErrorsDev,
                 EcalIdDeviceCollection& integrityMemBlockSizeErrorsDev,
                 EcalIdDeviceCollection& integrityMemChIdErrorsDev,
                 EcalIdDeviceCollection& integrityMemGainErrorsDev,
                 uint32_t const nfedsWithData) {
    auto workDiv = cms::alpakatools::make_workdiv<Acc1D>(nfedsWithData, 32);  // 32 channels per block
    alpaka::exec<Acc1D>(queue,
                        workDiv,
                        Kernel_unpack_mem{},
                        inputDevice.data.data(),
                        inputDevice.offsets.data(),
                        inputDevice.feds.data(),
                        integrityMemTtIdErrorsDev.view(),
                        integrityMemBlockSizeErrorsDev.view(),
                        integrityMemChIdErrorsDev.view(),
                        integrityMemGainErrorsDev.view());
  }

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::raw
