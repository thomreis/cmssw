#ifndef DataFormats_EcalDetId_interface_alpaka_EcalIdDeviceCollection_h
#define DataFormats_EcalDetId_interface_alpaka_EcalIdDeviceCollection_h

#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "DataFormats/EcalDetId/interface/EcalIdSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  // EcalIdSoA in device global memory
  using EcalIdDeviceCollection = PortableCollection<EcalIdSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif
