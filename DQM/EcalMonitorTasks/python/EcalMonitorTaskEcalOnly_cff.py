import FWCore.ParameterSet.Config as cms

from HeterogeneousCore.CUDACore.SwitchProducerCUDA import SwitchProducerCUDA
from DQM.EcalMonitorTasks.EcalMonitorTaskEcalOnly_cfi import ecalMonitorTaskEcalOnly as _ecalMonitorTaskEcalOnly
ecalMonitorTaskEcalOnly = SwitchProducerCUDA(
    cpu = _ecalMonitorTaskEcalOnly.clone()
)

# Customization to run the CPU vs GPU comparison task if the job runs on a GPU enabled machine
from Configuration.ProcessModifiers.gpu_validation_cff import gpu_validation
from DQM.EcalMonitorTasks.GpuTask_cfi import ecalGpuTask

gpu_validation.toModify(ecalMonitorTaskEcalOnly,
    cuda = _ecalMonitorTaskEcalOnly.clone(
        workers = (lambda w: w.append("GpuTask"))(_ecalMonitorTaskEcalOnly.workers.copy()),
        workerParameters = dict(GpuTask = ecalGpuTask)))

