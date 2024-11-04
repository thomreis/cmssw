#ifndef DataFormats_EcalDigi_interface_alpaka_EcalPnDiodeDigiDeviceCollection_h
#define DataFormats_EcalDigi_interface_alpaka_EcalPnDiodeDigiDeviceCollection_h

#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigiSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  // EcalPnDiodeDigiSoA in device global memory
  using EcalPnDiodeDigiDeviceCollection = PortableCollection<EcalPnDiodeDigiSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif
