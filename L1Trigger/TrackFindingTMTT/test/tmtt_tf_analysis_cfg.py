################################################################################################
# To run execute do
# cmsRun tmtt_tf_analysis_cfg.py Events=50 inputMC=Samples/Muons/PU0.txt histFile=outputHistFile.root 
# where the arguments take default values if you don't specify them. You can change defaults below.
#################################################################################################

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
import os

process = cms.Process("Demo")

GEOMETRY = "D21"

if GEOMETRY == "D17":
  process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')
  process.load('Configuration.Geometry.GeometryExtended2023D17_cff')
elif GEOMETRY == "D21":
  process.load('Configuration.Geometry.GeometryExtended2023D21Reco_cff')
  process.load('Configuration.Geometry.GeometryExtended2023D21_cff')
elif GEOMETRY == "D41":
  process.load('Configuration.Geometry.GeometryExtended2023D41Reco_cff')
  process.load('Configuration.Geometry.GeometryExtended2023D41_cff')
elif GEOMETRY == "D49":
  process.load('Configuration.Geometry.GeometryExtended2023D49Reco_cff')
  process.load('Configuration.Geometry.GeometryExtended2023D49_cff')

process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.load("FWCore.MessageLogger.MessageLogger_cfi")

options = VarParsing.VarParsing ('analysis')

def getTxtFile(txtFileName): 
  return os.environ['CMSSW_BASE']+'/src/L1Trigger/TrackFindingTMTT/test/'+txtFileName

#--- Specify input MC
if GEOMETRY == "D17":
  inputMCtxt = getTxtFile('MCsamples/937/RelVal/TTbar/PU200.txt')
elif GEOMETRY == "D21":
  inputMCtxt = getTxtFile('MCsamples/1040/RelVal/TTbar/PU200.txt')
elif GEOMETRY == "D41":
  inputMCtxt = getTxtFile('MCsamples/1060/RelVal/TTbar/PU200.txt')
elif GEOMETRY == "D49":
  inputMCtxt = getTxtFile('MCsamples/1110/RelVal/TTbar/PU200.txt')

# Fastest to use a local copy ...
#inputMCtxt = getTxtFile('MCsamples/1040/RelVal/TTbar/localRAL/PU200.txt') 

options.register('inputMC', inputMCtxt, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "Files to be processed")

#--- Specify number of events to process.
options.register('Events',100,VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,"Number of Events to analyze")

#--- Specify name of output histogram file. (If name = '', then no histogram file will be produced).
options.register('histFile','Hist.root',VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,"Name of output histogram file")

#--- Specify if stubs need to be (re)made, e.g. because they are not available in the input file
options.register('makeStubs',0,VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"Make stubs, and truth association, on the fly")

#--- Specify whether to output a GEN-SIM-DIGI-RAW dataset containing the TMTT L1 tracks & associators.
# (Warning: you may need to edit the associator python below to specify which track fitter you are using).
options.register('outputDataset',0,VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"Create GEN-SIM-DIGI-RAW dataset containing TMTT L1 tracks")

options.parseArguments()

#--- input and output

list = FileUtils.loadListFromFile(options.inputMC)
readFiles = cms.untracked.vstring(*list)
secFiles = cms.untracked.vstring()

# Override input dataset.
#readFiles = cms.untracked.vstring('/store/user/abhijith/DisplacedMuPlus.root')

outputHistFile = options.histFile

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.Events) )

if outputHistFile != "":
  process.TFileService = cms.Service("TFileService", fileName = cms.string(outputHistFile))

process.source = cms.Source ("PoolSource",
                            fileNames = readFiles,
                            secondaryFileNames = secFiles,
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            # Following needed to read CMSSW 9 datasets with CMSSW 10
                            inputCommands = cms.untracked.vstring(
                              'keep *_*_*_*',
                              'drop l1tEMTFHit2016*_*_*_*',
                              'drop l1tEMTFTrack2016*_*_*_*'
                            )
                            )

process.Timing = cms.Service("Timing", summaryOnly = cms.untracked.bool(True))

#--- Load code that produces our L1 tracks and makes corresponding histograms.
#process.load('L1Trigger.TrackFindingTMTT.TMTrackProducer_cff')

#--- Alternative cfg including improvements not yet in the firmware. Aimed at L1 trigger studies.
process.load('L1Trigger.TrackFindingTMTT.TMTrackProducer_Ultimate_cff')
#
#--- Optionally override default configuration parameters here (example given of how).

#process.TMTrackProducer.TrackFitSettings.TrackFitters = cms.vstring(
#                                "KF5ParamsComb",
#                                "KF4ParamsComb",
#                                "KF4ParamsCombHLS"
#                                )

# If the input samples contain stubs and the truth association, then you can just use the following path
process.p = cms.Path(process.TMTrackProducer)

# Optionally (re)make the stubs on the fly
if options.makeStubs == 1:
	process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
	process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')
	process.TTClusterAssociatorFromPixelDigis.digiSimLinks = cms.InputTag("simSiPixelDigis","Tracker")

        LOOSE_STUBS = False

        if (LOOSE_STUBS):
          # S.Viret's loose Dec. 2017 stub windows from commented out part of
          # L1Trigger/TrackTrigger/python/TTStubAlgorithmRegister_cfi.py in CMSSW 9.3.7
          # Optimised for electrons.

          process.TTStubAlgorithm_official_Phase2TrackerDigi_ = cms.ESProducer("TTStubAlgorithm_official_Phase2TrackerDigi_",
          zMatchingPS  = cms.bool(True),
          zMatching2S  = cms.bool(True),
          #Number of tilted rings per side in barrel layers (for tilted geom only)
          NTiltedRings = cms.vdouble( 0., 12., 12., 12., 0., 0., 0.), 
          BarrelCut    = cms.vdouble( 0, 2.0, 3, 4.5, 6, 6.5, 7.0),
          TiltedBarrelCutSet = cms.VPSet(
               cms.PSet( TiltedCut = cms.vdouble( 0 ) ),
               cms.PSet( TiltedCut = cms.vdouble( 0, 3, 3., 2.5, 3., 3., 2.5, 2.5, 2., 1.5, 1.5, 1, 1) ),
               cms.PSet( TiltedCut = cms.vdouble( 0, 4., 4, 4, 4, 4., 4., 4.5, 5, 4., 3.5, 3.5, 3) ),
               cms.PSet( TiltedCut = cms.vdouble( 0, 5, 5, 5, 5, 5, 5, 5.5, 5, 5, 5.5, 5.5, 5.5) ),
              ),
          EndcapCutSet = cms.VPSet(
               cms.PSet( EndcapCut = cms.vdouble( 0 ) ),
               cms.PSet( EndcapCut = cms.vdouble( 0, 1., 2.5, 2.5, 3.5, 5.5, 5.5, 6, 6.5, 6.5, 6.5, 6.5, 6.5, 6.5, 7, 7) ),
               cms.PSet( EndcapCut = cms.vdouble( 0, 0.5, 2.5, 2.5, 3, 5, 6, 6, 6.5, 6.5, 6.5, 6.5, 6.5, 6.5, 7, 7) ),
               cms.PSet( EndcapCut = cms.vdouble( 0, 1, 3., 4.5, 6., 6.5, 6.5, 6.5, 7, 7, 7, 7, 7) ),
               cms.PSet( EndcapCut = cms.vdouble( 0, 1., 2.5, 3.5, 6., 6.5, 6.5, 6.5, 6.5, 7, 7, 7, 7) ),
               cms.PSet( EndcapCut = cms.vdouble( 0, 0.5, 1.5, 3., 4.5, 6.5, 6.5, 7, 7, 7, 7, 7, 7) ),
              )
          )

        else:
          # S.Viret's July 2017 stub windows (tight) from commented out part of
         # L1Trigger/TrackTrigger/python/TTStubAlgorithmRegister_cfi.py in CMSSW 9.3.2

          process.TTStubAlgorithm_official_Phase2TrackerDigi_ = cms.ESProducer("TTStubAlgorithm_official_Phase2TrackerDigi_",
          zMatchingPS  = cms.bool(True),
          zMatching2S  = cms.bool(True),
          #Number of tilted rings per side in barrel layers (for tilted geom only)
          NTiltedRings = cms.vdouble( 0., 12., 12., 12., 0., 0., 0.), 
          BarrelCut = cms.vdouble( 0, 2.0, 2.0, 3.5, 4.5, 5.5, 6.5), #Use 0 as dummy to have direct access using DetId to the correct element 

          TiltedBarrelCutSet = cms.VPSet(
              cms.PSet( TiltedCut = cms.vdouble( 0 ) ),
              cms.PSet( TiltedCut = cms.vdouble( 0, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2., 2., 1.5, 1.5, 1., 1.) ),
              cms.PSet( TiltedCut = cms.vdouble( 0, 3., 3., 3., 3., 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2, 2) ),
              cms.PSet( TiltedCut = cms.vdouble( 0, 4.5, 4.5, 4, 4, 4, 4, 3.5, 3.5, 3.5, 3, 3, 3) ),
              ),
          EndcapCutSet = cms.VPSet(
              cms.PSet( EndcapCut = cms.vdouble( 0 ) ),
              cms.PSet( EndcapCut = cms.vdouble( 0, 1, 1.5, 1.5, 2, 2, 2.5, 3, 3, 3.5, 4, 2.5, 3, 3.5, 4.5, 5.5) ),
              cms.PSet( EndcapCut = cms.vdouble( 0, 1, 1.5, 1.5, 2, 2, 2, 2.5, 3, 3, 3, 2, 3, 4, 5, 5.5) ),
              cms.PSet( EndcapCut = cms.vdouble( 0, 1.5, 1.5, 2, 2, 2.5, 2.5, 2.5, 3.5, 2.5, 5, 5.5, 6) ),
              cms.PSet( EndcapCut = cms.vdouble( 0, 1.0, 1.5, 1.5, 2, 2, 2, 2, 3, 3, 6, 6, 6.5) ),
              cms.PSet( EndcapCut = cms.vdouble( 0, 1.0, 1.5, 1.5, 1.5, 2, 2, 2, 3, 3, 6, 6, 6.5) ),
              )
          ) 

	process.p = cms.Path(process.TrackTriggerClustersStubs * process.TrackTriggerAssociatorClustersStubs * process.TMTrackProducer)

# Optionally create output GEN-SIM-DIGI-RAW dataset containing TMTT L1 tracks & associators.
if options.outputDataset == 1:

  # Associate TMTT L1 tracks to truth particles
  process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')
  process.TTAssociatorTMTT = process.TTTrackAssociatorFromPixelDigis.clone(
# Edit to specify which input L1 track collection to run associator on.
          TTTracks = cms.VInputTag(cms.InputTag("TMTrackProducer", 'TML1TracksKF4ParamsComb'))
#          TTTracks = cms.VInputTag(cms.InputTag("TMTrackProducer", 'TML1TracksglobalLinearRegression'))
  )
  process.pa = cms.Path(process.TTAssociatorTMTT)

  # Write output dataset
  process.load('Configuration.EventContent.EventContent_cff')

  process.writeDataset = cms.OutputModule("PoolOutputModule",
      splitLevel = cms.untracked.int32(0),
      eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
      outputCommands = process.RAWSIMEventContent.outputCommands,
      fileName = cms.untracked.string('output_dataset.root'), ## ADAPT IT ##
      dataset = cms.untracked.PSet(
          filterName = cms.untracked.string(''),
          dataTier = cms.untracked.string('GEN-SIM')
      )
  )
  # Include TMTT L1 tracks & associators + stubs.
  process.writeDataset.outputCommands.append('keep  *TTTrack*_*_*_*')
  process.writeDataset.outputCommands.append('keep  *TTStub*_*_*_*')

  process.pd = cms.EndPath(process.writeDataset)
  process.schedule = cms.Schedule(process.p, process.pa, process.pd)
