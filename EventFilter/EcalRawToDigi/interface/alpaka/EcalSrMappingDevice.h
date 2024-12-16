#ifndef EventFilter_EcalRawToDigi_interface_alpaka_EcalSrMappingDevice_h
#define EventFilter_EcalRawToDigi_interface_alpaka_EcalSrMappingDevice_h

#include "DataFormats/Portable/interface/alpaka/PortableObject.h"
#include "EventFilter/EcalRawToDigi/interface/EcalSrMappingHost.h"
#include "EventFilter/EcalRawToDigi/interface/EcalSrMap.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  using EcalSrMappingDevice = PortableObject<EcalSrMap>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

ASSERT_DEVICE_MATCHES_HOST_COLLECTION(EcalSrMappingDevice, EcalSrMappingHost);

#endif
