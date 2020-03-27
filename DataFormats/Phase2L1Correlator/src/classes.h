/*! \brief   Definition of all the relevant data types
 *  \details Herw we declare instances of all the relevant types. 
 *
 *  \author Vladimir Rekovic
 *  \date   2020, Mar 26 
 *
 */

#ifndef PHASE2L1CORRELATOR_CLASSES_H
#define PHASE2L1CORRELATOR_CLASSES_H

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
//#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

// includes needed for the L1TrackTriggerObjects

#include "DataFormats/Phase2L1Correlator/interface/L1TkPrimaryVertex.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkEtMissParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkEtMissParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkEmParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkEmParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkEGTauParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TrkTauParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1CaloTkTauParticle.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkElectronParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkElectronParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkJetParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkJetParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkHTMissParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkHTMissParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkMuonParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkMuonParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkGlbMuonParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkGlbMuonParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkTauParticle.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkTauParticleFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkPhiCandidate.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkPhiCandidateFwd.h"

#include "DataFormats/Phase2L1Correlator/interface/L1TkBsCandidate.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkBsCandidateFwd.h"

#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/RefProd.h"

namespace {

  //  Phase2 L1 Correlator Objects :

  struct dictionaryl1tkobj {

    // L1 Primary Vertex
    l1t::L1TkPrimaryVertex trzvtx;
    edm::Wrapper<l1t::L1TkPrimaryVertexCollection> trzvtxColl;
    edm::Ref<l1t::L1TkPrimaryVertexCollection> trkvtxRef;

    // L1TkEtMiss... following L1EtMiss...
    l1t::L1TkEtMissParticle TketMiss;
    l1t::L1TkEtMissParticleCollection TketMissColl;
    edm::Wrapper<l1t::L1TkEtMissParticle> w_TketMiss;
    edm::Wrapper<l1t::L1TkEtMissParticleCollection> w_TketMissColl;
    //l1t::L1TkEtMissParticleRef refTkEtMiss ;
    //l1t::L1TkEtMissParticleRefVector refTkVecEtMiss ;
    //l1t::L1TkEtMissParticleVectorRef vecTkRefEtMissColl ;
    //l1t::L1TkEtMissParticleRefProd refTkProdEtMiss ;
    //edm::reftobase::Holder<reco::Candidate, l1t::L1TkEtMissParticleRef> rtbTkm1;
    //edm::reftobase::Holder<reco::Candidate, l1t::L1TkEtMissParticleRefProd> rtbTkm2;

    // L1TkEmParticle
    l1t::L1TkEmParticleCollection trkemColl;
    edm::Wrapper<l1t::L1TkEmParticleCollection> w_trkemColl;
    l1t::L1TkEmParticleRef reftrkEm;
    //l1t::L1TkEmParticleRefVector refVectrkEmColl ;
    //l1t::L1TkEmParticleVectorRef vecReftrkEmColl ;
    //edm::reftobase::Holder<reco::Candidate, l1t::L1TkEmParticleRef> rtbtrke;

    // L1TkEGTauParticle
    l1t::L1TkEGTauParticleCollection trkegColl;
    edm::Wrapper<l1t::L1TkEGTauParticleCollection> w_trkegColl;
    l1t::L1TkEGTauParticleRef reftrkEG;

    // L1TrkTauParticle
    l1t::L1TrkTauParticleCollection tktauColl;
    edm::Wrapper<l1t::L1TrkTauParticleCollection> w_tktauColl;
    l1t::L1TrkTauParticleRef reftkTau;

    // L1CaloTkTauParticle
    l1t::L1CaloTkTauParticleCollection calotrktauColl;
    edm::Wrapper<l1t::L1CaloTkTauParticleCollection> w_calotrktauColl;
    l1t::L1CaloTkTauParticleRef refcalotrkTau;

    // L1TkElectronParticle
    l1t::L1TkElectronParticleCollection trkeleColl;
    edm::Wrapper<l1t::L1TkElectronParticleCollection> w_trkeleColl;
    l1t::L1TkElectronParticleRef reftrkEle;

    // L1TkJetParticle
    l1t::L1TkJetParticleCollection trkjetColl;
    edm::Wrapper<l1t::L1TkJetParticleCollection> w_trkjetColl;
    l1t::L1TkJetParticleRef reftrkJet;
    l1t::L1TkJetParticleRefProd refTkProdJet;

    // L1TkHTMissParticle
    l1t::L1TkHTMissParticle TkHTMiss;
    l1t::L1TkHTMissParticleCollection TkHTMissColl;
    edm::Wrapper<l1t::L1TkHTMissParticle> w_TkHTMiss;
    edm::Wrapper<l1t::L1TkHTMissParticleCollection> w_TkHTMissColl;

    // L1TkMuonParticle
    l1t::L1TkMuonParticleCollection trkmuColl ;
    edm::Wrapper<l1t::L1TkMuonParticleCollection> w_trkmuColl;
    l1t::L1TkMuonParticleRef reftrkMu ;

    // L1TkGlbMuonParticle
    l1t::L1TkGlbMuonParticleCollection trkglbmuColl;
    edm::Wrapper<l1t::L1TkGlbMuonParticleCollection> w_trkglbmuColl;
    l1t::L1TkGlbMuonParticleRef reftrkGlbMu;

    // L1TkTauParticle
    l1t::L1TkTauParticleCollection trktauColl;
    edm::Wrapper<l1t::L1TkTauParticleCollection> w_trktauColl;
    l1t::L1TkTauParticleRef reftrkTau;

    // L1TkPhiCandidate
    l1t::L1TkPhiCandidateCollection trkPhiColl;
    edm::Wrapper<l1t::L1TkPhiCandidateCollection> w_trkPhiColl;
    l1t::L1TkPhiCandidateRef reftrkPhi;
    //l1t::L1TkPhiCandidateRefProd refTkProdPhi;

    // L1TkBsCandidate
    l1t::L1TkBsCandidateCollection trkBsColl;
    edm::Wrapper<l1t::L1TkBsCandidateCollection> w_trkBsColl;
    l1t::L1TkBsCandidateRef reftrkBs;
    //l1t::L1TkBsCandidateRefProd refTkProdBs;
  };
}  // namespace

#endif
