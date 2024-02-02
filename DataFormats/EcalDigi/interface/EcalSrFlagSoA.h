#ifndef DataFormats_EcalDigi_EcalSrFlagSoA_h
#define DataFormats_EcalDigi_EcalSrFlagSoA_h

#include "DataFormats/SoATemplate/interface/SoALayout.h"

GENERATE_SOA_LAYOUT(EcalSrFlagSoALayout, SOA_COLUMN(uint32_t, id), SOA_COLUMN(uint8_t, flag), SOA_SCALAR(uint32_t, size))

using EcalSrFlagSoA = EcalSrFlagSoALayout<>;

#endif
