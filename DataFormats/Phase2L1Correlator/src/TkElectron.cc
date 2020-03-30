// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     L1TkEmParticle
//

#include "DataFormats/Phase2L1Correlator/interface/L1TkElectronParticle.h"

using namespace l1t;

L1TkElectronParticle::L1TkElectronParticle() {}

L1TkElectronParticle::L1TkElectronParticle(const LorentzVector& p4,
                                           const edm::Ref<EGammaBxCollection>& egRef,
                                           const edm::Ptr<L1TTTrackType>& trackPtr,
                                           float tkisol)
    : L1TkEmParticle(p4, egRef, tkisol, -999),
      trkPtr_(trackPtr)

{
  if (trkPtr_.isNonnull()) {
    float z = trkPtr()->POCA().z();
    setTrkzVtx(z);
  }
}
