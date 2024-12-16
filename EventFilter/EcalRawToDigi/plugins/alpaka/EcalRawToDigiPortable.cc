#include "CondFormats/DataRecord/interface/EcalMappingElectronicsRcd.h"
#include "CondFormats/EcalObjects/interface/alpaka/EcalElectronicsMappingDevice.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/alpaka/EcalIdDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalDigiDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalSrFlagDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalPnDiodeDigiDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalPseudoStripInputDigiDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalTriggerPrimitiveDigiDeviceCollection.h"
#include "DataFormats/EcalRawData/interface/alpaka/EcalDCCHeaderBlockDeviceCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "EventFilter/EcalRawToDigi/interface/DCCRawDataDefinitions.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/EmptyGroupDescription.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/ESGetToken.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EDGetToken.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EDPutToken.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/Event.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EventSetup.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/stream/EDProducer.h"

#include <alpaka/alpaka.hpp>

#include "DeclsForKernels.h"
#include "UnpackPortable.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  using namespace cms::alpakatools;

  class EcalRawToDigiPortable : public stream::EDProducer<> {
  public:
    explicit EcalRawToDigiPortable(edm::ParameterSet const& ps);
    ~EcalRawToDigiPortable() override = default;
    static void fillDescriptions(edm::ConfigurationDescriptions&);

    void produce(device::Event&, device::EventSetup const&) override;

  private:
    bool const unpackHeader_;
    bool const unpackSrFlags_;
    bool const unpackTcc_;
    bool const unpackFe_;
    bool const unpackMem_;

    edm::EDGetTokenT<FEDRawDataCollection> const rawDataToken_;

    // digis collections
    device::EDPutToken<EcalDigiDeviceCollection> digisDevEBToken_;
    device::EDPutToken<EcalDigiDeviceCollection> digisDevEEToken_;
    // SR flag collections
    device::EDPutToken<EcalSrFlagDeviceCollection> srFlagsDevEBToken_;
    device::EDPutToken<EcalSrFlagDeviceCollection> srFlagsDevEEToken_;
    // integrity DetId collections for xtal data - EB
    device::EDPutToken<EcalIdDeviceCollection> integrityGainErrorsDevEBToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityGainSwitchErrorsDevEBToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityChIdErrorsDevEBToken_;
    // integrity DetId collections for xtal data - EE
    device::EDPutToken<EcalIdDeviceCollection> integrityGainErrorsDevEEToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityGainSwitchErrorsDevEEToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityChIdErrorsDevEEToken_;
    // PN diode digis
    device::EDPutToken<EcalPnDiodeDigiDeviceCollection> pnDiodeDigisDevToken_;
    // TCC collections
    device::EDPutToken<EcalTriggerPrimitiveDigiDeviceCollection> ecalTriggerPrimitivesDevToken_;
    device::EDPutToken<EcalPseudoStripInputDigiDeviceCollection> ecalPseudoStripInputsDevToken_;
    // integrity errors
    device::EDPutToken<EcalIdDeviceCollection> integrityTTIdErrorsDevToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityZSXtalIdErrorsDevToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityBlockSizeErrorsDevToken_;
    // mem integrity collections
    device::EDPutToken<EcalIdDeviceCollection> integrityMemTtIdErrorsDevToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityMemBlockSizeErrorsDevToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityMemChIdErrorsDevToken_;
    device::EDPutToken<EcalIdDeviceCollection> integrityMemGainErrorsDevToken_;
    // ECAL raw data collection
    device::EDPutToken<EcalDCCHeaderBlockDeviceCollection> ecalRawDataDevToken_;

    device::ESGetToken<EcalElectronicsMappingDevice, EcalMappingElectronicsRcd> const eMappingToken_;

    std::vector<int> const fedsToUnpack_;

    ecal::raw::ConfigurationParameters config_;
  };

  void EcalRawToDigiPortable::fillDescriptions(edm::ConfigurationDescriptions& confDesc) {
    edm::ParameterSetDescription desc;

    desc.add<edm::InputTag>("InputLabel", edm::InputTag("rawDataCollector"));
    std::vector<int> feds(54);
    for (uint32_t i = 0; i < 54; ++i)
      feds[i] = i + 601;
    desc.add<std::vector<int>>("FEDs", feds);
    desc.add<uint32_t>("maxChannelsEB", 61200);
    desc.add<uint32_t>("maxChannelsEE", 14648);
    desc.add<bool>("headerUnpacking", true);
    desc.add<bool>("tccUnpacking", true);
    desc.add<bool>("memUnpacking", true);
    desc.ifValue(edm::ParameterDescription<bool>("feUnpacking", true, true),
                 true >> (edm::ParameterDescription<std::string>("digisLabelEB", "ebDigis", true) and
                          edm::ParameterDescription<std::string>("digisLabelEE", "eeDigis", true)) or
                     false >> edm::EmptyGroupDescription());
    desc.ifValue(edm::ParameterDescription<bool>("srpUnpacking", true, true),
                 true >> (edm::ParameterDescription<std::string>("srFlagsLabelEB", "ebSrFlags", true) and
                          edm::ParameterDescription<std::string>("srFlagsLabelEE", "eeSrFlags", true)) or
                     false >> edm::EmptyGroupDescription());

    confDesc.addWithDefaultLabel(desc);
  }

  EcalRawToDigiPortable::EcalRawToDigiPortable(const edm::ParameterSet& ps)
      : unpackHeader_{ps.getParameter<bool>("headerUnpacking")},
        unpackSrFlags_{ps.getParameter<bool>("srpUnpacking")},
        unpackTcc_{ps.getParameter<bool>("tccUnpacking")},
        unpackFe_{ps.getParameter<bool>("feUnpacking")},
        unpackMem_{ps.getParameter<bool>("memUnpacking")},
        rawDataToken_{consumes<FEDRawDataCollection>(ps.getParameter<edm::InputTag>("InputLabel"))},
        eMappingToken_{esConsumes()},
        fedsToUnpack_{ps.getParameter<std::vector<int>>("FEDs")} {
    config_.maxChannelsEB = ps.getParameter<uint32_t>("maxChannelsEB");
    config_.maxChannelsEE = ps.getParameter<uint32_t>("maxChannelsEE");

    if (unpackHeader_) {
      ecalRawDataDevToken_ = produces();
    }
    if (unpackSrFlags_) {
      srFlagsDevEBToken_ = produces(ps.getParameter<std::string>("srFlagsLabelEB"));
      srFlagsDevEEToken_ = produces(ps.getParameter<std::string>("srFlagsLabelEE"));
    }
    if (unpackTcc_) {
      ecalTriggerPrimitivesDevToken_ = produces();
      ecalPseudoStripInputsDevToken_ = produces();
    }
    if (unpackFe_) {
      digisDevEBToken_ = produces(ps.getParameter<std::string>("digisLabelEB"));
      digisDevEEToken_ = produces(ps.getParameter<std::string>("digisLabelEE"));
      integrityGainErrorsDevEBToken_ = produces("integrityGainErrorsEB");
      integrityGainSwitchErrorsDevEBToken_ = produces("integrityGainSwitchErrorsEB");
      integrityChIdErrorsDevEBToken_ = produces("integrityChIdErrorsEB");
      integrityGainErrorsDevEEToken_ = produces("integrityGainErrorsEE");
      integrityGainSwitchErrorsDevEEToken_ = produces("integrityGainSwitchErrorsEE");
      integrityChIdErrorsDevEEToken_ = produces("integrityChIdErrorsEE");
      integrityTTIdErrorsDevToken_ = produces("integrityTTIdErrors");
      integrityZSXtalIdErrorsDevToken_ = produces("integrityZSXtalIdErrors");
      integrityBlockSizeErrorsDevToken_ = produces("integrityBlockSizeErrors");
      pnDiodeDigisDevToken_ = produces();
    }
    if (unpackMem_) {
      integrityMemTtIdErrorsDevToken_ = produces("integrityMemTtIdErrors");
      integrityMemBlockSizeErrorsDevToken_ = produces("integrityMemBlockSizeErrors");
      integrityMemChIdErrorsDevToken_ = produces("integrityMemChIdErrors");
      integrityMemGainErrorsDevToken_ = produces("integrityMemGainErrors");
    }
  }

  void EcalRawToDigiPortable::produce(device::Event& event, device::EventSetup const& setup) {
    // conditions
    auto const& eMappingProduct = setup.getData(eMappingToken_);

    // event data
    const auto rawDataHandle = event.getHandle(rawDataToken_);

    // make a first iteration over the FEDs to compute the total buffer size
    uint32_t size = 0;
    uint32_t feds = 0;
    for (auto const& fed : fedsToUnpack_) {
      auto const& data = rawDataHandle->FEDData(fed);
      auto const nbytes = data.size();

      // skip empty FEDs
      if (nbytes < globalFieds::EMPTYEVENTSIZE)
        continue;

      size += nbytes;
      ++feds;
    }

    auto& queue = event.queue();

    // input host buffers
    ecal::raw::InputDataHost inputHost(queue, size, feds);

    // output device collections
    EcalDigiDeviceCollection digisDevEB{static_cast<int32_t>(config_.maxChannelsEB), queue};
    EcalDigiDeviceCollection digisDevEE{static_cast<int32_t>(config_.maxChannelsEE), queue};

    EcalIdDeviceCollection integrityGainErrorsDevEB{static_cast<int32_t>(config_.maxChannelsEB), queue};
    EcalIdDeviceCollection integrityGainSwitchErrorsDevEB{static_cast<int32_t>(config_.maxChannelsEB), queue};
    EcalIdDeviceCollection integrityChIdErrorsDevEB{static_cast<int32_t>(config_.maxChannelsEB), queue};
    EcalIdDeviceCollection integrityGainErrorsDevEE{static_cast<int32_t>(config_.maxChannelsEE), queue};
    EcalIdDeviceCollection integrityGainSwitchErrorsDevEE{static_cast<int32_t>(config_.maxChannelsEE), queue};
    EcalIdDeviceCollection integrityChIdErrorsDevEE{static_cast<int32_t>(config_.maxChannelsEE), queue};
    EcalIdDeviceCollection integrityTTIdErrorsDev{EcalTrigTowerDetId::kEBTotalTowers + 2 * EcalScDetId::SC_PER_EE_CNT,
                                                  queue};  // FIXME right size
    EcalIdDeviceCollection integrityZSXtalIdErrorsDev{
        static_cast<int32_t>(config_.maxChannelsEB + config_.maxChannelsEE), queue};  // FIXME right size
    EcalIdDeviceCollection integrityBlockSizeErrorsDev{
        static_cast<int32_t>(config_.maxChannelsEB + config_.maxChannelsEE), queue};  // FIXME right size
    EcalPnDiodeDigiDeviceCollection pnDiodeDigisDev{72, queue};                       // PN diodes per SM

    // TODO check if number of SR flags is fixed in raw data format
    EcalSrFlagDeviceCollection srFlagsDevEB{static_cast<int32_t>(config_.maxChannelsEB), queue};  // FIXME right size
    EcalSrFlagDeviceCollection srFlagsDevEE{static_cast<int32_t>(config_.maxChannelsEE), queue};  // FIXME right size

    // DCC headers collection
    EcalDCCHeaderBlockDeviceCollection dccHeadersDev{static_cast<int32_t>(feds), queue};

    // TCC collections
    EcalTriggerPrimitiveDigiDeviceCollection ecalTriggerPrimitivesDev{
        EcalTrigTowerDetId::kEBTotalTowers + 2 * EcalScDetId::SC_PER_EE_CNT, queue};  // FIXME right size
    EcalPseudoStripInputDigiDeviceCollection ecalPseudoStripInputsDev{
        EcalTrigTowerDetId::kEBTotalTowers + 2 * EcalScDetId::SC_PER_EE_CNT, queue};  // FIXME right size

    // MEM collections
    EcalIdDeviceCollection integrityMemTtIdErrorsDev{72, queue};       // FIXME right size
    EcalIdDeviceCollection integrityMemBlockSizeErrorsDev{72, queue};  // FIXME right size
    EcalIdDeviceCollection integrityMemChIdErrorsDev{72, queue};       // FIXME right size
    EcalIdDeviceCollection integrityMemGainErrorsDev{72, queue};       // FIXME right size

    // reset the size scalars of the SoAs
    // memset takes an alpaka view that is created from the scalar in a view to the device collection
    auto digiViewEB = make_device_view<uint32_t>(alpaka::getDev(queue), digisDevEB.view().size());
    auto digiViewEE = make_device_view<uint32_t>(alpaka::getDev(queue), digisDevEE.view().size());
    alpaka::memset(queue, digiViewEB, 0);
    alpaka::memset(queue, digiViewEE, 0);

    auto integrityGainErrorsViewEB =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityGainErrorsDevEB.view().size());
    auto integrityGainSwitchErrorsViewEB =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityGainSwitchErrorsDevEB.view().size());
    auto integrityChIdErrorsViewEB =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityChIdErrorsDevEB.view().size());
    auto integrityGainErrorsViewEE =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityGainErrorsDevEE.view().size());
    auto integrityGainSwitchErrorsViewEE =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityGainSwitchErrorsDevEE.view().size());
    auto integrityChIdErrorsViewEE =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityChIdErrorsDevEE.view().size());
    alpaka::memset(queue, integrityGainErrorsViewEB, 0);
    alpaka::memset(queue, integrityGainSwitchErrorsViewEB, 0);
    alpaka::memset(queue, integrityChIdErrorsViewEB, 0);
    alpaka::memset(queue, integrityGainErrorsViewEE, 0);
    alpaka::memset(queue, integrityGainSwitchErrorsViewEE, 0);
    alpaka::memset(queue, integrityChIdErrorsViewEE, 0);

    auto pnDiodeDigisView = make_device_view<uint32_t>(alpaka::getDev(queue), pnDiodeDigisDev.view().size());
    alpaka::memset(queue, pnDiodeDigisView, 0);

    auto ecalPseudoStripInputsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), ecalPseudoStripInputsDev.view().size());
    auto ecalTriggerPrimitivesView =
        make_device_view<uint32_t>(alpaka::getDev(queue), ecalTriggerPrimitivesDev.view().size());
    alpaka::memset(queue, ecalPseudoStripInputsView, 0);
    alpaka::memset(queue, ecalTriggerPrimitivesView, 0);

    auto srFlagsViewEB = make_device_view<uint32_t>(alpaka::getDev(queue), srFlagsDevEB.view().size());
    auto srFlagsViewEE = make_device_view<uint32_t>(alpaka::getDev(queue), srFlagsDevEE.view().size());
    alpaka::memset(queue, srFlagsViewEB, 0);
    alpaka::memset(queue, srFlagsViewEE, 0);

    auto integrityTTIdErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityTTIdErrorsDev.view().size());
    auto integrityZSXtalIdErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityZSXtalIdErrorsDev.view().size());
    auto integrityBlockSizeErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityBlockSizeErrorsDev.view().size());
    alpaka::memset(queue, integrityTTIdErrorsView, 0);
    alpaka::memset(queue, integrityZSXtalIdErrorsView, 0);
    alpaka::memset(queue, integrityBlockSizeErrorsView, 0);

    auto integrityMemTtIdErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityMemTtIdErrorsDev.view().size());
    auto integrityMemBlockSizeErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityMemBlockSizeErrorsDev.view().size());
    auto integrityMemChIdErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityMemChIdErrorsDev.view().size());
    auto integrityMemGainErrorsView =
        make_device_view<uint32_t>(alpaka::getDev(queue), integrityMemGainErrorsDev.view().size());
    alpaka::memset(queue, integrityMemTtIdErrorsView, 0);
    alpaka::memset(queue, integrityMemBlockSizeErrorsView, 0);
    alpaka::memset(queue, integrityMemChIdErrorsView, 0);
    alpaka::memset(queue, integrityMemGainErrorsView, 0);

    auto dccHeadersView = make_device_view<uint32_t>(alpaka::getDev(queue), dccHeadersDev.view().size());
    alpaka::memset(queue, dccHeadersView, 0);

    // iterate over FEDs to fill the host buffer
    uint32_t currentCummOffset = 0;
    uint32_t fedCounter = 0;
    for (auto const& fed : fedsToUnpack_) {
      auto const& data = rawDataHandle->FEDData(fed);
      auto const nbytes = data.size();

      // skip empty FEDs
      if (nbytes < globalFieds::EMPTYEVENTSIZE)
        continue;

      // copy raw data into host buffer
      std::memcpy(inputHost.data.data() + currentCummOffset, data.data(), nbytes);
      // set the offset in bytes from the start
      inputHost.offsets[fedCounter] = currentCummOffset;
      inputHost.feds[fedCounter] = fed;

      // this is the current offset into the buffer
      currentCummOffset += nbytes;
      ++fedCounter;
    }
    assert(currentCummOffset == size);
    assert(fedCounter == feds);

    // unpack if at least one FED has data
    if (fedCounter > 0) {
      // input device buffers
      ecal::raw::InputDataDevice inputDevice(queue, currentCummOffset, fedCounter);

      // transfer the raw data
      alpaka::memcpy(queue, inputDevice.data, inputHost.data);
      alpaka::memcpy(queue, inputDevice.offsets, inputHost.offsets);
      alpaka::memcpy(queue, inputDevice.feds, inputHost.feds);

      if (unpackFe_) {
        ecal::raw::unpackFe(queue,
                            inputDevice,
                            digisDevEB,
                            digisDevEE,
                            integrityGainErrorsDevEB,
                            integrityGainErrorsDevEE,
                            integrityGainSwitchErrorsDevEB,
                            integrityGainSwitchErrorsDevEE,
                            integrityChIdErrorsDevEB,
                            integrityChIdErrorsDevEE,
                            integrityTTIdErrorsDev,
                            integrityZSXtalIdErrorsDev,
                            integrityBlockSizeErrorsDev,
                            pnDiodeDigisDev,
                            eMappingProduct,
                            fedCounter,
                            currentCummOffset);
      }
      if (unpackSrFlags_) {
        ecal::raw::unpackSrFlags(queue, inputDevice, srFlagsDevEB, srFlagsDevEE, fedCounter);
      }
      if (unpackHeader_) {
        ecal::raw::unpackHeaders(queue, inputDevice, dccHeadersDev, fedCounter);
      }
      if (unpackTcc_) {
        ecal::raw::unpackTcc(queue, inputDevice, ecalTriggerPrimitivesDev, ecalPseudoStripInputsDev, fedCounter);
      }
      if (unpackMem_) {
        ecal::raw::unpackMem(queue,
                             inputDevice,
                             integrityMemTtIdErrorsDev,
                             integrityMemBlockSizeErrorsDev,
                             integrityMemChIdErrorsDev,
                             integrityMemGainErrorsDev,
                             fedCounter);
      }
    }

    // put collections in the event
    if (unpackFe_) {
      event.emplace(digisDevEBToken_, std::move(digisDevEB));
      event.emplace(digisDevEEToken_, std::move(digisDevEE));
      event.emplace(integrityGainErrorsDevEBToken_, std::move(integrityGainErrorsDevEB));
      event.emplace(integrityGainSwitchErrorsDevEBToken_, std::move(integrityGainSwitchErrorsDevEB));
      event.emplace(integrityChIdErrorsDevEBToken_, std::move(integrityChIdErrorsDevEB));
      event.emplace(integrityGainErrorsDevEEToken_, std::move(integrityGainErrorsDevEE));
      event.emplace(integrityGainSwitchErrorsDevEEToken_, std::move(integrityGainSwitchErrorsDevEE));
      event.emplace(integrityChIdErrorsDevEEToken_, std::move(integrityChIdErrorsDevEE));
      event.emplace(integrityTTIdErrorsDevToken_, std::move(integrityTTIdErrorsDev));
      event.emplace(integrityZSXtalIdErrorsDevToken_, std::move(integrityZSXtalIdErrorsDev));
      event.emplace(integrityBlockSizeErrorsDevToken_, std::move(integrityBlockSizeErrorsDev));
      event.emplace(pnDiodeDigisDevToken_, std::move(pnDiodeDigisDev));
    }
    if (unpackSrFlags_) {
      event.emplace(srFlagsDevEBToken_, std::move(srFlagsDevEB));
      event.emplace(srFlagsDevEEToken_, std::move(srFlagsDevEE));
    }
    if (unpackHeader_) {
      event.emplace(ecalRawDataDevToken_, std::move(dccHeadersDev));
    }
    if (unpackTcc_) {
      event.emplace(ecalTriggerPrimitivesDevToken_, std::move(ecalTriggerPrimitivesDev));
      event.emplace(ecalPseudoStripInputsDevToken_, std::move(ecalPseudoStripInputsDev));
    }
    if (unpackMem_) {
      event.emplace(integrityMemTtIdErrorsDevToken_, std::move(integrityMemTtIdErrorsDev));
      event.emplace(integrityMemBlockSizeErrorsDevToken_, std::move(integrityMemBlockSizeErrorsDev));
      event.emplace(integrityMemChIdErrorsDevToken_, std::move(integrityMemChIdErrorsDev));
      event.emplace(integrityMemGainErrorsDevToken_, std::move(integrityMemGainErrorsDev));
    }
  }

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#include "HeterogeneousCore/AlpakaCore/interface/alpaka/MakerMacros.h"
DEFINE_FWK_ALPAKA_MODULE(EcalRawToDigiPortable);
