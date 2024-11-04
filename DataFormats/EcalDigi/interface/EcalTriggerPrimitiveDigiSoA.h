#ifndef DataFormats_EcalDigi_EcalTriggerPrimitiveDigiSoA_h
#define DataFormats_EcalDigi_EcalTriggerPrimitiveDigiSoA_h

#include "DataFormats/Common/interface/StdArray.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/SoATemplate/interface/SoALayout.h"

// due to a ROOT limitation the std::array needs to be wrapped
// https://github.com/root-project/root/issues/12007
using EcalTriggerPrimitiveSampleArray = edm::StdArray<uint16_t, EcalTriggerPrimitiveDigi::MAXSAMPLES>;

GENERATE_SOA_LAYOUT(EcalTriggerPrimitiveDigiSoALayout,
                    SOA_COLUMN(uint32_t, id),
                    SOA_COLUMN(EcalTriggerPrimitiveSampleArray, data),
                    SOA_SCALAR(uint32_t, size))

using EcalTriggerPrimitiveDigiSoA = EcalTriggerPrimitiveDigiSoALayout<>;

#endif
