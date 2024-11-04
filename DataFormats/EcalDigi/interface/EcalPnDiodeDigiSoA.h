#ifndef DataFormats_EcalDigi_EcalPnDiodeDigiSoA_h
#define DataFormats_EcalDigi_EcalPnDiodeDigiSoA_h

#include "DataFormats/Common/interface/StdArray.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigi.h"
#include "DataFormats/SoATemplate/interface/SoALayout.h"

// due to a ROOT limitation the std::array needs to be wrapped
// https://github.com/root-project/root/issues/12007
using EcalFEMSampleArray = edm::StdArray<uint16_t, EcalPnDiodeDigi::MAXSAMPLES>;

GENERATE_SOA_LAYOUT(EcalPnDiodeDigiSoALayout,
                    SOA_COLUMN(uint32_t, id),
                    SOA_COLUMN(EcalFEMSampleArray, data),
                    SOA_SCALAR(uint32_t, size))

using EcalPnDiodeDigiSoA = EcalPnDiodeDigiSoALayout<>;

#endif
