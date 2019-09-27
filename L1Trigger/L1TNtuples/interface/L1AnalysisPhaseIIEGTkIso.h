#ifndef __L1Analysis_L1AnalysisPhaseIIEGThIso_H__
#define __L1Analysis_L1AnalysisPhaseIIEGTkIso_H__

//-------------------------------------------------------------------------------
// Original Author:  Thomas Reis
//         Created:  Wed, 11 Sep 2019 16:33 GMT 
//-------------------------------------------------------------------------------

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIEGTkIsoDataFormat.h"

namespace L1Analysis {
  class L1AnalysisPhaseIIEGTkIso {
  public:
    typedef TTTrack<Ref_Phase2TrackerDigi_> L1TTTrackType;
    typedef std::vector<L1TTTrackType> L1TTTrackCollectionType;

    L1AnalysisPhaseIIEGTkIso(const edm::ParameterSet& pSet);
    ~L1AnalysisPhaseIIEGTkIso() {};

    inline void Reset() { l1Phase2EGTkIso_.Reset(); }

    void SetEGWithTracks (const edm::Handle<l1t::EGammaBxCollection>& eg, const edm::Handle<l1t::EGammaBxCollection>& egHGC, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom);

    inline L1AnalysisPhaseIIEGTkIsoDataFormat* GetData() { return &l1Phase2EGTkIso_; }

  private:
    void setBranches(const l1t::EGammaBxCollection::const_iterator& it, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const int bx, const bool isHGC);
    edm::Ptr<L1TTTrackType> findMatchedTrack(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, double& matchedTrackDR);
    void setIsoTracks(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const int matchedTrackIdx);
    double getPtScaledCut(const double pt, const std::vector<double>& parameters);

    L1AnalysisPhaseIIEGTkIsoDataFormat l1Phase2EGTkIso_;

    edm::ParameterSet pSet_;
    float egBarrelEtMin_;
    float egHGCEtMin_;

    float trkPtMin_;
    float trkChi2Max_;
    bool useTwoStubsPt_;
    std::vector<double> dPhiCutoff_;
    std::vector<double> dRCutoff_;

    float trkPtMinIso_;
    float trkChi2MaxIso_;
    float dRMinIso_;
    float dRMaxIso_;
  }; 
}
#endif

