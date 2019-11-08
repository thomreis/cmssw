import FWCore.ParameterSet.Config as cms

pfClustersFromHGC3DClusters = cms.EDProducer("PFClusterProducerFromHGC3DClusters",
    src = cms.InputTag("hgcalBackEndLayer2Producer","HGCalBackendLayer2Processor3DClustering"),
    corrector = cms.string("L1Trigger/Phase2L1ParticleFlow/data/hadcorr_HGCal3D_TC.root"),
    correctorEmfMax = cms.double(1.125),
    preEmId  = cms.string("hOverE < 0.3 && hOverE >= 0"),
    emVsPionID = cms.PSet(
        isPUFilter = cms.bool(False),
        preselection = cms.string(""),
        method = cms.string("BDT"), # "" to be disabled, "BDT" to be enabled
        variables = cms.VPSet(
            cms.PSet(name = cms.string("fabs(eta)"), value = cms.string("abs(eta())")),
            cms.PSet(name = cms.string("coreShowerLength"), value = cms.string("coreShowerLength()")),
            cms.PSet(name = cms.string("maxLayer"), value = cms.string("maxLayer()")),
            cms.PSet(name = cms.string("hOverE"), value = cms.string("hOverE()")),
            cms.PSet(name = cms.string("sigmaZZ"), value = cms.string("sigmaZZ()")),
        ),
        weightsFile = cms.string("L1Trigger/Phase2L1ParticleFlow/data/hgcal_egID/Photon_vs_Pion_BDTweights.xml.gz"),
        wp = cms.string("0.01")
    ),
    emVsPUID = cms.PSet(
        isPUFilter = cms.bool(True),
        preselection = cms.string(""),
        method = cms.string("BDT"), # "" to be disabled, "BDT" to be enabled
        variables = cms.VPSet(
            cms.PSet(name = cms.string("fabs(eta)"), value = cms.string("abs(eta())")),
            cms.PSet(name = cms.string("coreShowerLength"), value = cms.string("coreShowerLength()")),
            cms.PSet(name = cms.string("maxLayer"), value = cms.string("maxLayer()")),
            cms.PSet(name = cms.string("sigmaPhiPhiTot"), value = cms.string("sigmaPhiPhiTot()")),
        ),
        weightsFile = cms.string("L1Trigger/Phase2L1ParticleFlow/data/hgcal_egID/Photon_Pion_vs_Neutrino_BDTweights.xml.gz"),
        wp = cms.string("-0.02")
    ),
    emOnly = cms.bool(False),
    etMin = cms.double(1.0),
    resol = cms.PSet(
        etaBins = cms.vdouble( 1.900,  2.200,  2.500,  2.800,  2.950),
        offset  = cms.vdouble( 2.593,  3.089,  2.879,  2.664,  2.947),
        scale   = cms.vdouble( 0.120,  0.098,  0.099,  0.098,  0.124),
        kind    = cms.string('calo')
    ),
)


from Configuration.Eras.Modifier_phase2_hgcalV10_cff import phase2_hgcalV10
phase2_hgcalV10.toModify(pfClustersFromHGC3DClusters,
    corrector = "L1Trigger/Phase2L1ParticleFlow/data/hadcorr_HGCal3D_TC_106X.root",
    resol = cms.PSet(
        etaBins = cms.vdouble( 1.900,  2.200,  2.500,  2.800,  2.950),
        offset  = cms.vdouble( 2.801,  2.735,  2.778,  2.986,  2.808),
        scale   = cms.vdouble( 0.106,  0.101,  0.101,  0.097,  0.124),
        kind    = cms.string('calo')
    ),
) 
