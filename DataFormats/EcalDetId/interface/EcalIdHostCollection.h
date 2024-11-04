#ifndef DataFormats_EcalDetId_EcalIdHostCollection_h
#define DataFormats_EcalDetId_EcalIdHostCollection_h

#include "DataFormats/Portable/interface/PortableHostCollection.h"
#include "DataFormats/EcalDetId/interface/EcalIdSoA.h"

// Generic EcalIdSoA in host memory
using EcalIdHostCollection = PortableHostCollection<EcalIdSoA>;

#endif
