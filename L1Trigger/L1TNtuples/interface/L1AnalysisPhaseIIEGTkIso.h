#ifndef __L1Analysis_L1AnalysisPhaseIIEGTkIso_H__
#define __L1Analysis_L1AnalysisPhaseIIEGTkIso_H__

//-------------------------------------------------------------------------------
// Original Author:  Thomas Reis
//         Created:  Wed, 11 Sep 2019 16:33 GMT 
//-------------------------------------------------------------------------------

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"

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

    void SetEGWithTracks (const edm::Handle<l1t::EGammaBxCollection>& eg, const edm::Handle<l1t::EGammaBxCollection>& egHGC, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField);
    void SetPFCands (const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField);

    inline L1AnalysisPhaseIIEGTkIsoDataFormat* GetData() { return &l1Phase2EGTkIso_; }

  private:
    void setBranches(const l1t::EGammaBxCollection::const_iterator& it, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField, const int bx, const bool isHGC);
    edm::Ptr<L1TTTrackType> findMatchedTrack(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, double& matchedTrackDR, double& matchedTrackDEta, double& matchedTrackDPhi);
    void setIsoTracks(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<L1TTTrackCollectionType>& tttrack, const TrackerGeometry* tGeom, const int matchedTrackIdx);
    void setIsoPFCands(const l1t::EGammaBxCollection::const_iterator& egIt, const edm::Handle<vector<l1t::PFCandidate>> &pfCands, const float bField);
    double getPtScaledCut(const double pt, const std::vector<double>& parameters);
    bool selectMatchedTrack(const double dR, const double dPhi, const double dEta, const double trkPt, const float egEta);

    L1AnalysisPhaseIIEGTkIsoDataFormat l1Phase2EGTkIso_;

    edm::ParameterSet pSet_;
    float egBarrelEtMin_;
    float egHGCEtMin_;

    float trkPtMin_;
    float trkChi2Max_;
    bool useTwoStubsPt_;
    std::string trkEGMatchType_;
    std::vector<double> dEtaCutoff_;
    std::vector<double> dPhiCutoff_;
    std::vector<double> dRCutoff_;

    float trkPtMinIsoEB_;
    float trkPtMinIsoHGC_;
    float trkChi2MaxIsoEB_;
    float trkChi2MaxIsoHGC_;
    unsigned int trkNStubMinIsoEB_;
    unsigned int trkNStubMinIsoHGC_;
    float dRMinIso_;
    float dRMaxIso_;
  }; 
}
#endif

