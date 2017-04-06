# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1NtupleRECO -s RAW2DIGI --era=Run2_2016 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --conditions=auto:run2_data -n 200 --data --no_exec --filein=/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v4/000/260/627/00000/12455212-1E85-E511-8913-02163E014472.root --geometry=Extended2016,Extended2016Reco --customise=L1Trigger/Configuration/customiseReEmul.L1TEventSetupForHF1x1TPs

RAWDATASOURCE = "rawDataCollector"
#RAWDATASOURCE = "rawDataRepacker"

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing ('analysis')

options.register ('output', 'l1ntuple.root', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The output file name")
options.register ('input',  '',        VarParsing.VarParsing.multiplicity.list,      VarParsing.VarParsing.varType.string,  "The input files")
options.register ('max',    '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The maximum number of events to process")
options.register ('skip',   '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The number of events to skip initially")
options.register ('streamer', False,   VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,    "Use streamer file as input")
options.register ('minidaq', False,    VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,    "MiniDaq run")
options.register ('legacy', False,     VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,    "Legacy trigger triggering")
options.register ('valonly', False,    VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,    "Fill only validation events with large readout window")
options.register ('bottomonly', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,    "Bottom only configuration")

## options.input = '/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/00A76FFA-0C82-E511-B441-02163E01450F.root'
#options.input = '/store/data/Run2015D/ZeroBias1/RAW/v1/000/256/843/00000/FE8AD1BB-D05E-E511-B3A7-02163E01276B.root'
#options.input = '/store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/264/374/00000/D0A082FD-C9D0-E511-87CC-02163E011B69.root'
options.input = '/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/291/115/00000/9CB9A3A6-981A-E711-A3EE-02163E0145B1.root'
options.max  = 100 
options.skip = 0 

options.parseArguments()

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Input source
if (options.streamer) :
    process.source = cms.Source(
        "NewEventStreamFileReader",
        fileNames = cms.untracked.vstring(
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0001_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0002_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0003_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0004_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0005_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0006_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0007_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0008_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0009_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0010_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0011_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0012_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0013_streamExpressCosmics_StorageManager.dat',
            'file:///afs/cern.ch/work/t/treis/public/data/run267357/run267357_ls0014_streamExpressCosmics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0010_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0011_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0012_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0013_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0014_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0015_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265645/run265645_ls0016_streamPhysics_StorageManager.dat',
            #'file:///afs/cern.ch/work/t/treis/public/data/run265377/run265377_ls0006_streamExpressCosmics_StorageManager.dat',
        )
    )
else:
    process.source = cms.Source(
        'PoolSource',
        #inputCommands = cms.untracked.vstring(
        #      'keep *',
        #      'drop *_*_*_RECO',
        #      ),
        #lumisToSkip = cms.untracked.VLuminosityBlockRange('272818:66-272818:67','272818:246','272818:280','272818:452-272818:457','272818:621-272818:max'),
        #lumisToSkip = cms.untracked.VLuminosityBlockRange('273725:1-273725:83'),
        #lumisToProcess = cms.untracked.VLuminosityBlockRange('272798:224-272798:1800'),
        #lumisToProcess = cms.untracked.VLuminosityBlockRange('274094:105-274094:332'),
        fileNames = cms.untracked.vstring(options.input)
        #fileNames = cms.untracked.vstring( *(
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/06A3C883-C223-E611-BA37-02163E011974.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/10702D88-C823-E611-92D3-02163E01410D.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/180D4AA8-C823-E611-9D1D-02163E014166.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/2EB1B998-C223-E611-93DE-02163E0135D4.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/2EFE1EAF-C223-E611-A6DA-02163E01465F.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/306AA7E4-AF23-E611-8222-02163E01438B.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/32DC9CAC-CE23-E611-8503-02163E011991.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/54D6EBA7-C223-E611-B438-02163E012326.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/625F8C89-C223-E611-8DD4-02163E012B20.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/62F8E198-CE23-E611-9A6D-02163E011E61.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/66F16F50-BE23-E611-A536-02163E01411D.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/7471103B-BE23-E611-AE73-02163E01422C.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/86E31688-C323-E611-B200-02163E014494.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/8A95439A-CE23-E611-B7D9-02163E01355E.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/8C4F19B5-C223-E611-9939-02163E012BC3.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/8CB32D64-BE23-E611-9B3C-02163E01466D.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/92ACFC31-BE23-E611-A94B-02163E01435A.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/9E41DF8A-C823-E611-8F86-02163E012855.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/AE67207E-C223-E611-9E5A-02163E014446.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/BA08237E-C223-E611-A722-02163E013718.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/BAF6999F-C823-E611-BBC6-02163E0138CA.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/E0B2003E-BE23-E611-81D0-02163E014235.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/E422A728-BE23-E611-8DF3-02163E01188E.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/E8C9697F-C223-E611-A731-02163E0124DD.root',
        #            'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2016B/ZeroBias/RAW/v2/000/274/094/00000/F6A94866-BE23-E611-9BE1-02163E01473B.root',
        #            #'file://badMuonMatches_272022_zb_308473ev.root',
        #            #'/store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/285/00000/F222D7FF-DCE5-E511-B523-02163E0138F4.root',
        #            #'/store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/285/00000/F89060E3-DDE5-E511-A924-02163E013756.root',
        #            #'/store/data/Commissioning2016/MinimumBias/RAW/v1/000/265/648/00000/5CB6F516-D1DC-E511-81E9-02163E012A67.root',
        #            #'/store/data/Commissioning2016/MinimumBias/RAW/v1/000/265/648/00000/96F15F1D-D2DC-E511-B326-02163E012A67.root',
        #            #'/store/data/Commissioning2016/MinimumBias/RAW/v1/000/265/648/00000/CA96375F-CBDC-E511-8CF6-02163E0118DE.root',
        #            #'root://eoscms.cern.ch//eos/cms/tier0/store/data/Commissioning2016/MiniDaq/RAW/v1/000/265/209/00000/0239C009-66DA-E511-AFCF-02163E0118DE.root',
        #            #'root://eoscms.cern.ch//eos/cms/tier0/store/data/Commissioning2016/MiniDaq/RAW/v1/000/265/213/00000/6A923799-66DA-E511-AFF4-02163E0118DE.root',
        #            #'root://eoscms.cern.ch//eos/cms/tier0/store/data/Commissioning2016/MiniDaq/RAW/v1/000/265/215/00000/6227134D-6DDA-E511-A216-02163E0118DE.root',
        #            #'root://eoscms.cern.ch//eos/cms/tier0/store/data/Commissioning2016/MiniDaq/RAW/v1/000/265/221/00000/A2D6E149-71DA-E511-8D6A-02163E0118DE.root',
        #            #'root://eoscms.cern.ch//eos/cms/tier0/store/data/Commissioning2016/MiniDaq/RAW/v1/000/265/222/00000/C0FFC9E2-71DA-E511-A45F-02163E0118DE.root',
        #) )
    )
if options.skip > 0:
    process.source.skipEvents = cms.untracked.uint32(options.skip)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.max)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
#process.Tracer = cms.Service("Tracer")

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1NtupleRECO nevts:200'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

process.load('L1Trigger.L1TMuon.simRegionalMuonCandSorter_cfi')
process.simRegionalMuonCandSorter.bmtfInput  = cms.InputTag("gmtStage2Digis", "BMTF")
process.simRegionalMuonCandSorter.omtfInput  = cms.InputTag("gmtStage2Digis", "OMTF")
process.simRegionalMuonCandSorter.emtfInput  = cms.InputTag("gmtStage2Digis", "EMTF")

process.load('L1Trigger.Configuration.SimL1Emulator_cff')
process.simGmtStage2Digis.barrelTFInput = cms.InputTag("gmtStage2Digis", "BMTF")
process.simGmtStage2Digis.forwardTFInput = cms.InputTag("gmtStage2Digis", "EMTF")
process.simGmtStage2Digis.overlapTFInput = cms.InputTag("gmtStage2Digis", "OMTF")
#process.simGmtStage2Digis.barrelTFInput = cms.InputTag("simRegionalMuonCandSorter", "BMTF")
#process.simGmtStage2Digis.forwardTFInput = cms.InputTag("simRegionalMuonCandSorter", "EMTF")
#process.simGmtStage2Digis.overlapTFInput = cms.InputTag("simRegionalMuonCandSorter", "OMTF")
process.simGmtStage2Digis.triggerTowerInput = cms.InputTag("emptyCaloCollsProducer","EmptyTriggerTower2x2s")
#process.gmtStage2Digis.debug = cms.untracked.bool (True)
#process.gtStage2Digis.debug = cms.untracked.bool (True)

# muon analyzer
process.muonAnalyzer = cms.EDAnalyzer('L1MuonAnalyzer',
          fedId = cms.untracked.int32(1402),
          fedRawDataTag = cms.InputTag(RAWDATASOURCE),
          bmtfTag = process.simGmtStage2Digis.barrelTFInput,
          #bmtfTag = cms.InputTag("none"),
          omtfTag = process.simGmtStage2Digis.overlapTFInput,
          #omtfTag = cms.InputTag("none"),
          emtfTag = process.simGmtStage2Digis.forwardTFInput,
          #emtfTag = cms.InputTag("none"),
          ugmtEmulatorTag = cms.InputTag("simGmtStage2Digis"),
          ugmtUnpackerTag = cms.InputTag("gmtStage2Digis", "Muon"),
          gmtTag = cms.InputTag("gtDigis"),
          #gmtTag = cms.InputTag("none"),
          debug = cms.untracked.bool(False),
          #printMismatch = cms.untracked.bool(False),
          printMismatch = cms.untracked.bool(True),
          commaSuppression = cms.untracked.bool(False),
          analyseValidationEvents = cms.untracked.bool(True),
          analyseNonValidationEvents = cms.untracked.bool(True),

          # shifts for matching with GMT
          # for OMTF/EMTF first pos. side then neg. side
          bmtfBxShifts = cms.untracked.vint32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
          #bmtfBxShifts = cms.untracked.vint32(-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3),
          omtfBxShifts = cms.untracked.vint32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
          #emtfBxShifts = cms.untracked.vint32(-1, -2, -1, -2, -1, -1, -1, -1, -1, -2, -1, -2),
          #emtfBxShifts = cms.untracked.vint32(0, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1),
          #emtfBxShifts = cms.untracked.vint32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
          emtfBxShifts = cms.untracked.vint32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
          #emtfBxShifts = cms.untracked.vint32(7, 1, -3, 2, 2, 0, 3, 0, 0, 0, -3, -1), # for run 264593
)

# bad muon filter
process.badMuonFilter = cms.EDFilter('FilterMatchingMuonEvents',
          bmtfTag = process.muonAnalyzer.bmtfTag,
          omtfTag = process.muonAnalyzer.omtfTag,
          emtfTag = process.muonAnalyzer.emtfTag,
          ugmtEmulatorTag = process.muonAnalyzer.ugmtEmulatorTag,
          ugmtUnpackerTag = process.muonAnalyzer.ugmtUnpackerTag,
          applyFilter = cms.bool(True),
          invertDecision = cms.bool(False),
)

if options.valonly:
    process.muonAnalyzer.analyseValidationEvents = cms.untracked.bool(True)
    process.muonAnalyzer.analyseNonValidationEvents = cms.untracked.bool(False)

process.emptyCaloCollsProducer = cms.EDProducer("L1TMicroGMTEmptyCaloCollsProducer")

# dump raw data
process.dumpRaw = cms.EDAnalyzer(
    "DumpFEDRawDataProduct",
    label = cms.untracked.string(RAWDATASOURCE),
    feds = cms.untracked.vint32(1402),
    dumpPayload = cms.untracked.bool ( True )
)


process.outputBadMatches = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
                SelectEvents = cms.vstring('ugmtHWEmulComp')
                ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('badMuonMatches.root'),
    outputCommands = cms.untracked.vstring(
              'keep *',
              'drop *_*_*_RECO',
              ),
)

# Additional output definition
# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string(options.output)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# ntuple producers
process.l1EventTree = cms.EDAnalyzer("L1EventTreeProducer",
    hltSource = cms.InputTag("TriggerResults","","HLT"),
    maxAllowedWeight = cms.untracked.double(-1),
    puDataFile = cms.untracked.string(''),
    puDataHist = cms.untracked.string(''),
    puMCFile = cms.untracked.string(''),
    puMCHist = cms.untracked.string(''),
    useAvgVtx = cms.untracked.bool(True)
)


process.l1UpgradeEmuTree = cms.EDAnalyzer("L1UpgradeTreeProducer",
    egToken = cms.untracked.InputTag("none"),
    jetToken = cms.untracked.InputTag("none"),
    maxL1Upgrade = cms.uint32(60),
    muonToken = cms.untracked.InputTag("simGmtStage2Digis"),
    sumToken = cms.untracked.InputTag("none"),
    tauTokens = cms.untracked.VInputTag("none")
)


process.l1UpgradeTfMuonEmuTree = cms.EDAnalyzer("L1UpgradeTfMuonTreeProducer",
    bmtfInputPhMuonToken = cms.untracked.InputTag("bmtfDigis"),
    bmtfInputThMuonToken = cms.untracked.InputTag("bmtfDigis"),
    bmtfMuonToken = cms.untracked.InputTag("simBmtfDigis","BMTF"),
    emtfMuonToken = cms.untracked.InputTag("simEmtfDigis","EMTF"),
    maxL1UpgradeTfMuon = cms.uint32(60),
    omtfMuonToken = cms.untracked.InputTag("simOmtfDigis","OMTF")
)


process.l1UpgradeTfMuonTree = cms.EDAnalyzer("L1UpgradeTfMuonTreeProducer",
    bmtfInputPhMuonToken = cms.untracked.InputTag("bmtfDigis"),
    bmtfInputThMuonToken = cms.untracked.InputTag("bmtfDigis"),
    bmtfMuonToken = cms.untracked.InputTag("gmtStage2Digis","BMTF"),
    emtfMuonToken = cms.untracked.InputTag("gmtStage2Digis","EMTF"),
    maxL1UpgradeTfMuon = cms.uint32(60),
    omtfMuonToken = cms.untracked.InputTag("gmtStage2Digis","OMTF")
)


process.l1UpgradeTree = cms.EDAnalyzer("L1UpgradeTreeProducer",
    egToken = cms.untracked.InputTag("none"),
    jetToken = cms.untracked.InputTag("none"),
    maxL1Upgrade = cms.uint32(60),
    muonToken = cms.untracked.InputTag("gmtStage2Digis","Muon"),
    sumToken = cms.untracked.InputTag("none"),
    tauTokens = cms.untracked.VInputTag(cms.InputTag("none"))
)

#sequence
process.RawToDigiAndUgmtEmul = cms.Sequence(process.emptyCaloCollsProducer
                                           +process.gtDigis
                                           +process.bmtfDigis
                                           #+process.dumpRaw
                                           +process.gmtStage2Digis
                                           #+process.gtStage2Digis
                                           #+process.simRegionalMuonCandSorter
                                           +process.simGmtStage2Digis
                                           +process.l1EventTree
                                           +process.l1UpgradeEmuTree
                                           #+process.l1UpgradeTfMuonEmuTree
                                           +process.l1UpgradeTfMuonTree
                                           +process.l1UpgradeTree
                                           +process.muonAnalyzer
                                           +process.badMuonFilter
)

process.ugmtHWEmulComp = cms.Path(process.RawToDigiAndUgmtEmul)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.output_step = cms.EndPath(process.outputBadMatches)

# Schedule definition
#process.schedule = cms.Schedule(process.ugmtHWEmulComp,process.endjob_step)
process.schedule = cms.Schedule(process.ugmtHWEmulComp,process.endjob_step,process.output_step)

# customisation of the process.

# End of customisation functions

if options.minidaq or not options.legacy:
    process.RawToDigiAndUgmtEmul.remove(process.gtDigis)
    process.muonAnalyzer.gmtTag = cms.InputTag("none")

# enable uGMT inputs
process.gmtParams.caloInputsDisable = cms.bool(False)
if options.bottomonly:
    print '# configure inputs for cosmics bottom only'
    process.gmtParams.bmtfInputsToDisable = cms.vuint32(1,1,1,1,1,1,1,0,0,0,0,0) # BMTF 0-11
    process.gmtParams.omtfInputsToDisable = cms.vuint32(1,1,1,1,1,1,1,1,1,1,1,1) # OMTF+0-5, OMTF-0-5
    process.gmtParams.emtfInputsToDisable = cms.vuint32(1,1,1,0,0,1,1,1,1,0,0,1) # EMTF+0-5, EMTF-0-5
else:
    print '# configure inputs for collisions run'
    process.gmtParams.bmtfInputsToDisable = cms.vuint32(0,0,0,0,0,0,0,0,0,0,0,0) # BMTF 0-11
    process.gmtParams.omtfInputsToDisable = cms.vuint32(0,0,0,0,0,0,0,0,0,0,0,0) # OMTF+0-5, OMTF-0-5
    process.gmtParams.emtfInputsToDisable = cms.vuint32(0,0,0,0,0,0,0,0,0,0,0,0) # EMTF+0-5, EMTF-0-5


