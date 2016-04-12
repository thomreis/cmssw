import FWCore.ParameterSet.Config as cms

import os

l1tgmt_basedir = "L1Trigger/L1TMuon/"
lut_dir = os.path.join(l1tgmt_basedir, "data/microgmt_luts/")

gmtParamsSource = cms.ESSource(
    "EmptyESSource",
    recordName = cms.string('L1TMuonGlobalParamsRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

gmtParams = cms.ESProducer('L1TMuonGlobalParamsESProducer',
    fwVersion = cms.uint32(1),

    bxMin = cms.int32(-2),
    bxMax = cms.int32(2),

    AbsIsoCheckMemLUTPath        = cms.string(os.path.join(lut_dir, 'AbsIsoCheckMem.txt')),
    RelIsoCheckMemLUTPath        = cms.string(os.path.join(lut_dir, 'RelIsoCheckMem.txt')),
    IdxSelMemPhiLUTPath          = cms.string(os.path.join(lut_dir, 'IdxSelMemPhi.txt')),
    IdxSelMemEtaLUTPath          = cms.string(os.path.join(lut_dir, 'IdxSelMemEta.txt')),
    #BrlSingleMatchQualLUTPath    = cms.string(''),
    FwdPosSingleMatchQualLUTPath = cms.string(''),
    FwdNegSingleMatchQualLUTPath = cms.string(''),
    OvlPosSingleMatchQualLUTPath = cms.string(''),
    OvlNegSingleMatchQualLUTPath = cms.string(''),
    BOPosMatchQualLUTPath        = cms.string(''),
    BONegMatchQualLUTPath        = cms.string(''),
    FOPosMatchQualLUTPath        = cms.string(''),
    FONegMatchQualLUTPath        = cms.string(''),
    BPhiExtrapolationLUTPath     = cms.string(os.path.join(lut_dir, 'BPhiExtrapolation.txt')),
    OPhiExtrapolationLUTPath     = cms.string(os.path.join(lut_dir, 'OPhiExtrapolation.txt')),
    FPhiExtrapolationLUTPath     = cms.string(os.path.join(lut_dir, 'EPhiExtrapolation.txt')),
    BEtaExtrapolationLUTPath     = cms.string(os.path.join(lut_dir, 'BEtaExtrapolation.txt')),
    OEtaExtrapolationLUTPath     = cms.string(os.path.join(lut_dir, 'OEtaExtrapolation.txt')),
    FEtaExtrapolationLUTPath     = cms.string(os.path.join(lut_dir, 'EEtaExtrapolation.txt')),
    SortRankLUTPath              = cms.string(os.path.join(lut_dir, 'SortRank.txt')),

    #BrlSingleMatchQualLUTMaxDR    = cms.double(0.1),
    FwdPosSingleMatchQualLUTMaxDR = cms.double(0.05),
    FwdPosSingleMatchQualLUTfEta  = cms.double(1),
    FwdPosSingleMatchQualLUTfPhi  = cms.double(1),
    FwdNegSingleMatchQualLUTMaxDR = cms.double(0.05),
    FwdNegSingleMatchQualLUTfEta  = cms.double(1),
    FwdNegSingleMatchQualLUTfPhi  = cms.double(1),
    OvlPosSingleMatchQualLUTMaxDR = cms.double(0.1),
    OvlPosSingleMatchQualLUTfEta  = cms.double(1),
    OvlPosSingleMatchQualLUTfPhi  = cms.double(2),
    OvlNegSingleMatchQualLUTMaxDR = cms.double(0.1),
    OvlNegSingleMatchQualLUTfEta  = cms.double(1),
    OvlNegSingleMatchQualLUTfPhi  = cms.double(2),
    BOPosMatchQualLUTMaxDR        = cms.double(0.1),
    BOPosMatchQualLUTfEtaFine     = cms.double(1),
    BOPosMatchQualLUTfEtaCoarse   = cms.double(1./3),
    BOPosMatchQualLUTfPhi         = cms.double(2),
    BONegMatchQualLUTMaxDR        = cms.double(0.1),
    BONegMatchQualLUTfEtaFine     = cms.double(1),
    BONegMatchQualLUTfEtaCoarse   = cms.double(1./3),
    BONegMatchQualLUTfPhi         = cms.double(2),
    FOPosMatchQualLUTMaxDR        = cms.double(0.15),
    FOPosMatchQualLUTfEta         = cms.double(1),
    FOPosMatchQualLUTfPhi         = cms.double(3),
    FONegMatchQualLUTMaxDR        = cms.double(0.15),
    FONegMatchQualLUTfEta         = cms.double(1),
    FONegMatchQualLUTfPhi         = cms.double(3),

    SortRankLUTPtFactor   = cms.uint32(1), # can be 0 or 1
    SortRankLUTQualFactor = cms.uint32(4), # can be 0 to 34
)

