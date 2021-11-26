import FWCore.ParameterSet.Config as cms

from HeterogeneousCore.CUDACore.SwitchProducerCUDA import SwitchProducerCUDA
from DQM.EcalMonitorTasks.EcalMonitorTask_cfi import ecalMonitorTask as _ecalMonitorTask
ecalMonitorTask = SwitchProducerCUDA(
    cpu = _ecalMonitorTask.clone()
)

# Customization to run the CPU vs GPU comparison task if the job runs on a GPU enabled machine
from Configuration.ProcessModifiers.gpu_validation_cff import gpu_validation
from DQM.EcalMonitorTasks.GpuTask_cfi import ecalGpuTask

gpu_validation.toModify(ecalMonitorTask,
    cuda = _ecalMonitorTask.clone(
        workers = (lambda w: w.append("GpuTask"))(_ecalMonitorTask.workers.copy()),
        workerParameters = dict(GpuTask = ecalGpuTask)))

