#ifndef DataFormats_EcalDigi_EcalPseudoStripInputDigiHostCollection_h
#define DataFormats_EcalDigi_EcalPseudoStripInputDigiHostCollection_h

#include "DataFormats/Portable/interface/PortableHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalPseudoStripInputDigiSoA.h"

// EcalPseudoStripInputDigiSoA in host memory
using EcalPseudoStripInputDigiHostCollection = PortableHostCollection<EcalPseudoStripInputDigiSoA>;

#endif
