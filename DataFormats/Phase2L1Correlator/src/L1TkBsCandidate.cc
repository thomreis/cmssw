// -*- C++ -*-
//
// Package:     DataFormats/L1TrackTrigger
// Class  :     L1TkBsCandidate
//

#include "DataFormats/Phase2L1Correlator/interface/L1TkPhiCandidate.h"
#include "DataFormats/Phase2L1Correlator/interface/L1TkBsCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace l1t;

L1TkBsCandidate::L1TkBsCandidate() {}
L1TkBsCandidate::L1TkBsCandidate(const LorentzVector& p4, L1TkPhiCandidate cand1, L1TkPhiCandidate cand2)
    : L1Candidate(p4) {
  phiCandList_.push_back(cand1);
  phiCandList_.push_back(cand2);
}
// deltaR between the Phi pair
double L1TkBsCandidate::dRPhiPair() const {
  const LorentzVector& lva = phiCandidate(0).p4();
  const LorentzVector& lvb = phiCandidate(1).p4();
  return reco::deltaR(lva, lvb);
}
// position difference between track pair
double L1TkBsCandidate::dxyPhiPair() const {
  const L1TkPhiCandidate& phia = phiCandidate(0);
  const L1TkPhiCandidate& phib = phiCandidate(1);
  return std::sqrt(std::pow(phia.vx() - phib.vx(), 2) + std::pow(phia.vy() - phib.vy(), 2));
}
double L1TkBsCandidate::dzPhiPair() const { return (phiCandidate(0).vz() - phiCandidate(1).vz()); }
