// -*- C++ -*-
//
// Package:     DataFormats/L1TrackTrigger
// Class  :     TkBsCandidate
//

#include "DataFormats/Phase2L1Correlator/interface/TkPhiCandidate.h"
#include "DataFormats/Phase2L1Correlator/interface/TkBsCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace l1t;

TkBsCandidate::TkBsCandidate() {}
TkBsCandidate::TkBsCandidate(const LorentzVector& p4, TkPhiCandidate cand1, TkPhiCandidate cand2)
    : L1Candidate(p4) {
  phiCandList_.push_back(cand1);
  phiCandList_.push_back(cand2);
}
// deltaR between the Phi pair
double TkBsCandidate::dRPhiPair() const {
  const LorentzVector& lva = phiCandidate(0).p4();
  const LorentzVector& lvb = phiCandidate(1).p4();
  return reco::deltaR(lva, lvb);
}
// position difference between track pair
double TkBsCandidate::dxyPhiPair() const {
  const TkPhiCandidate& phia = phiCandidate(0);
  const TkPhiCandidate& phib = phiCandidate(1);
  return std::sqrt(std::pow(phia.vx() - phib.vx(), 2) + std::pow(phia.vy() - phib.vy(), 2));
}
double TkBsCandidate::dzPhiPair() const { return (phiCandidate(0).vz() - phiCandidate(1).vz()); }
