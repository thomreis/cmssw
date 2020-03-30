// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     L1TkEmParticle
//

#include "DataFormats/Phase2L1Correlator/interface/L1TkTauParticle.h"

using namespace l1t;

L1TkTauParticle::L1TkTauParticle() {}

L1TkTauParticle::L1TkTauParticle(const LorentzVector& p4,
                                 const edm::Ref<TauBxCollection>& tauCaloRef,
                                 const edm::Ptr<L1TTTrackType>& trackPtr,
                                 const edm::Ptr<L1TTTrackType>& trackPtr2,
                                 const edm::Ptr<L1TTTrackType>& trackPtr3,
                                 float tkisol)
    : L1Candidate(p4),
      tauCaloRef_(tauCaloRef),
      trkPtr_(trackPtr),
      trkPtr2_(trackPtr2),
      trkPtr3_(trackPtr3),
      TrkIsol_(tkisol)

{
  if (trkPtr_.isNonnull()) {
    float z = trkPtr()->POCA().z();
    setTrkzVtx(z);
  }
}
