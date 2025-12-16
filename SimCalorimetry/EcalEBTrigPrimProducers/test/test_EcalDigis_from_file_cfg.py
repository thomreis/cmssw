import FWCore.ParameterSet.Config as cms

process = cms.Process("EBTBToDigis")

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1)
)
process.MessageLogger.cerr.INFO.limit = 1000000

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Legacy ECAL digis
#process.source = cms.Source("EcalTBFileInputToDigiSource",
#    #fileNames = cms.untracked.vstring('file:../../../hls/testbench_data/H4_test_input.txt'),
#    fileNames = cms.untracked.vstring('file:../../../hls/testbench_data/rx_tbspikes0p9_ld2_2800evts.txt'),
#    firstLuminosityBlockForEachRun = cms.untracked.VLuminosityBlockID([]),
#    runNumber = cms.untracked.uint32(1),
#    firstEventNumber = cms.untracked.uint32(1),
#    startSample = cms.untracked.uint32(7),
#    skipEvents = cms.untracked.int32(0),
#    attenuation = cms.untracked.double(4.)
#)

# Phase 2 ECAL digis
process.source = cms.Source("EcalTBFileInputToDigiPh2Source",
    #fileNames = cms.untracked.vstring('file:../../../hls/testbench_data/H4_test_input.txt'),
    fileNames = cms.untracked.vstring('file:../../../hls/testbench_data/rx_tbspikes0p9_ld2_2800evts.txt'),
    firstLuminosityBlockForEachRun = cms.untracked.VLuminosityBlockID([]),
    runNumber = cms.untracked.uint32(1),
    firstEventNumber = cms.untracked.uint32(1),
    startSample = cms.untracked.uint32(4),
    skipEvents = cms.untracked.int32(0),
    attenuation = cms.untracked.double(4.)
)

process.p = cms.Path()

process.Out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string( "EBDigis_PhaseII_filetest.root" ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring("keep *")
)

process.end = cms.EndPath( process.Out )

