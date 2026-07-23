import FWCore.ParameterSet.Config as cms

#
# attention: default is changed to work on unsuppressed digis!! ##############
#

simEcalEBTriggerPrimitivePhase2Digis = cms.EDProducer("EcalEBTrigPrimPhase2Producer",
    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis"),
    binOfMaximum = cms.int32(6), 
    Debug = cms.bool(False),
    spikeTagger = cms.PSet(
        algoType = cms.string("ld"),
        version = cms.uint32(1)
    )
)


from Configuration.Eras.Modifier_phase2_ecalTP_devel_cff import phase2_ecalTP_devel
phase2_ecalTP_devel.toModify( simEcalEBTriggerPrimitivePhase2Digis)
