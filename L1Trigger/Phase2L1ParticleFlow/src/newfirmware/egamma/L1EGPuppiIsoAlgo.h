#ifndef L1Trigger_Phase2L1ParticleFlow_L1EGPuppiIsoAlgo_h
#define L1Trigger_Phase2L1ParticleFlow_L1EGPuppiIsoAlgo_h

#include <list>
#include <string>
#include <vector>
#include <memory>

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#ifdef CMSSW_GIT_HASH
#include "../dataformats/datatypes.h"
#include "../dataformats/layer1_emulator.h"
#include "../dataformats/egamma.h"
#include "../dataformats/puppi.h"
#else
#include "../../../dataformats/datatypes.h"
#include "../../../dataformats/layer1_emulator.h"
#include "../../../dataformats/egamma.h"
#include "../../../dataformats/puppi.h"
#endif

namespace l1ct {

  struct L1EGPuppiIsoAlgoConfig {
    enum { kPFIso, kPuppiIso };

    int pfIsoType_;
    pt_t ptMin_;
    ap_int<z0_t::width + 1> dZMax_;
    int dRMin2_;
    int dRMax2_;

    L1EGPuppiIsoAlgoConfig(const std::string &pfIsoTypeStr,
                           const float ptMin,
                           const float dZMax,
                           const float dRMin,
                           const float dRMax)
        : pfIsoType_(pfIsoTypeStr == "PF" ? kPFIso : kPuppiIso),
          ptMin_(Scales::makePtFromFloat(ptMin)),
          dZMax_(Scales::makeZ0(dZMax)),
          dRMin2_(Scales::makeDR2FromFloatDR(dRMin)),
          dRMax2_(Scales::makeDR2FromFloatDR(dRMax)) {}
  };

  typedef std::vector<EGIsoObjEmu> EGIsoObjsEmu;
  typedef std::vector<EGIsoEleObjEmu> EGIsoEleObjsEmu;
  typedef std::vector<PuppiObj> PuppiObjs;

  class L1EGPuppiIsoAlgo {
  public:
    L1EGPuppiIsoAlgo(const L1EGPuppiIsoAlgoConfig& config) : config_(config) {}
    L1EGPuppiIsoAlgo(const edm::ParameterSet& pSet);
    virtual ~L1EGPuppiIsoAlgo() = default;

    void run(const EGIsoObjsEmu& l1EGs, const PuppiObjs& l1PFCands, EGIsoObjsEmu& outL1EGs, z0_t z0 = 0) const;
    void run(EGIsoObjsEmu& l1EGs, const PuppiObjs& l1PFCands, z0_t z0 = 0) const;
    void run(EGIsoEleObjsEmu& l1Eles, const PuppiObjs& l1PFCands) const;

  private:
    iso_t calcIso(const EGIsoObj& l1EG, std::list<std::shared_ptr<PuppiObj>>& workPFCands, z0_t z0 = 0) const;

    const L1EGPuppiIsoAlgoConfig config_;
  };

}  // namespace l1ct
#endif
