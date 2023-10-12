import FWCore.ParameterSet.Config as cms
from HeterogeneousCore.CUDACore.SwitchProducerCUDA import SwitchProducerCUDA

# ECAL unpacker running on CPU
from EventFilter.EcalRawToDigi.EcalUnpackerData_cfi import ecalEBunpacker as _ecalEBunpacker

ecalDigis = SwitchProducerCUDA(
    cpu = _ecalEBunpacker.clone()
)

ecalDigisTask = cms.Task(
    # ECAL unpacker running on CPU
    ecalDigis
)

# process modifier to run on GPUs
from Configuration.ProcessModifiers.gpu_cff import gpu

from Configuration.StandardSequences.Accelerators_cff import *
from HeterogeneousCore.AlpakaCore.ProcessAcceleratorAlpaka_cfi import ProcessAcceleratorAlpaka

# ECAL conditions used by the portable unpacker
from EventFilter.EcalRawToDigi.ecalElectronicsMappingHostESProducer_cfi import ecalElectronicsMappingHostESProducer

# alpaka ECAL unpacker
from EventFilter.EcalRawToDigi.ecalRawToDigiPortable_cfi import ecalRawToDigiPortable as _ecalRawToDigiPortable
ecalDigisPortable = _ecalRawToDigiPortable.clone()

# extend the SwitchProducer to add a case to copy the ECAL digis from the accelerator to CPU and covert them from SoA to legacy format
from EventFilter.EcalRawToDigi.ecalDigisFromPortableProducer_cfi import ecalDigisFromPortableProducer as _ecalDigisFromPortableProducer

gpu.toModify(ecalDigis,
    # copy the ECAL digis from the accelerator to CPU and covert them from SoA to legacy format
    cuda = _ecalDigisFromPortableProducer.clone(
        digisInLabelEB = 'ecalDigisPortable:ebDigis',
        digisInLabelEE = 'ecalDigisPortable:eeDigis',
        produceDummyIntegrityCollections = True
    )
)

gpu.toReplaceWith(ecalDigisTask, cms.Task(
    # ECAL conditions used by the portable unpacker
    ecalElectronicsMappingHostESProducer,
    # run the portable ECAL unpacker
    ecalDigisPortable,
    # run the ECAL unpacker on CPU, or copy the ECAL digis from GPU to CPU and covert them from SoA to legacy format
    ecalDigis
))
