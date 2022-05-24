import FWCore.ParameterSet.Config as cms

def customisePhase2TTNoMC(process):
    process.L1TrackTrigger.replace(process.L1PromptExtendedHybridTracksWithAssociators, process.L1PromptExtendedHybridTracks)
    process.L1TrackTrigger.remove(process.TrackTriggerAssociatorClustersStubs)
    process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

    return process

def addHcalTriggerPrimitives(process):
    process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

    return process

def addMenuNtuples(process):
    process.load("L1Trigger.Phase2L1ParticleFlow.l1ctLayer2EG_cff")
    process.load("L1Trigger.L1TNtuples.l1PhaseIITreeStep1Producer_cfi")
    process.load("L1Trigger.L1TNtuples.l1EventTree_cfi")
    process.load("L1Trigger.L1TNtuples.l1PhaseIIEGTkIsoTreeProducer_cfi")
    process.runmenutree *= process.l1PhaseIIEGTkIsoTree
    process.runmenutree *= process.l1EventTree
    process.TFileService = cms.Service("TFileService",
        fileName = cms.string('L1NtuplePhaseII_Step1.root')
    )

    return process

