#ifndef EventFilter_EcalRawToDigi_interface_EcalSrMap_h
#define EventFilter_EcalRawToDigi_interface_EcalSrMap_h

#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "EventFilter/EcalRawToDigi/interface/ElectronicsIdGPU.h"

#include <array>

struct EcalSrMap {
  static constexpr size_t kNSrFlagsEE = 2 * EcalScDetId::SC_PER_EE_CNT;
  static constexpr size_t kNDccEE =
      ecal::raw::ElectronicsIdGPU::MAX_DCCID_EEM - ecal::raw::ElectronicsIdGPU::MIN_DCCID_EEM +
      ecal::raw::ElectronicsIdGPU::MAX_DCCID_EEP - ecal::raw::ElectronicsIdGPU::MIN_DCCID_EEP + 2;
  std::array<uint32_t, kNDccEE> srFlagOffsets;
  std::array<uint32_t, kNSrFlagsEE> scDetIds;
  std::array<uint32_t, kNSrFlagsEE> srFlagWordIdx;
};

#endif  // EventFilter_EcalRawToDigi_plugins_alpaka_DeclsForKernels_h
