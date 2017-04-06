import FWCore.ParameterSet.Config as cms
import os

simRegionalMuonCandSorter = cms.EDProducer('L1TRegionalMuonCandSorter',
    bmtfInput  = cms.InputTag("simBmtfDigis", "BMTF"),
    omtfInput  = cms.InputTag("simOmtfDigis", "OMTF"),
    emtfInput  = cms.InputTag("simEmtfDigis", "EMTF"),
)





