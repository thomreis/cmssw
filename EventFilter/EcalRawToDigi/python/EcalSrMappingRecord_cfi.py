import FWCore.ParameterSet.Config as cms

esrmapping = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalSrMappingRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

