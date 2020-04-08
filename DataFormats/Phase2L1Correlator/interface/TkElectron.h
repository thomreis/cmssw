#ifndef TkTrigger_L1Electron_h
#define TkTrigger_L1Electron_h

// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     TkEm
//

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"

#include "DataFormats/Phase2L1Correlator/interface/TkEm.h"
#include "DataFormats/Phase2L1Correlator/interface/TkEmFwd.h"

#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

namespace l1t {

  class TkElectron : public TkEm {
  public:
    typedef TTTrack<Ref_Phase2TrackerDigi_> L1TTTrackType;
    typedef std::vector<L1TTTrackType> L1TTTrackCollection;

    TkElectron();

    TkElectron(const LorentzVector& p4,
                         const edm::Ref<EGammaBxCollection>& egRef,
                         const edm::Ptr<L1TTTrackType>& trkPtr,
                         float tkisol = -999.);

    virtual ~TkElectron() {}

    // ---------- const member functions ---------------------

    const edm::Ptr<L1TTTrackType>& trkPtr() const { return trkPtr_; }

    float trkzVtx() const { return TrkzVtx_; }
    double trackCurvature() const { return trackCurvature_; }

    // ---------- member functions ---------------------------

    void setTrkzVtx(float TrkzVtx) { TrkzVtx_ = TrkzVtx; }
    void setTrackCurvature(double trackCurvature) { trackCurvature_ = trackCurvature; }

  private:
    edm::Ptr<L1TTTrackType> trkPtr_;
    float TrkzVtx_;
    double trackCurvature_;
  };
}  // namespace l1t
#endif
