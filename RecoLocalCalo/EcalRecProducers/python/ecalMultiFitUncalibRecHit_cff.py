import FWCore.ParameterSet.Config as cms
from HeterogeneousCore.CUDACore.SwitchProducerCUDA import SwitchProducerCUDA
from Configuration.ProcessModifiers.gpu_cff import gpu

# ECAL multifit running on CPU
from RecoLocalCalo.EcalRecProducers.ecalMultiFitUncalibRecHit_cfi import ecalMultiFitUncalibRecHit as _ecalMultiFitUncalibRecHit
ecalMultiFitUncalibRecHit = SwitchProducerCUDA(
  cpu = _ecalMultiFitUncalibRecHit.clone()
)

ecalMultiFitUncalibRecHitTask = cms.Task(
  # ECAL multifit running on CPU
  ecalMultiFitUncalibRecHit
)

# ECAL conditions used by the multifit running on the accelerator
from RecoLocalCalo.EcalRecProducers.ecalMultifitConditionsHostESProducer_cfi import ecalMultifitConditionsHostESProducer
from RecoLocalCalo.EcalRecProducers.ecalMultifitParametersHostESProducer_cfi import ecalMultifitParametersHostESProducer

ecalMultifitParametersSource = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalMultifitParametersRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

# ECAL multifit running on the accelerator
from RecoLocalCalo.EcalRecProducers.ecalUncalibRecHitProducerPortable_cfi import ecalUncalibRecHitProducerPortable as _ecalUncalibRecHitProducerPortable
ecalMultiFitUncalibRecHitPortable = _ecalUncalibRecHitProducerPortable.clone(
  digisLabelEB = 'ecalDigisPortable:ebDigis',
  digisLabelEE = 'ecalDigisPortable:eeDigis'
)

# convert the uncalibrated rechits from SoA to legacy format
from RecoLocalCalo.EcalRecProducers.ecalUncalibRecHitConvertPortable2CPUFormat_cfi import ecalUncalibRecHitConvertPortable2CPUFormat as _ecalUncalibRecHitConvertPortable2CPUFormat
gpu.toModify(ecalMultiFitUncalibRecHit, cuda = _ecalUncalibRecHitConvertPortable2CPUFormat.clone())

gpu.toReplaceWith(ecalMultiFitUncalibRecHitTask, cms.Task(
  # ECAL conditions used by the multifit running on the accelerator
  ecalMultifitConditionsHostESProducer,
  ecalMultifitParametersHostESProducer,
  # ECAL multifit running on device
  ecalMultiFitUncalibRecHitPortable,
  # ECAL multifit running on CPU, or convert the uncalibrated rechits from SoA to legacy format
  ecalMultiFitUncalibRecHit,
))
