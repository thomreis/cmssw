#ifndef EventFilter_EcalRawToDigi_plugins_alpaka_UnpackPortable_h
#define EventFilter_EcalRawToDigi_plugins_alpaka_UnpackPortable_h

#include "CondFormats/EcalObjects/interface/alpaka/EcalElectronicsMappingDevice.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalDigiDeviceCollection.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/traits.h"
#include "DeclsForKernels.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::raw {

  void unpackRaw(Queue&,
                 InputDataHost const&,
                 InputDataDevice&,
                 EcalDigiDeviceCollection&,
                 EcalDigiDeviceCollection&,
                 EcalElectronicsMappingDevice const&,
                 uint32_t const,
                 uint32_t const);

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::raw

#endif  // EventFilter_EcalRawToDigi_plugins_alpaka_UnpackPortable_h
