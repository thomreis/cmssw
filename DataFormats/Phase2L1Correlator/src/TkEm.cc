// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     L1TkEmParticle
//

#include "DataFormats/Phase2L1Correlator/interface/L1TkEmParticle.h"

using namespace l1t;

L1TkEmParticle::L1TkEmParticle() {}

L1TkEmParticle::L1TkEmParticle(const LorentzVector& p4, const edm::Ref<EGammaBxCollection>& egRef, float tkisol)
    : L1Candidate(p4), egRef_(egRef), TrkIsol_(tkisol), TrkIsolPV_(-999) {}

L1TkEmParticle::L1TkEmParticle(const LorentzVector& p4,
                               const edm::Ref<EGammaBxCollection>& egRef,
                               float tkisol,
                               float tkisolPV)
    : L1Candidate(p4), egRef_(egRef), TrkIsol_(tkisol), TrkIsolPV_(tkisolPV) {}
