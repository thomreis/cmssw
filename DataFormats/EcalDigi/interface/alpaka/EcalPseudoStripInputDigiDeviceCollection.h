#ifndef DataFormats_EcalDigi_interface_alpaka_EcalPseudoStripInputDigiDeviceCollection_h
#define DataFormats_EcalDigi_interface_alpaka_EcalPseudoStripInputDigiDeviceCollection_h

#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "DataFormats/EcalDigi/interface/EcalPseudoStripInputDigiSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  // EcalPseudoStripInputDigiSoA in device global memory
  using EcalPseudoStripInputDigiDeviceCollection = PortableCollection<EcalPseudoStripInputDigiSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif
