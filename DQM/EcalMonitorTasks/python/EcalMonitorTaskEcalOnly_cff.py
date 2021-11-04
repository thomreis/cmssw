import FWCore.ParameterSet.Config as cms

from DQM.EcalMonitorTasks.EcalMonitorTaskEcalOnly_cfi import *

# Customization to run the CPU vs GPU comparison task if the job runs on a GPU enabled machine
from Configuration.ProcessModifiers.gpu_cff import gpu
from DQM.EcalMonitorTasks.GpuTask_cfi import ecalGpuTask

gpu.toModify(ecalMonitorTaskEcalOnly.workers, func = lambda workers: workers.append("GpuTask"))
gpu.toModify(ecalMonitorTaskEcalOnly, workerParameters = dict(GpuTask = ecalGpuTask))
