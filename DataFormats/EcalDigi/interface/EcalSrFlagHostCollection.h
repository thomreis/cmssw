#ifndef DataFormats_EcalDigi_EcalSrFlagHostCollection_h
#define DataFormats_EcalDigi_EcalSrFlagHostCollection_h

#include "DataFormats/Portable/interface/PortableHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalSrFlagSoA.h"

// EcalSrFlagSoA in host memory
using EcalSrFlagHostCollection = PortableHostCollection<EcalSrFlagSoA>;

#endif
