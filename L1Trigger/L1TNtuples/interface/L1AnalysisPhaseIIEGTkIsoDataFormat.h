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

      matchedTkEGIdx.clear();
      matchedTkPt.clear();
      matchedTkEta.clear();
      matchedTkPhi.clear();
      matchedTkDR.clear();

      isoTkEGIdx.clear();
      isoTkPt.clear();
      isoTkEta.clear();
      isoTkPhi.clear();
      isoTkDR.clear();
      isoTkDEta.clear();
      isoTkDPhi.clear();
      isoTkMatchedTkDR.clear();
      isoTkMatchedTkDEta.clear();
      isoTkMatchedTkDPhi.clear();
      isoTkMatchedTkDz.clear();
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

    std::vector<unsigned int> matchedTkEGIdx;
    std::vector<double> matchedTkPt;
    std::vector<double> matchedTkEta;
    std::vector<double> matchedTkPhi;
    std::vector<double> matchedTkDR;

    std::vector<unsigned int> isoTkEGIdx;
    std::vector<double> isoTkPt;
    std::vector<double> isoTkEta;
    std::vector<double> isoTkPhi;
    std::vector<double> isoTkDR;
    std::vector<double> isoTkDEta;
    std::vector<double> isoTkDPhi;
    std::vector<double> isoTkMatchedTkDR;
    std::vector<double> isoTkMatchedTkDEta;
    std::vector<double> isoTkMatchedTkDPhi;
    std::vector<double> isoTkMatchedTkDz;
  }; 
}
#endif

