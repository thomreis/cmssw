import FWCore.ParameterSet.Config as cms

l1ctLayer2TkEmPuppiIso = cms.EDProducer("L1TTkEmPuppiIsoProducer",
    l1TkEmCollection = cms.InputTag("l1tCorrelatorEG", "L1TkEmEB"),
    #l1PFCandidates = cms.InputTag("l1pfCandidates", "PF"),
    l1PFCandidates = cms.InputTag("l1pfCandidates", "Puppi"),
    #pfIsoType = cms.string("PF"),
    pfIsoType = cms.string("PUPPI"),
    outInstanceName = cms.string("L1TkEm"),
    pfPtMin = cms.double(1.),
    dZMax = cms.double(0.6),
    dRMin = cms.double(0.07),
    dRMax = cms.double(0.3)
)

l1ctLayer2TkElePuppiIso = cms.EDProducer("L1TTkElectronPuppiIsoProducer",
    l1TkEmCollection = cms.InputTag("l1tCorrelatorEG", "L1TkEleEB"),
    #l1PFCandidates = cms.InputTag("l1pfCandidates", "PF"),
    l1PFCandidates = cms.InputTag("l1pfCandidates", "Puppi"),
    #pfIsoType = cms.string("PF"),
    pfIsoType = cms.string("PUPPI"),
    outInstanceName = cms.string("L1TkEle"),
    pfPtMin = cms.double(1.),
    dZMax = cms.double(0.6),
    dRMin = cms.double(0.03),
    dRMax = cms.double(0.2)
)

