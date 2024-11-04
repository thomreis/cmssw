#ifndef DataFormats_EcalRawData_interface_alpaka_EcalDCCHeaderBlockDeviceCollection_h
#define DataFormats_EcalRawData_interface_alpaka_EcalDCCHeaderBlockDeviceCollection_h

#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlockSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  // EcalDCCHeaderBlockDeviceSoA in device global memory
  using EcalDCCHeaderBlockDeviceCollection = PortableCollection<EcalDCCHeaderBlockSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif
