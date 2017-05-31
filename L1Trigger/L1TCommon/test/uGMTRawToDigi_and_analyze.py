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
        #fileNames = cms.untracked.vstring(options.input)
        fileNames = cms.untracked.vstring(
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/023CF9D4-4745-E711-A8E1-02163E0146FE.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/085FEBB5-4645-E711-AB5E-02163E013902.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/08D7F942-4745-E711-86A8-02163E011825.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/08FA4A0C-4645-E711-B797-02163E01A295.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/0A59B107-4745-E711-B578-02163E01A46D.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/0C154D59-4645-E711-A5EB-02163E019C9F.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/0CA922D7-4545-E711-9C67-02163E01A364.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/100FC583-4645-E711-8539-02163E01410A.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/10EA2E99-4645-E711-BBCD-02163E01A654.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/122BF6CF-4645-E711-9C10-02163E019CD2.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/12E73AF3-4545-E711-BF2C-02163E0146FE.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/160D77E1-4645-E711-953D-02163E01A5E6.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/183AFD44-4745-E711-BB72-02163E012AC6.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/1CCDB243-4745-E711-8FBD-02163E01A672.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/1EF41D24-4645-E711-8E18-02163E0146E3.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/203FB832-4645-E711-9ED7-02163E019B6D.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/286A1EC0-4745-E711-B452-02163E012BAD.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/28C82F2F-4645-E711-9F6B-02163E01A616.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/381E0963-4645-E711-A7EA-02163E011C52.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/405F29B3-4645-E711-A613-02163E019C98.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/40688B9B-4845-E711-A791-02163E0145BD.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/46C6640F-4745-E711-A67B-02163E0145A9.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/4E050EF9-3F45-E711-B01A-02163E01A70B.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/520DCC32-4645-E711-BFE2-02163E01A5CF.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/58D889AB-3E45-E711-9BDC-02163E01A379.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/66170BD4-4745-E711-AB30-02163E014427.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/66D0C81D-4645-E711-875C-02163E01A405.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/6ACEC037-4645-E711-9A33-02163E01A4EA.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/7258F51F-4745-E711-8EAA-02163E0127AE.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/768A9946-4645-E711-BF24-02163E019C5D.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/7A2B2278-4645-E711-8836-02163E0134BF.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/7C6A9BD0-4745-E711-818C-02163E013395.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/8070F1FE-4545-E711-A16C-02163E01286A.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/80C2371B-4645-E711-BC99-02163E019B7A.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/8A600140-4645-E711-87E2-02163E01A562.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/8E47D4E1-4545-E711-80A9-02163E01A672.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/90EBD821-4745-E711-8669-02163E013853.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/96289D75-4645-E711-A528-02163E01A59F.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/967B4124-4645-E711-9183-02163E011CE2.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/9690B9F6-4545-E711-A070-02163E01A4A9.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/9C3F5405-4745-E711-8683-02163E0134E9.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/9E00B66D-4645-E711-BD83-02163E01A303.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/A4000365-4745-E711-A0E1-02163E014714.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/A4106AD5-4645-E711-82E2-02163E014761.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/A6F09711-4645-E711-A18D-02163E01A365.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/A8688B7B-4645-E711-B75C-02163E011A83.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/AA01F4E5-4645-E711-955D-02163E01A28F.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/AAF30A74-4645-E711-9D55-02163E019E08.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/B403F4B5-4745-E711-9D74-02163E012B39.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/B805C637-4845-E711-8BB3-02163E014653.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/B823C9F9-4645-E711-8B20-02163E01A792.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/BE93B32F-4745-E711-A679-02163E014430.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/C66C12A9-4745-E711-8EA6-02163E01420A.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/D40519AC-4645-E711-A676-02163E01426A.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/D46C0A36-4645-E711-A112-02163E019D54.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/D8F42763-4745-E711-8739-02163E01A52D.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/E03411FC-4545-E711-A766-02163E011C45.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/E464EC09-4745-E711-B0C8-02163E011E6D.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F0AB22E6-4545-E711-A549-02163E019C10.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F220DCC8-4645-E711-A001-02163E0134F5.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F4A6C7B4-4645-E711-83C8-02163E0136A4.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F4CB0EF4-4545-E711-854B-02163E011907.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F61AAAF2-4645-E711-8627-02163E014662.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F862A535-4645-E711-8658-02163E01A70B.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/F8BC5805-4645-E711-AED7-02163E011D27.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/FE67AE75-4745-E711-9C44-02163E0129C6.root',
                    'root://eoscms//eos/cms/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/FEBADB0A-4645-E711-A55D-02163E01A666.root',
        #            'root://eoscms//eos/cms/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/291/504/00000/DAEAE55E-851E-E711-999F-02163E014562.root',
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
        )
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
          #ugmtEmulatorTag = cms.InputTag("simGmtStage2Digis", "imdMuonsOMTFNeg"),
          #ugmtUnpackerTag = cms.InputTag("gmtStage2Digis", "imdMuonsOMTFNeg"),
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
process.gmtParams.caloInputsDisable = cms.bool(True)
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


