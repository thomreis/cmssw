#ifndef DataFormats_EcalDigi_interface_alpaka_EcalSrFlagDeviceCollection_h
#define DataFormats_EcalDigi_interface_alpaka_EcalSrFlagDeviceCollection_h

#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "DataFormats/EcalDigi/interface/EcalSrFlagSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  // EcalSrFlagSoA in device global memory
  using EcalSrFlagDeviceCollection = PortableCollection<EcalSrFlagSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif
