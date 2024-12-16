#ifndef EventFilter_EcalRawToDigi_plugins_alpaka_UnpackPortable_h
#define EventFilter_EcalRawToDigi_plugins_alpaka_UnpackPortable_h

#include "CondFormats/EcalObjects/interface/alpaka/EcalElectronicsMappingDevice.h"
#include "DataFormats/EcalDetId/interface/alpaka/EcalIdDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalDigiDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalSrFlagDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalPnDiodeDigiDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalPseudoStripInputDigiDeviceCollection.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalTriggerPrimitiveDigiDeviceCollection.h"
#include "DataFormats/EcalRawData/interface/alpaka/EcalDCCHeaderBlockDeviceCollection.h"
#include "EventFilter/EcalRawToDigi/interface/alpaka/EcalSrMappingDevice.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "DeclsForKernels.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::raw {

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
                uint32_t const nbytesTotal);

  void unpackSrFlags(Queue& queue,
                     InputDataDevice const& inputDevice,
                     EcalSrFlagDeviceCollection& srFlagsDevEB,
                     EcalSrFlagDeviceCollection& srFlagsDevEE,
                     EcalSrMappingDevice const& srMapping,
                     uint32_t const nfedsWithData);

  void unpackHeaders(Queue& queue,
                     InputDataDevice const& inputDevice,
                     EcalDCCHeaderBlockDeviceCollection& dccHeaders,
                     uint32_t const nfedsWithData);

  void unpackTcc(Queue& queue,
                 InputDataDevice const& inputDevice,
                 EcalTriggerPrimitiveDigiDeviceCollection& tpDigisDev,
                 EcalPseudoStripInputDigiDeviceCollection& psDigisDev,
                 uint32_t const nfedsWithData);

  void unpackMem(Queue& queue,
                 InputDataDevice const& inputDevice,
                 EcalIdDeviceCollection& integrityMemTtIdErrorsDev,
                 EcalIdDeviceCollection& integrityMemBlockSizeErrorsDev,
                 EcalIdDeviceCollection& integrityMemChIdErrorsDev,
                 EcalIdDeviceCollection& integrityMemGainErrorsDev,
                 uint32_t const nfedsWithData);

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::raw

#endif  // EventFilter_EcalRawToDigi_plugins_alpaka_UnpackPortable_h
