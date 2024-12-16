#ifndef DataFormats_EcalDetId_EcalIdSoA_h
#define DataFormats_EcalDetId_EcalIdSoA_h

#include "DataFormats/SoATemplate/interface/SoALayout.h"

GENERATE_SOA_LAYOUT(EcalIdSoALayout, SOA_COLUMN(uint32_t, id), SOA_SCALAR(uint32_t, size))

using EcalIdSoA = EcalIdSoALayout<>;

#endif
