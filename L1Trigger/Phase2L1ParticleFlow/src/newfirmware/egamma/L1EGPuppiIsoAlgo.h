#ifndef L1Trigger_Phase2L1ParticleFlow_L1EGPuppiIsoAlgo_h
#define L1Trigger_Phase2L1ParticleFlow_L1EGPuppiIsoAlgo_h

#include <list>
#include <string>
#include <vector>
#include <memory>

#include "DataFormats/Math/interface/deltaR.h"

#ifdef CMSSW_GIT_HASH
#include "../dataformats/layer1_emulator.h"
#include "../dataformats/egamma.h"
#include "../dataformats/puppi.h"
#else
#include "../../../dataformats/layer1_emulator.h"
#include "../../../dataformats/egamma.h"
#include "../../../dataformats/puppi.h"
#endif

namespace l1ct {

struct L1EGPuppiIsoAlgoConfig {
  enum {kPFIso, kPuppiIso};

  int pfIsoType_;
  double pfMinPt_;
  double dRMinIso_;
  double dRMaxIso_;
  double dRMaxDz_;

  L1EGPuppiIsoAlgoConfig(const std::string pfIsoTypeStr,
                         const double pfMinPt,
                         const double dRMinIso,
                         const double dRMaxIso,
                         const double dRMaxDz)
    : pfIsoType_(pfIsoTypeStr == "PF" ? kPFIso : kPuppiIso),
      pfMinPt_(pfMinPt),
      dRMinIso_(dRMinIso),
      dRMaxIso_(dRMaxIso),
      dRMaxDz_(dRMaxDz) {}
};

typedef std::vector<EGIsoObjEmu> EGIsoObjsEmu;
typedef std::vector<PuppiObj> PuppiObjs;

class L1EGPuppiIsoAlgo {
 public:
  L1EGPuppiIsoAlgo(const L1EGPuppiIsoAlgoConfig& config) : config_(config) {}
  virtual ~L1EGPuppiIsoAlgo() = default;

  void run(const EGIsoObjsEmu& l1EGs, const PuppiObjs& l1PFCands, EGIsoObjsEmu& outL1EGs) const;

 private:
  float calcIso(const EGIsoObj& l1EG, std::list<std::shared_ptr<PuppiObj>>& workPFCands, float z0 = 0.) const;

  const L1EGPuppiIsoAlgoConfig config_;
};

void L1EGPuppiIsoAlgo::run(const EGIsoObjsEmu& l1EGs, const PuppiObjs& l1PFCands, EGIsoObjsEmu& outL1EGs) const {
  outL1EGs.reserve(l1EGs.size());

  // make a list of pointers to PF candidates
  // the pointer will be removed from the list once the candidate has been used
  std::list<std::shared_ptr<PuppiObj>> workPFCands;
  std::list<std::shared_ptr<PuppiObj>> workPFCandsPV;
  for (unsigned i = 0; i < l1PFCands.size(); ++i) {
    workPFCands.emplace_back(std::make_shared<PuppiObj>(l1PFCands[i]));
    workPFCandsPV.emplace_back(std::make_shared<PuppiObj>(l1PFCands[i]));
  }

  for (const auto &l1EG : l1EGs) {
    auto outL1EG(l1EG);
    float iso = 0.;
    float isoPV = 0.;
    if (!workPFCands.empty()) {
      iso = calcIso(l1EG, workPFCands);
      isoPV = calcIso(l1EG, workPFCandsPV); // FIXME calculate PV constrained iso
    }

    if (config_.pfIsoType_ == L1EGPuppiIsoAlgoConfig::kPFIso) {
      outL1EG.setHwIso(EGIsoObjEmu::IsoType::PfIso, Scales::makeIso(iso));
      outL1EG.setHwIso(EGIsoObjEmu::IsoType::PfIsoPV, Scales::makeIso(isoPV));
    } else {
      outL1EG.setHwIso(EGIsoObjEmu::IsoType::PfIso, Scales::makeIso(iso)); // Use PF iso type for lack of a dedicated PUPPI one for now
      outL1EG.setHwIso(EGIsoObjEmu::IsoType::PfIsoPV, Scales::makeIso(isoPV));
    }
    outL1EGs.emplace_back(outL1EG);
  }
}

float L1EGPuppiIsoAlgo::calcIso(const EGIsoObj& l1EG, std::list<std::shared_ptr<PuppiObj>>& workPFCands, float z0) const {
  float sumPt = 0.;

  auto pfIt = workPFCands.cbegin();
  while (pfIt != workPFCands.cend()) {
    // use the PF candidate pT if it is within the cone and optional dz cut for charged PF candidates
    // and then remove the candidate from the collection
    const auto workPFCand = *pfIt;
    float pfCandZ0 = 0.;
    if (workPFCand->hwId.charged()) {
      pfCandZ0 = workPFCand->floatZ0();
    }
    const auto dz = std::abs(z0 - pfCandZ0);
    if (workPFCand->intCharge() == 0 || (workPFCand->intCharge() != 0 && dz < config_.dRMaxDz_)) {
      const auto dR = reco::deltaR(l1EG.floatEta(), l1EG.floatPhi(), workPFCand->floatEta(), workPFCand->floatPhi());
      if (dR >= config_.dRMinIso_ && dR < config_.dRMaxIso_ && workPFCand->floatPt() >= config_.pfMinPt_) {
        sumPt += workPFCand->floatPt();
        pfIt = workPFCands.erase(pfIt); // this returns an iterator to the next element already so no need to increase here
      } else {
        ++pfIt;
      }
    } else {
      ++pfIt;
    }
  }

  return sumPt / l1EG.floatPt();
}

}  // namespace l1ct

#endif

