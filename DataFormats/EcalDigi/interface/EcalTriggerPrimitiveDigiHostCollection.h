#ifndef DataFormats_EcalDigi_EcalTriggerPrimitiveDigiHostCollection_h
#define DataFormats_EcalDigi_EcalTriggerPrimitiveDigiHostCollection_h

#include "DataFormats/Portable/interface/PortableHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigiSoA.h"

// EcalTriggerPrimitiveDigiSoA in host memory
using EcalTriggerPrimitiveDigiHostCollection = PortableHostCollection<EcalTriggerPrimitiveDigiSoA>;

#endif
