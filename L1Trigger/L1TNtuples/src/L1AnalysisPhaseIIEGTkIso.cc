// Matching Algorithm
#include "L1Trigger/L1TTrackMatch/interface/L1TkElectronTrackMatchAlgo.h"
#include "L1Trigger/L1TTrackMatch/interface/pTFrom2Stubs.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "L1Trigger/Phase2L1ParticleFlow/interface/L1TPFUtils.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIEGTkIso.h"

L1Analysis::L1AnalysisPhaseIIEGTkIso::L1AnalysisPhaseIIEGTkIso(const edm::ParameterSet& pSet) :
  pSet_(pSet),
  egBarrelEtMin_((float)pSet.getParameter<double>("egBarrelMinEt")),
  egHGCEtMin_((float)pSet.getParameter<double>("egHGCMinEt")),
  trkPtMin_((float)pSet.getParameter<double>("trackMinPt")),
  trkChi2Max_((float)pSet.getParameter<double>("trackMaxChi2")),
  useTwoStubsPt_(pSet.getParameter<bool>("useTwoStubsPt")),
  trkEGMatchType_(pSet.getParameter<std::string>("trackEGammaMatchType")),
  dEtaCutoff_(pSet.getParameter<std::vector<double>>("trackEGammaDeltaEta")),
  dPhiCutoff_(pSet.getParameter<std::vector<double>>("trackEGammaDeltaPhi")),
  dRCutoff_(pSet.getParameter<std::vector<double>>("trackEGammaDeltaR")),
  trkPtMinIsoEB_((float)pSet.getParameter<double>("trackMinPtForIsoEB")),
  trkPtMinIsoHGC_((float)pSet.getParameter<double>("trackMinPtForIsoHGC")),
  trkChi2MaxIsoEB_((float)pSet.getParameter<double>("trackMaxChi2ForIsoEB")),
  trkChi2MaxIsoHGC_((float)pSet.getParameter<double>("trackMaxChi2ForIsoHGC")),
  trkNStubMinIsoEB_((unsigned int)pSet.getParameter<unsigned int>("trackMinNStubsForIsoEB")),
  trkNStubMinIsoHGC_((unsigned int)pSet.getParameter<unsigned int>("trackMinNStubsForIsoHGC")),
  dRMinIso_((float)pSet.getParameter<double>("dRMinForIso")),
  dRMaxIso_((float)pSet.getParameter<double>("dRMaxForIso"))
{
}

void L1Analysis::L1AnalysisPhaseIIEGTkIso::SetEGWithTracks(const edm::Handle<l1t::EGammaBxCollection>& egBarrel, const edm::Handle<l1t::EGammaBxCollection>& egHGC, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField)
{
  //std::cout << "This event contains " << tttrack->size() << " tracks." << std::endl;

  // barrel
  for (auto bx = egBarrel->getFirstBX(); bx <= egBarrel->getLastBX(); ++bx) {
    for (l1t::EGammaBxCollection::const_iterator it = egBarrel->begin(bx); it != egBarrel->end(bx); ++it) {
      if (it->et() > egBarrelEtMin_) {
        setBranches(it, tttrack, tGeom, pfCands, bField, bx, false);
      }
    }
  }

  // HGC
  for (auto bx = egHGC->getFirstBX(); bx <= egHGC->getLastBX(); ++bx) {
    for (l1t::EGammaBxCollection::const_iterator it = egHGC->begin(bx); it != egHGC->end(bx); ++it) {
      if (it->et() > egHGCEtMin_) {
        setBranches(it, tttrack, tGeom, pfCands, bField, bx, true);
      }
    }
  }
}

void L1Analysis::L1AnalysisPhaseIIEGTkIso::SetPFCands(const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField)
{
  for (size_t i = 0; i < pfCands->size(); ++i) {
    const auto pfCand = pfCands->at(i);
    const auto vtx = math::XYZTLorentzVector(pfCand.vx(), pfCand.vy(), pfCand.vz(), 0.);
    const auto charge = pfCand.charge();
    const auto etaPhiAtCalo = l1tpf::propagateToCalo(pfCand.p4(), vtx, charge, bField);

    // store PF cand info
    l1Phase2EGTkIso_.pfId.push_back(pfCand.id());
    l1Phase2EGTkIso_.pfEt.push_back(pfCand.pt());
    l1Phase2EGTkIso_.pfEta.push_back(pfCand.eta());
    l1Phase2EGTkIso_.pfPhi.push_back(pfCand.phi());
    l1Phase2EGTkIso_.pfPuppiWeight.push_back(pfCand.puppiWeight());
    l1Phase2EGTkIso_.pfEtaAtCalo.push_back(etaPhiAtCalo.first);
    l1Phase2EGTkIso_.pfPhiAtCalo.push_back(etaPhiAtCalo.second);
  }
}

void L1Analysis::L1AnalysisPhaseIIEGTkIso::setBranches(const l1t::EGammaBxCollection::const_iterator& it, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField, const int bx, const bool isHGC)
{
  //if (it->hwQual() == 5) {
  //  std::cout << "EG " << l1Phase2EGTkIso_.nEG << ": et: " << it->et() << ", eta: " << it->eta() << ", phi: " << it->phi() << ", Q: " << it->hwQual() << std::endl;
  //}
  ++l1Phase2EGTkIso_.nEG;
  l1Phase2EGTkIso_.EGEt.push_back(it->et());
  l1Phase2EGTkIso_.EGEta.push_back(it->eta());
  l1Phase2EGTkIso_.EGPhi.push_back(it->phi());
  l1Phase2EGTkIso_.EGIso.push_back(it->isoEt());
  l1Phase2EGTkIso_.EGHwQual.push_back(it->hwQual());
  l1Phase2EGTkIso_.EGBx.push_back(bx);
  if (isHGC) {
    l1Phase2EGTkIso_.EGHGC.push_back(1);
    bool quality = it->hwQual() == 5;
    l1Phase2EGTkIso_.EGPassesLooseTrackID.push_back(quality); 
    l1Phase2EGTkIso_.EGPassesPhotonID.push_back(quality);
  } else {
    l1Phase2EGTkIso_.EGHGC.push_back(0);
    bool quality = ((it->hwQual() >> 1) & 1) > 0;
    l1Phase2EGTkIso_.EGPassesLooseTrackID.push_back(quality); 
    quality = ((it->hwQual() >> 2) & 1) > 0;
    l1Phase2EGTkIso_.EGPassesPhotonID.push_back(quality);
  }

  // search matching track
  double matchedTrackDR = 999.;
  double matchedTrackDEta = 999.;
  double matchedTrackDPhi = 999.;
  const auto matchedTrackPtr = findMatchedTrack(it, tttrack, tGeom, matchedTrackDR, matchedTrackDEta, matchedTrackDPhi);
  int matchedTrackIdx = -1;
  if (matchedTrackPtr.isNonnull()) {
    matchedTrackIdx = matchedTrackPtr.key();
    //std::cout << "Matched track: EG idx: " << l1Phase2EGTkIso_.nEG - 1 << ", pt: " << matchedTrackPtr->getMomentum().perp() << ", eta: " << matchedTrackPtr->eta() << ", phi: " << matchedTrackPtr->phi() << ", dR: " << matchedTrackDR << std::endl;
    l1Phase2EGTkIso_.EGHasMatchedTrack.push_back(1);
    l1Phase2EGTkIso_.matchedTkEGIdx.push_back(l1Phase2EGTkIso_.nEG - 1);
    l1Phase2EGTkIso_.matchedTkPt.push_back(matchedTrackPtr->momentum().perp());
    l1Phase2EGTkIso_.matchedTkEta.push_back(matchedTrackPtr->eta());
    l1Phase2EGTkIso_.matchedTkPhi.push_back(matchedTrackPtr->phi());
    l1Phase2EGTkIso_.matchedTkRInv.push_back(matchedTrackPtr->rInv());
    l1Phase2EGTkIso_.matchedTkChi2.push_back(matchedTrackPtr->chi2());
    l1Phase2EGTkIso_.matchedTkDR.push_back(matchedTrackDR);
    l1Phase2EGTkIso_.matchedTkDEta.push_back(matchedTrackDEta);
    l1Phase2EGTkIso_.matchedTkDPhi.push_back(matchedTrackDPhi);
  } else {
    l1Phase2EGTkIso_.EGHasMatchedTrack.push_back(0);
  }

  // find other tracks in cone around EG for isolation calculation
  setIsoTracks(it, tttrack, tGeom, matchedTrackIdx);
  setIsoPFCands(it, pfCands, bField);
}

edm::Ptr<L1Analysis::L1AnalysisPhaseIIEGTkIso::L1TTTrackType> L1Analysis::L1AnalysisPhaseIIEGTkIso::findMatchedTrack(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, double& matchedTrackDR, double& matchedTrackDEta, double& matchedTrackDPhi)
{
  int iTrack = 0;
  int iMatchedTrack = -1;
  for (L1TTTrackCollectionType::const_iterator trkIt = tttrack->begin(); trkIt != tttrack->end(); ++trkIt) {
    auto trkPt = trkIt->momentum().perp();
    if (useTwoStubsPt_) {
      trkPt = pTFrom2Stubs::pTFrom2(trkIt, tGeom);
    }

    if (trkPt > trkPtMin_ and trkIt->chi2() < trkChi2Max_) {
      // calculate dR to EG
      double dPhi = 999.;
      double dR = 999.;
      double dEta = 999.;
      edm::Ptr<L1TTTrackType> trackPtr(tttrack, iTrack);
      L1TkElectronTrackMatchAlgo::doMatch(egIt, trackPtr, dPhi, dR, dEta);
      if (selectMatchedTrack(dR, dPhi, dEta, trkPt, egIt->eta()) and dR < matchedTrackDR) {
        matchedTrackDR = dR;
        matchedTrackDEta = dEta;
        matchedTrackDPhi = dPhi;
        iMatchedTrack = iTrack;
      }
    }
    ++iTrack;
  }

  if (iMatchedTrack >= 0) {
    edm::Ptr<L1TTTrackType> trackPtr(tttrack, iMatchedTrack);
    return trackPtr;
  } else {
    edm::Ptr<L1TTTrackType> edmNullPtr;
    return edmNullPtr;
  }
}

// method to calculate isolation
void L1Analysis::L1AnalysisPhaseIIEGTkIso::setIsoTracks(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const int matchedTrackIdx)
{
  auto trkPtMinIso = trkPtMinIsoEB_;
  auto trkChi2MaxIso = trkChi2MaxIsoEB_;
  auto trkNStubMinIso = trkNStubMinIsoEB_;
  if (fabs(egIt->eta()) > 1.479) {
    trkPtMinIso = trkPtMinIsoHGC_;
    trkChi2MaxIso = trkChi2MaxIsoHGC_;
    trkNStubMinIso = trkNStubMinIsoHGC_;
  }

  int iTrack = -1;
  for (L1TTTrackCollectionType::const_iterator trkIt = tttrack->begin(); trkIt != tttrack->end(); ++trkIt) {
    if (++iTrack != matchedTrackIdx) { // Do not use the matched track in the isolation
      // track momentum calculation
      auto trkPt = trkIt->momentum().perp();
      if (useTwoStubsPt_) {
        trkPt = pTFrom2Stubs::pTFrom2(trkIt, tGeom);
      }

      // basic track selection
      if (trkIt->chi2() > trkChi2MaxIso
          or trkIt->getStubRefs().size() < trkNStubMinIso
          or trkPt < trkPtMinIso) {
        continue;
      }

      // calculate dR to EG
      double dPhi = 999.;
      double dR = 999.;
      double dEta = 999.;
      edm::Ptr<L1TTTrackType> trackPtr(tttrack, iTrack);
      L1TkElectronTrackMatchAlgo::doMatch(egIt, trackPtr, dPhi, dR, dEta);

      // store track info if close enough to the EG object
      if (dR < 999. and dR >= dRMinIso_ and dR < dRMaxIso_) {
        double dPhiTrkTrk = 0.;
        double dEtaTrkTrk = 0.;
        double dRTrkTrk =  0.;
        double dzTrkTrk = 0.;
        // calculate distance from matched track
        if (matchedTrackIdx >= 0) {
          edm::Ptr<L1TTTrackType> matchedTrackPtr(tttrack, matchedTrackIdx);
          const auto phi1 = static_cast<float>(trkIt->phi());
          const auto phi2 = static_cast<float>(matchedTrackPtr->phi());
          dPhiTrkTrk = reco::deltaPhi(phi1, phi2);
          dEtaTrkTrk = static_cast<float>(trkIt->eta() - matchedTrackPtr->eta());
          dRTrkTrk = std::sqrt(dPhiTrkTrk * dPhiTrkTrk + dEtaTrkTrk * dEtaTrkTrk);
          dzTrkTrk = fabs(trkIt->POCA().z() - matchedTrackPtr->POCA().z());
        }

        //std::cout << "Iso track: EG idx: " << l1Phase2EGTkIso_.nEG - 1 << ", pt: " << trkPt << ", eta: " << trkIt->eta() << ", phi: " << trkIt->phi() << ", dR: " << dR << ", dEta: " << dEta << ", dPhi: " << dPhi << ", dR tktk: " << dRTrkTrk << ", dEta tktk: " << dEtaTrkTrk << ", dPhi tktk: " << dPhiTrkTrk << ", dz tktk: " << dzTrkTrk << std::endl;
        l1Phase2EGTkIso_.isoTkEGIdx.push_back(l1Phase2EGTkIso_.nEG - 1);
        l1Phase2EGTkIso_.isoTkPt.push_back(trkPt);
        l1Phase2EGTkIso_.isoTkEta.push_back(trkIt->eta());
        l1Phase2EGTkIso_.isoTkPhi.push_back(trkIt->phi());
        l1Phase2EGTkIso_.isoTkRInv.push_back(trkIt->rInv());
        l1Phase2EGTkIso_.isoTkChi2.push_back(trkIt->chi2());
        l1Phase2EGTkIso_.isoTkDR.push_back(dR);
        l1Phase2EGTkIso_.isoTkDEta.push_back(dEta);
        l1Phase2EGTkIso_.isoTkDPhi.push_back(dPhi);
        l1Phase2EGTkIso_.isoTkMatchedTkDR.push_back(dRTrkTrk);
        l1Phase2EGTkIso_.isoTkMatchedTkDEta.push_back(dEtaTrkTrk);
        l1Phase2EGTkIso_.isoTkMatchedTkDPhi.push_back(dPhiTrkTrk);
        l1Phase2EGTkIso_.isoTkMatchedTkDz.push_back(dzTrkTrk);
      }
    }
  }
}


// method to calculate isolation
void L1Analysis::L1AnalysisPhaseIIEGTkIso::setIsoPFCands(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField) {
  auto egPos = L1TkElectronTrackMatchAlgo::calorimeterPosition(egIt->phi(), egIt->eta(), egIt->energy());
  for (size_t i = 0; i < pfCands->size(); ++i) {
    const auto pfCand = pfCands->at(i);
    const auto vtx = math::XYZTLorentzVector(pfCand.vx(), pfCand.vy(), pfCand.vz(), 0.);
    const auto charge = pfCand.charge();
    const auto etaPhiAtCalo = l1tpf::propagateToCalo(pfCand.p4(), vtx, charge, bField);

    // calculate dR to EG
    double dPhi = 999.;
    double dR = 999.;
    double dEta = 999.;

    dPhi = reco::deltaPhi(egPos.phi(), etaPhiAtCalo.second);
    dEta = egPos.eta() - etaPhiAtCalo.first;
    dR = reco::deltaR(egPos.eta(), egPos.phi(), etaPhiAtCalo.first, etaPhiAtCalo.second);

    // store PF cand info if close enough to the EG object
    if (dR < 999. and dR >= dRMinIso_ and dR < dRMaxIso_) {
      //const auto pfCluster = pfCand.pfCluster();
      //const auto pfTrack = pfCand.pfTrack();
      //if (egIt->hwQual() == 5) {
      //  std::cout << "Iso PF cand: EG idx: " << l1Phase2EGTkIso_.nEG - 1 << ", ID: " << pfCand.id() << ", Et: " << pfCand.pt() << ", eta: " << pfCand.eta() << ", phi: " << pfCand.phi() << ", eta at calo: " << etaPhiAtCalo.first << ", phi at calo: " << etaPhiAtCalo.second << ", dR: " << dR << ", dEta: " << dEta << ", dPhi: " << dPhi << ", charge: " << pfCand.charge() << ", vtx pos: " << pfCand.vx() << "," << pfCand.vy() << "," << pfCand.vz() << std::endl;
      //}
      l1Phase2EGTkIso_.isoPFEGIdx.push_back(l1Phase2EGTkIso_.nEG - 1);
      l1Phase2EGTkIso_.isoPFId.push_back(pfCand.id());
      l1Phase2EGTkIso_.isoPFEt.push_back(pfCand.pt());
      l1Phase2EGTkIso_.isoPFEta.push_back(pfCand.eta());
      l1Phase2EGTkIso_.isoPFPhi.push_back(pfCand.phi());
      l1Phase2EGTkIso_.isoPFPuppiWeight.push_back(pfCand.puppiWeight());
      l1Phase2EGTkIso_.isoPFEtaAtCalo.push_back(etaPhiAtCalo.first);
      l1Phase2EGTkIso_.isoPFPhiAtCalo.push_back(etaPhiAtCalo.second);
      l1Phase2EGTkIso_.isoPFDR.push_back(dR);
      l1Phase2EGTkIso_.isoPFDEta.push_back(dEta);
      l1Phase2EGTkIso_.isoPFDPhi.push_back(dPhi);
    }
  }
}


double L1Analysis::L1AnalysisPhaseIIEGTkIso::getPtScaledCut(const double pt, const std::vector<double>& parameters)
{
  return parameters[0] + parameters[1] * exp(parameters[2] * pt);
}

bool L1Analysis::L1AnalysisPhaseIIEGTkIso::selectMatchedTrack(const double dR, const double dPhi, const double dEta, const double trkPt, const float egEta) {
  if (trkEGMatchType_ == "PtDependentCut") {
    return (fabs(dPhi) < getPtScaledCut(trkPt, dPhiCutoff_) && dR < getPtScaledCut(trkPt, dRCutoff_));
  } else { // elliptical matching
    auto dEtaMax = dEtaCutoff_[0]; // EB high eta
    if (fabs(egEta) <= 0.9) {
      dEtaMax = dEtaCutoff_[1]; // EB low eta
    } else if (fabs(egEta) > 1.479) { // HGCal
      dEtaMax = dEtaCutoff_[2];
    }
    const auto dEtaRatio = dEta / dEtaMax;
    const auto dPhiRatio = dPhi / dPhiCutoff_[0];
    return (dEtaRatio * dEtaRatio + dPhiRatio * dPhiRatio < 1.);
  }
  return false;
}
