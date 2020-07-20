# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: repr --processName=REPR --python_filename=reprocess_test_10_5_0_pre1.py --no_exec -s L1 --datatier GEN-SIM-DIGI-RAW -n 2 --era Phase2 --eventcontent FEVTDEBUGHLT --filein root://cms-xrd-global.cern.ch//store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_pythia8/FEVT/PU200_pilot_103X_upgrade2023_realistic_v2_ext4-v1/280000/FF5C31D5-D96E-5E48-B97F-61A0E00DF5C4.root --conditions 103X_upgrade2023_realistic_v2 --beamspot HLLHC14TeV --geometry Extended2023D28 --fileout file:step2_2ev_reprocess_slim.root
import FWCore.ParameterSet.Config as cms

#from Configuration.ProcessModifiers.convertHGCalDigisSim_cff import convertHGCalDigisSim
from Configuration.StandardSequences.Eras import eras

#process = cms.Process('REPR', eras.Phase2C9_trigger) # for HLT TDR samples
#process = cms.Process('REPR', eras.Phase2C8_trigger) # for L1 TDR samples
process = cms.Process('REPR', eras.Phase2C8) # for L1 TDR samples
#process = cms.Process('REPR',eras.Phase2_trigger,convertHGCalDigisSim)
#process = cms.Process('REPR',eras.Phase2C4_trigger) # for MTD samples
#process = cms.Process('REPR',eras.Phase2C4_timing_layer_bar)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.Geometry.GeometryExtended2026D35Reco_cff') # for MTD samples
#process.load('Configuration.Geometry.GeometryExtended2026D35_cff') # for MTD samples
process.load('Configuration.Geometry.GeometryExtended2026D41Reco_cff') # for L1 TDR samples
process.load('Configuration.Geometry.GeometryExtended2026D41_cff') # for L1 TDR samples
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(-1)
    input = cms.untracked.int32(50)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/FE315623-B7A7-F64A-8BC7-1D0BD5D4EDDC.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/F5F4429F-108C-7549-B624-0906557932A0.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/F3EFE5AC-BFBD-1E44-9280-72783C6086D5.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/F3DBC383-776B-6345-B279-E1B2CED72342.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/F3917E94-0928-624A-AB08-ABD754A0FE2F.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/F0783A95-4278-224E-9C48-E954791CA3E8.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/EF65C3EF-66EF-0245-894D-8363F709BFE1.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/EC860668-E9D7-7C4A-A022-1F0377D8DE4C.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/E70927D8-16DA-664D-8ECD-5782820C78F3.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValZEE_14/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200v2-v1/10000/E6900BE8-3FBA-8A49-BFD8-9FBD45002FC8.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValElectronGunPt2To100/GEN-SIM-DIGI-RAW/106X_upgrade2023_realistic_v3_2023D41noPU-v1/10000/A22AA5DB-ACEE-1140-B081-104F634079A1.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValElectronGunPt2To100/GEN-SIM-DIGI-RAW/106X_upgrade2023_realistic_v3_2023D41noPU-v1/10000/8B75BCAF-FF0C-094C-AB40-08F104148BC0.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValElectronGunPt2To100/GEN-SIM-DIGI-RAW/106X_upgrade2023_realistic_v3_2023D41noPU-v1/10000/71C02E39-ED72-054B-871F-6B1FD1A1C14A.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValElectronGunPt2To100/GEN-SIM-DIGI-RAW/106X_upgrade2023_realistic_v3_2023D41noPU-v1/10000/283255C6-1E20-6F48-8B8B-31E6A62BD48D.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValElectronGunPt2To100/GEN-SIM-DIGI-RAW/106X_upgrade2023_realistic_v3_2023D41noPU-v1/10000/190EDE9F-770B-174A-8BA6-F7814FC67FD4.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/0D9ADA63-D0F0-3D40-8AF1-EA7C250ADD27.root'
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/A62FD17F-85CC-DC4C-85AB-BDFED4C33156.root'
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/17BA48DF-E784-1E4D-A304-EDEE636282E6.root'
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/D9B2E929-00C9-3546-A50B-CE8CEABFB934.root'
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/742FA8D4-FBDC-F74B-B24C-6396357A09EA.root'
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/FC554DC8-1F1C-644A-B131-D07533351CA3.root'
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/D3B381B5-6FE4-A245-8548-EB9E06231F33.root'
'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/FF57637B-0567-DD46-9838-CDF532DAF083.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/FD4BE924-35A6-F540-A1EC-405BA169FEBB.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/FD32690F-270D-FF42-9ED7-6D99AB85C9CA.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/FC9D2ACA-BA79-3845-849C-48FB12BFFB35.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/F68EB76C-6108-E54E-86AE-BE4846A0EF22.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/F618CD0A-261A-A64D-A647-1DFDD6779580.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/F55F3728-20B3-AB4D-8EBB-BD33DCDC294C.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/F5347339-051F-694E-8799-541F97ABC573.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/F3A66C8A-ECB4-2143-86E5-F6BD984F4A8D.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/EC4927F7-3EDA-854A-9727-8931CE6515AB.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/EC33357F-35FB-4846-BBA4-5FE99DA0FB74.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/EBE38D79-3543-6D49-8E12-D3DE4BB3078B.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/EB2F5AD4-BA35-474C-A9AF-C96F5F4B345E.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/E9BAAE68-815C-2B49-8779-FBC4079EB4EC.root',
#'/store/mc/PhaseIITDRSpring19DR/SingleElectron_PT2to100/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/60000/E7C6EAFC-8CB2-A944-8875-B5D9CD7312D0.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValPhotonGunPt8To150/GEN-SIM-DIGI-RAW/106X_upgrade2023_realistic_v3_2023D41noPU-v1/10000/B3518F14-DB91-0443-A349-1F1EB2B9B8FB.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/FCA3F03E-4C9A-7941-86D8-FBD29777AA0A.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/F5B7509A-C9E1-E54B-8776-B79D44F4775A.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/F3930679-62C8-3C48-A20D-56D3A1DD0991.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/F389E8F6-71C2-DA41-AF79-A6F2C94DA05B.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/F2915EF3-0E47-F046-B918-BE853DDDFCE4.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/EFF6EF7F-55D9-1343-A908-49B42F6A0D27.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/EF28B365-CE7E-754D-8FCE-B3D5CFEA8B00.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/EE678F70-58D4-BD41-9E91-669EC57113B7.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/EC0B14ED-D0CF-9A4B-9D75-DEE0D4C29EF9.root',
#'/store/relval/CMSSW_10_6_0_patch2/RelValNuGun/GEN-SIM-DIGI-RAW/PU25ns_106X_upgrade2023_realistic_v3_2023D41PU200-v1/10000/E530BAE3-09BC-CF44-A24F-14245C11CEEE.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FFB3195D-E113-3744-877D-44E21C060358.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FF052B08-416D-FA46-B296-3BE28535CF30.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FEFEFA95-3375-F240-B0B4-E43642689BC1.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FE8164F6-7AC9-D14C-B252-08ABFFB17B10.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FDF53340-E1C6-7044-A7D2-626B29EEC487.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FD7C576A-EC4D-1043-83E1-E6BC98242D62.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FCF0FFF8-67AF-BA40-AB38-03DDB109B751.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FCE5299A-E437-0A49-89E1-CD6E232859DA.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FC3F5D1A-F17D-CD49-B0B8-D844E790696F.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FC370019-437D-DF4C-AEA8-025580D035B5.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FC339EFC-1DC4-9640-821A-FA6AF8F7DB7D.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FC1F62F4-C720-3F46-B75C-0E2DF4789C14.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FBB89A73-743C-D146-A99D-10ACACB53EFB.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FB5BDBDA-0806-7045-AD3E-CCA6BF782ACC.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FB346BAA-DE51-4A4E-96A6-902BC124680B.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FB1BEB4A-969E-F646-BD14-6EF16EA29462.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/FAFA65F4-B73C-3342-B977-927BBD128BBF.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/F9BF6F44-3A8C-9342-81AD-E87DFEAA3254.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/F9698F16-C2B8-1240-842D-860F2AE744C4.root',
#'/store/mc/PhaseIITDRSpring19DR/Nu_E10-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v3/70001/F91DF4FA-F2D7-A34A-B91D-9F234D48BE6A.root',
#'/store/mc/PhaseIIMTDTDRAutumn18DR/SingleE_FlatPt-2to100/FEVT/NoPU_103X_upgrade2023_realistic_v2-v1/70000/B0464430-CA74-7844-A36A-A5EF21C3D7C0.root'
#'/store/mc/PhaseIIMTDTDRAutumn18DR/SingleE_FlatPt-2to100/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/70000/FF17CBE6-81E5-8D43-B58B-6DF17222820E.root',
#'/store/mc/PhaseIIMTDTDRAutumn18DR/NeutrinoGun_E_10GeV/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/280000/EFFCC733-2B7F-C645-929D-505B1E0949D6.root'
),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('repr nevts:2'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step2_2ev_reprocess_slim.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)
process.FEVTDEBUGHLToutput.outputCommands.append('keep *_*_*_REPR')

# Additional output definition


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2023_realistic_v2', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')

# Path and EndPath definitions
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# run also the TMTT producer
#--- Load code that produces our L1 tracks and makes corresponding histograms.
#process.load('L1Trigger.TrackFindingTMTT.TMTrackProducer_cff')
#--- Alternative cfg including improvements. Not in firmware.
#process.load('L1Trigger.TrackFindingTMTT.TMTrackProducer_Ultimate_cff')

# Override defaults
#process.TMTrackProducer.ParamsProducer.EnableTruncation = cms.bool( True )

#process.tmtt = cms.Path(process.TMTrackProducer)

# menu trees

#process.load("L1Trigger.L1TNtuples.l1PhaseIITreeProducer_cfi")
process.load("L1Trigger.L1TNtuples.l1CaloTowerTree_cfi")
process.load("L1Trigger.L1TNtuples.l1GeneratorTree_cfi")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.l1CaloTowerTree.ecalEBToken = cms.untracked.InputTag("simEcalEBTriggerPrimitiveDigis")
process.l1CaloTowerTree.ecalToken = cms.untracked.InputTag("simEcalTriggerPrimitiveDigis")
process.l1CaloTowerTree.hcalToken = cms.untracked.InputTag("simHcalTriggerPrimitiveDigis")
process.l1CaloTowerTree.l1TowerToken = cms.untracked.InputTag("simCaloStage2Digis")
process.calotptree = cms.Path(process.l1CaloTowerTree)
process.load("L1Trigger.L1TNtuples.l1EventTree_cfi")
process.evttree = cms.Path(process.offlineBeamSpot * process.l1EventTree)

# tree with EG Tk Iso variables
process.l1PhaseIIEGTkIsoTree = cms.EDAnalyzer("L1PhaseIIEGTkIsoTreeProducer",
    l1EgBarrel = cms.InputTag("L1EGammaClusterEmuProducer"),
    l1EgHGC = cms.InputTag("l1EGammaEEProducer","L1EGammaCollectionBXVWithCuts"),
    #l1Tracks = cms.InputTag("TTTracksFromTracklet", "Level1TTTracks"),
    l1Tracks = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
    #l1Tracks = cms.InputTag("TMTrackProducer","TML1Tracks"),
    l1PFCandidates = cms.InputTag("l1pfCandidates","PF"),
    #l1PFCandidates = cms.InputTag("l1pfCandidates","Puppi"),
    trackerGeometry = cms.string("idealForDigi"), # tracker geometry record to get for calculating pT from 2 stubs (unused if useTwoStubsPt is False)
    egBarrelMinEt = cms.double(2.), # minimum Et for barrel EG object
    egHGCMinEt = cms.double(2.), # minimum Et for HGC EG object
    #trackMinPt = cms.double(2.), # minimum Pt to select tracks for matching
    trackMaxChi2 = cms.double(1e10), # maximum Chi2 to select tracks for matching
    trackMinPt = cms.double(10.), # minimum Pt to select tracks for matching
    #trackMaxChi2 = cms.double(100), # maximum Chi2 to select tracks for matching
    useTwoStubsPt = cms.bool(False), # calculate track pT from 2 stubs
    #trackEGammaMatchType = cms.string("PtDependentCut"),
    trackEGammaMatchType = cms.string("EllipticalCut"),
    trackEGammaDeltaPhi = cms.vdouble(0.07, 0., 0.), # functional Delta Phi cut parameters to match Track with L1EG objects
    trackEGammaDeltaR = cms.vdouble(0.08, 0., 0.), # functional Delta R cut parameters to match Track with L1EG objects
    #trackEGammaDeltaEta = cms.vdouble(1e10, 0., 0.), # Delta Eta cutoff to match Track with L1EG objects
    trackEGammaDeltaEta = cms.vdouble(0.015, 0.025, 0.0075), # Delta Eta cutoff to match Track with L1EG objects (elliptical cut values (EB high, EB low, HGC))
    trackMinPtForIsoEB = cms.double(2.), # minimum Pt to select tracks for the isolation in EB
    trackMinPtForIsoHGC = cms.double(2.), # minimum Pt to select tracks for the isolation in HGCal
    trackMaxChi2ForIsoEB = cms.double(1e10), # maximum Chi2 to select tracks for the isolation in EB
    trackMaxChi2ForIsoHGC = cms.double(100), # maximum Chi2 to select tracks for the isolation in HGCal
    trackMinNStubsForIsoEB = cms.uint32(0), # minimum number of stubs to select tracks for the isolation in EB
    trackMinNStubsForIsoHGC = cms.uint32(4), # minimum number of stubs to select tracks for the isolation in HGCal
    dRMinForIso = cms.double(0.), # minimum dR to select tracks for the isolation
    dRMaxForIso = cms.double(0.8) # maximum dR to select tracks for the isolation
)
process.phase2EGTkIsoTreePath = cms.Path(process.l1PhaseIIEGTkIsoTree * process.l1GeneratorTree)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1NtuplePhaseII.root')
)

# Schedule definition
process.schedule = cms.Schedule(
    process.L1simulation_step,
    process.evttree,
    #process.extraCollectionsMenuTree,
    #process.runmenutree,
    process.phase2EGTkIsoTreePath,
    #process.calotptree,
    process.endjob_step#,
    #process.FEVTDEBUGHLToutput_step
)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# Customisation from command line

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
#from L1Trigger.Configuration.customiseUtils import DropDepricatedProducts,DropOutputProducts

#call to customisation function DropDepricatedProducts imported from L1Trigger.Configuration.customiseUtils
#process = DropDepricatedProducts(process)

#from L1Trigger.Configuration.customiseUtils import L1TrackTriggerTracklet
#process = L1TrackTriggerTracklet(process)
#from L1Trigger.Configuration.customiseUtils import L1TrackTriggerHybrid
# since the customisation functions are not there anymore in 11_1_2 put the function here
def L1TrackTriggerHybrid(process):
    #print "L1T INFO:  run the L1TrackStep with Tracklet."
    process.load('L1Trigger.TrackFindingTracklet.Tracklet_cfi')
    process.L1TrackTriggerHybrid_step = cms.Path(process.TTTracksFromTrackletEmulation)
    process.schedule.insert(2,process.L1TrackTriggerHybrid_step)
    return process

process = L1TrackTriggerHybrid(process)
#from L1Trigger.Configuration.customiseUtils import L1TrackTriggerTMTT
#process = L1TrackTriggerTMTT(process)

process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string('ERROR'),
    default = cms.untracked.PSet(
        limit = cms.untracked.int32(10000)
    )
)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
