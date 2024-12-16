#include <alpaka/alpaka.hpp>

#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/DataRecord/interface/EcalMappingElectronicsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalMappingElectronics.h"
#include "EventFilter/EcalRawToDigi/interface/EcalElectronicsMapper.h"
#include "EventFilter/EcalRawToDigi/interface/ElectronicsIdGPU.h"
#include "EventFilter/EcalRawToDigi/interface/alpaka/EcalSrMappingDevice.h"
#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"
#include "EventFilter/EcalRawToDigi/interface/DCCRawDataDefinitions.h"
#include "EventFilter/EcalRawToDigi/interface/EcalSrMappingRcd.h"
#include "EventFilter/EcalRawToDigi/interface/alpaka/EcalSrMappingDevice.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/ESGetToken.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/ESProducer.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/ModuleFactory.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/host.h"
#include "HeterogeneousCore/AlpakaInterface/interface/memory.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  class EcalSrMappingHostESProducer : public ESProducer {
  public:
    EcalSrMappingHostESProducer(edm::ParameterSet const& iConfig) : ESProducer(iConfig), mapper_(10, 1) {
      auto cc = setWhatProduced(this);
      token_ = cc.consumes();
      std::vector<int> emptyvec;
      mapper_.makeMapFromVectors(emptyvec, emptyvec);
    }

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
      edm::ParameterSetDescription desc;
      descriptions.addWithDefaultLabel(desc);
    }

    std::unique_ptr<EcalSrMappingHost> produce(EcalSrMappingRcd const& iRecord) {
      auto const& mapping = iRecord.get(token_);
      mapper_.setEcalElectronicsMapping(&mapping);

      auto product = std::make_unique<EcalSrMappingHost>(cms::alpakatools::host());
      product->zeroInitialise();

      // fill in ee
      // following EventFilter/EcalRawToDigi/src/EcalElectronicsMapper.cc
      size_t linIdx = 0;
      size_t eeDccIdx = 0;
      for (int smId = 1; smId <= 54; ++smId) {
        if (smId > ::ecal::raw::ElectronicsIdGPU::MAX_DCCID_EEM &&
            smId < ::ecal::raw::ElectronicsIdGPU::MIN_DCCID_EEP)  // only need the endcaps
          continue;

        mapper_.setActiveDCC(smId);
        product->value().srFlagOffsets[eeDccIdx] = linIdx;

        for (unsigned int feChannel = 1; feChannel <= mapper_.getNumChannelsInDcc(smId); ++feChannel) {
          // handle the two jump DCCs where there is a gap in SR information in the raw data
          if ((smId == SECTOR_EEM_CCU_JUMP || smId == SECTOR_EEP_CCU_JUMP) &&
              (MIN_CCUID_JUMP <= feChannel && feChannel <= MAX_CCUID_JUMP))
            continue;

          std::vector<EcalScDetId> scDetIds = mapping.getEcalScDetId(smId, feChannel);
          for (size_t i = 0; i < scDetIds.size(); ++i) {
            product->value().scDetIds[linIdx] = scDetIds[i].rawId();
            product->value().srFlagWordIdx[linIdx] = feChannel - 1;
            ++linIdx;
          }
        }
        ++eeDccIdx;
      }
      return product;
    }

  private:
    EcalElectronicsMapper mapper_;
    edm::ESGetToken<EcalElectronicsMapping, EcalMappingRcd> token_;
  };
}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

DEFINE_FWK_EVENTSETUP_ALPAKA_MODULE(EcalSrMappingHostESProducer);
