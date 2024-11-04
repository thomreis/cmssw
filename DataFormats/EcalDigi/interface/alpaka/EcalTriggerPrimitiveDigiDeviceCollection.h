#ifndef DataFormats_EcalDigi_interface_alpaka_EcalTriggerPrimitiveDigiDeviceCollection_h
#define DataFormats_EcalDigi_interface_alpaka_EcalTriggerPrimitiveDigiDeviceCollection_h

#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigiSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  // EcalTriggerPrimitiveDigioA in device global memory
  using EcalTriggerPrimitiveDigiDeviceCollection = PortableCollection<EcalTriggerPrimitiveDigiSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

#endif
