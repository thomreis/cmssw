// -*- C++ -*-
//
// Package:     DataFormats/L1TrackTrigger
// Class  :     L1TkPhiCandidate
//
#include "DataFormats/Phase2L1Correlator/interface/L1TkPhiCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace l1t;

L1TkPhiCandidate::L1TkPhiCandidate() {}
L1TkPhiCandidate::L1TkPhiCandidate(const LorentzVector& p4,
                                   const edm::Ptr<L1TTTrackType>& trkPtr1,
                                   const edm::Ptr<L1TTTrackType>& trkPtr2)
    : L1Candidate(p4) {
  trkPtrList_.push_back(trkPtr1);
  trkPtrList_.push_back(trkPtr2);
}
// deltaR between track pair
double L1TkPhiCandidate::dRTrkPair() const {
  const edm::Ptr<L1TTTrackType>& itrk = getTrkPtr(0);
  const edm::Ptr<L1TTTrackType>& jtrk = getTrkPtr(1);

  math::PtEtaPhiMLorentzVector itrkP4(
      itrk->momentum().perp(), itrk->momentum().eta(), itrk->momentum().phi(), kmass);
  math::PtEtaPhiMLorentzVector jtrkP4(
      jtrk->momentum().perp(), jtrk->momentum().eta(), jtrk->momentum().phi(), kmass);
  return reco::deltaR(itrkP4, jtrkP4);
}

// difference from nominal mass
double L1TkPhiCandidate::dmass() const { return std::fabs(phi_polemass - mass()); }
// position difference between track pair
double L1TkPhiCandidate::dxyTrkPair() const {
  const edm::Ptr<L1TTTrackType>& itrk = getTrkPtr(0);
  const edm::Ptr<L1TTTrackType>& jtrk = getTrkPtr(1);

  return std::sqrt(std::pow(itrk->POCA().x() - jtrk->POCA().x(), 2) +
                   std::pow(itrk->POCA().y() - jtrk->POCA().y(), 2));
}
double L1TkPhiCandidate::dzTrkPair() const { return (getTrkPtr(0)->POCA().z() - getTrkPtr(1)->POCA().z()); }
double L1TkPhiCandidate::vx() const { return 0.5 * (getTrkPtr(0)->POCA().x() + getTrkPtr(1)->POCA().x()); }
double L1TkPhiCandidate::vy() const { return 0.5 * (getTrkPtr(0)->POCA().y() + getTrkPtr(1)->POCA().y()); }
double L1TkPhiCandidate::vz() const { return 0.5 * (getTrkPtr(0)->POCA().z() + getTrkPtr(1)->POCA().z()); }
