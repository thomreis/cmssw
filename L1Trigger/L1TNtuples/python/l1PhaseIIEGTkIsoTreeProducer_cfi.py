import FWCore.ParameterSet.Config as cms

# tree with EG Tk Iso variables
l1PhaseIIEGTkIsoTree = cms.EDAnalyzer("L1PhaseIIEGTkIsoTreeProducer",
    l1EgBarrel = cms.InputTag("L1EGammaClusterEmuProducer"),
    l1EgHGC = cms.InputTag("l1ctLayer1EG","L1EgEE"),
    l1Tracks = cms.InputTag("TTTracksFromTrackletEmulation","Level1TTTracks"),
    #l1PFCandidates = cms.InputTag("l1pfCandidates","PF"),
    l1PFCandidates = cms.InputTag("l1ctLayer2Deregionizer","Puppi"),
    storeAllPFCands = cms.bool(True),
    trackerGeometry = cms.string("idealForDigi"), # tracker geometry record to get for calculating pT from 2 stubs (unused if useTwoStubsPt is False)
    egBarrelMinEt = cms.double(2.), # minimum Et for barrel EG object
    egHGCMinEt = cms.double(2.), # minimum Et for HGC EG object
    #trackMinPt = cms.double(2.), # minimum Pt to select tracks for matching
    trackMaxChi2 = cms.double(1e10), # maximum Chi2 to select tracks for matching
    trackMinPt = cms.double(10.), # minimum Pt to select tracks for matching
    #trackMaxChi2 = cms.double(100), # maximum Chi2 to select tracks for matching
    useTwoStubsPt = cms.bool(False), # calculate track pT from 2 stubs
    #trackEGammaMatchType = cms.string("PtDependentCut"),
    trackEGammaMatchType = cms.string("EllipticalCut"),
    trackEGammaDeltaPhi = cms.vdouble(0.07, 0., 0.), # functional Delta Phi cut parameters to match Track with L1EG objects
    trackEGammaDeltaR = cms.vdouble(0.08, 0., 0.), # functional Delta R cut parameters to match Track with L1EG objects
    #trackEGammaDeltaEta = cms.vdouble(1e10, 0., 0.), # Delta Eta cutoff to match Track with L1EG objects
    trackEGammaDeltaEta = cms.vdouble(0.015, 0.025, 0.0075), # Delta Eta cutoff to match Track with L1EG objects (elliptical cut values (EB high, EB low, HGC))
    trackMinPtForIsoEB = cms.double(2.), # minimum Pt to select tracks for the isolation in EB
    trackMinPtForIsoHGC = cms.double(2.), # minimum Pt to select tracks for the isolation in HGCal
    trackMaxChi2ForIsoEB = cms.double(1e10), # maximum Chi2 to select tracks for the isolation in EB
    trackMaxChi2ForIsoHGC = cms.double(100), # maximum Chi2 to select tracks for the isolation in HGCal
    trackMinNStubsForIsoEB = cms.uint32(0), # minimum number of stubs to select tracks for the isolation in EB
    trackMinNStubsForIsoHGC = cms.uint32(4), # minimum number of stubs to select tracks for the isolation in HGCal
    dRMinForIso = cms.double(0.), # minimum dR to select tracks for the isolation
    dRMaxForIso = cms.double(0.8) # maximum dR to select tracks for the isolation
)

