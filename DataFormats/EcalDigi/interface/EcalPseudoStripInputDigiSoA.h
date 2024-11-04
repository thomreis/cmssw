#ifndef DataFormats_EcalDigi_EcalPseudoStripInputDigiSoA_h
#define DataFormats_EcalDigi_EcalPseudoStripInputDigiSoA_h

#include "DataFormats/Common/interface/StdArray.h"
#include "DataFormats/EcalDigi/interface/EcalPseudoStripInputDigi.h"
#include "DataFormats/SoATemplate/interface/SoALayout.h"

// due to a ROOT limitation the std::array needs to be wrapped
// https://github.com/root-project/root/issues/12007
using EcalPseudoStripInputSampleArray = edm::StdArray<uint16_t, EcalPseudoStripInputDigi::MAXSAMPLES>;

GENERATE_SOA_LAYOUT(EcalPseudoStripInputDigiSoALayout,
                    SOA_COLUMN(uint32_t, id),
                    SOA_COLUMN(EcalPseudoStripInputSampleArray, data),
                    SOA_SCALAR(uint32_t, size))

using EcalPseudoStripInputDigiSoA = EcalPseudoStripInputDigiSoALayout<>;

#endif
