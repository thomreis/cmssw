#ifndef DataFormats_EcalRawData_EcalDCCHeaderBlockSoA_h
#define DataFormats_EcalRawData_EcalDCCHeaderBlockSoA_h

#include "DataFormats/Common/interface/StdArray.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlock.h"
#include "DataFormats/SoATemplate/interface/SoALayout.h"

// due to a ROOT limitation the std::array needs to be wrapped
// https://github.com/root-project/root/issues/12007
using EcalTCCStatusArray = edm::StdArray<uint16_t, MAX_TCC_SIZE>;
using EcalTriggerTowerFlagArray = edm::StdArray<uint16_t, MAX_TT_SIZE>;
using EcalFEStatusArray = edm::StdArray<uint16_t, MAX_TT_SIZE>;
using EcalFEBxArray = edm::StdArray<uint16_t, MAX_TT_SIZE>;
using EcalTCCBxArray = edm::StdArray<uint16_t, MAX_TCC_SIZE>;
using EcalFELv1Array = edm::StdArray<uint16_t, MAX_TT_SIZE>;
using EcalTCCLv1Array = edm::StdArray<uint16_t, MAX_TCC_SIZE>;

GENERATE_SOA_LAYOUT(EcalDCCHeaderBlockSoALayout,
                    SOA_COLUMN(int32_t, dccId),
                    SOA_COLUMN(int32_t, fedId),
                    SOA_COLUMN(int32_t, dccErrors),
                    SOA_COLUMN(int32_t, dccInTTCCommand),
                    SOA_COLUMN(int32_t, orbitNumber),
                    SOA_COLUMN(int16_t, runType),
                    SOA_COLUMN(int16_t, zs),

                    SOA_COLUMN(int16_t, basicTriggerType),

                    SOA_COLUMN(int32_t, LV1event),
                    SOA_COLUMN(int32_t, runNumber),
                    SOA_COLUMN(int32_t, BX),
                    SOA_COLUMN(EcalDCCHeaderBlock::EcalDCCEventSettings, eventSettings),

                    SOA_COLUMN(int16_t, rtHalf),
                    SOA_COLUMN(int16_t, mgpaGain),
                    SOA_COLUMN(int16_t, memGain),
                    SOA_COLUMN(bool, selectiveReadout),
                    SOA_COLUMN(bool, testZeroSuppression),
                    SOA_COLUMN(bool, zeroSuppression),

                    SOA_COLUMN(int16_t, srpStatus),
                    SOA_COLUMN(EcalTCCStatusArray, tccStatus),
                    SOA_SCALAR(uint32_t, tccStatusSize),
                    SOA_COLUMN(EcalTriggerTowerFlagArray, triggerTowerFlag),
                    SOA_SCALAR(uint32_t, triggerTowerFlagSize),
                    SOA_COLUMN(EcalFEStatusArray, feStatus),
                    SOA_SCALAR(uint32_t, feStatusSize),

                    SOA_COLUMN(EcalFEBxArray, feBx),
                    SOA_SCALAR(uint32_t, feBxSize),
                    SOA_COLUMN(EcalTCCBxArray, tccBx),
                    SOA_SCALAR(uint32_t, tccBxSize),
                    SOA_COLUMN(int16_t, srpBx),

                    SOA_COLUMN(EcalFELv1Array, feLv1),
                    SOA_SCALAR(uint32_t, feLv1Size),
                    SOA_COLUMN(EcalTCCLv1Array, tccLv1),
                    SOA_SCALAR(uint32_t, tccLv1Size),
                    SOA_COLUMN(int16_t, srpLv1),
                    SOA_SCALAR(uint32_t, size))

using EcalDCCHeaderBlockSoA = EcalDCCHeaderBlockSoALayout<>;

#endif
