// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     TkEGTau
//

#include "DataFormats/Phase2L1Correlator/interface/TkEGTau.h"

using namespace l1t;

TkEGTau::TkEGTau() {}

TkEGTau::TkEGTau(const LorentzVector& p4,
                                     const std::vector<L1TTTrackRefPtr>& clustTracks,
                                     const std::vector<EGammaRef>& clustEGs,
                                     float iso)
    : L1Candidate(p4), clustTracks_(clustTracks), clustEGs_(clustEGs), iso_(iso) {}
