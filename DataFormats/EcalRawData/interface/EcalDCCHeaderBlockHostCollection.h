#ifndef DataFormats_EcalRawData_EcalDCCHeaderBlockHostCollection_h
#define DataFormats_EcalRawData_EcalDCCHeaderBlockHostCollection_h

#include "DataFormats/Portable/interface/PortableHostCollection.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlockSoA.h"

// EcalDCCHeaderBlockSoA in host memory
using EcalDCCHeaderBlockHostCollection = PortableHostCollection<EcalDCCHeaderBlockSoA>;

#endif
