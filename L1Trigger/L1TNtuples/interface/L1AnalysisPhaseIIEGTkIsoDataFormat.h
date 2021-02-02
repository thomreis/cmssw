#ifndef __L1Analysis_L1AnalysisPhaseIIEGTkIsoDataFormat_H__
#define __L1Analysis_L1AnalysisPhaseIIEGTkIsoDataFormat_H__

//-------------------------------------------------------------------------------
// Original Author:  Thomas Reis
//         Created:  Wed, 11 Sep 2019 16:33 GMT 
//-------------------------------------------------------------------------------

#include <vector>

namespace L1Analysis
{
  struct L1AnalysisPhaseIIEGTkIsoDataFormat
  {
    L1AnalysisPhaseIIEGTkIsoDataFormat() { Reset(); };
    ~L1AnalysisPhaseIIEGTkIsoDataFormat() {};

    void Reset()
    {
      nEG = 0;
      EGEt.clear();
      EGEta.clear();
      EGPhi.clear();
      EGBx.clear();
      EGIso.clear();
      EGzVtx.clear();
      EGHwQual.clear();      
      EGHGC.clear();
      EGPassesLooseTrackID.clear();
      EGPassesPhotonID.clear();
      EGHasMatchedTrack.clear();

      matchedTkEGIdx.clear();
      matchedTkPt.clear();
      matchedTkEta.clear();
      matchedTkPhi.clear();
      matchedTkRInv.clear();
      matchedTkChi2.clear();
      matchedTkDR.clear();
      matchedTkDEta.clear();
      matchedTkDPhi.clear();

      isoTkEGIdx.clear();
      isoTkPt.clear();
      isoTkEta.clear();
      isoTkPhi.clear();
      isoTkRInv.clear();
      isoTkChi2.clear();
      isoTkDR.clear();
      isoTkDEta.clear();
      isoTkDPhi.clear();
      isoTkMatchedTkDR.clear();
      isoTkMatchedTkDEta.clear();
      isoTkMatchedTkDPhi.clear();
      isoTkMatchedTkDz.clear();

      isoPFEGIdx.clear();
      isoPFId.clear();
      isoPFEt.clear();
      isoPFEta.clear();
      isoPFPhi.clear();
      isoPFPuppiWeight.clear();
      isoPFEtaAtCalo.clear();
      isoPFPhiAtCalo.clear();
      isoPFDR.clear();
      isoPFDEta.clear();
      isoPFDPhi.clear();

      pfId.clear();
      pfEt.clear();
      pfEta.clear();
      pfPhi.clear();
      pfPuppiWeight.clear();
      pfEtaAtCalo.clear();
      pfPhiAtCalo.clear();
    }
 
    unsigned int nEG;
    std::vector<double> EGEt;
    std::vector<double> EGEta;
    std::vector<double> EGPhi;
    std::vector<int> EGBx;
    std::vector<double> EGIso;
    std::vector<double> EGzVtx;
    std::vector<int> EGHwQual;
    std::vector<unsigned int> EGHGC;
    std::vector<unsigned int> EGPassesLooseTrackID;
    std::vector<unsigned int> EGPassesPhotonID;
    std::vector<int> EGHasMatchedTrack;

    std::vector<unsigned int> matchedTkEGIdx;
    std::vector<double> matchedTkPt;
    std::vector<double> matchedTkEta;
    std::vector<double> matchedTkPhi;
    std::vector<double> matchedTkRInv;
    std::vector<double> matchedTkChi2;
    std::vector<double> matchedTkDR;
    std::vector<double> matchedTkDEta;
    std::vector<double> matchedTkDPhi;

    std::vector<unsigned int> isoTkEGIdx;
    std::vector<double> isoTkPt;
    std::vector<double> isoTkEta;
    std::vector<double> isoTkPhi;
    std::vector<double> isoTkRInv;
    std::vector<double> isoTkChi2;
    std::vector<double> isoTkDR;
    std::vector<double> isoTkDEta;
    std::vector<double> isoTkDPhi;
    std::vector<double> isoTkMatchedTkDR;
    std::vector<double> isoTkMatchedTkDEta;
    std::vector<double> isoTkMatchedTkDPhi;
    std::vector<double> isoTkMatchedTkDz;

    std::vector<unsigned int> isoPFEGIdx;
    std::vector<unsigned int> isoPFId;
    std::vector<double> isoPFEt;
    std::vector<double> isoPFEta;
    std::vector<double> isoPFPhi;
    std::vector<double> isoPFPuppiWeight;
    std::vector<double> isoPFEtaAtCalo;
    std::vector<double> isoPFPhiAtCalo;
    std::vector<double> isoPFDR;
    std::vector<double> isoPFDEta;
    std::vector<double> isoPFDPhi;

    std::vector<unsigned int> pfId;
    std::vector<double> pfEt;
    std::vector<double> pfEta;
    std::vector<double> pfPhi;
    std::vector<double> pfPuppiWeight;
    std::vector<double> pfEtaAtCalo;
    std::vector<double> pfPhiAtCalo;
  };
}
#endif

