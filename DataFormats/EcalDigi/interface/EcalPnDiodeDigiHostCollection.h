#ifndef DataFormats_EcalDigi_EcalPnDiodeDigiHostCollection_h
#define DataFormats_EcalDigi_EcalPnDiodeDigiHostCollection_h

#include "DataFormats/Portable/interface/PortableHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigiSoA.h"

// EcalPnDiodeDigiSoA in host memory
using EcalPnDiodeDigiHostCollection = PortableHostCollection<EcalPnDiodeDigiSoA>;

#endif
