#ifndef L1Trigger_Phase2L1ParticleFlow_L1EGPuppiIsoAlgo_h
#define L1Trigger_Phase2L1ParticleFlow_L1EGPuppiIsoAlgo_h

#include <list>
#include <string>
#include <vector>

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

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

template <typename T>
class L1EGPuppiIsoAlgo {
 public:
  L1EGPuppiIsoAlgo(const L1EGPuppiIsoAlgoConfig& config) : config_(config) {}
  virtual ~L1EGPuppiIsoAlgo() = default;

  void run(const std::vector<T>& l1EGs, const l1t::PFCandidateCollection& l1PFCands, std::vector<T>& outL1EGs) const;

 private:
  float calcIso(const T& l1EG, std::list<edm::Ptr<l1t::PFCandidate>>& workPFCands, bool usePVConstraint = false) const;

  const L1EGPuppiIsoAlgoConfig config_;
};

template <typename T>
void L1EGPuppiIsoAlgo<T>::run(const std::vector<T>& l1EGs, const l1t::PFCandidateCollection& l1PFCands, std::vector<T>& outL1EGs) const {
  outL1EGs.reserve(l1EGs.size());

  // make a list of pointers to PF candidates
  // the pointer will be removed from the list once the candidate has been used
  std::list<edm::Ptr<l1t::PFCandidate>> workPFCands;
  std::list<edm::Ptr<l1t::PFCandidate>> workPFCandsPV;
  for (unsigned i = 0; i < l1PFCands.size(); ++i) {
    workPFCands.emplace_back(edm::Ptr<l1t::PFCandidate>(&l1PFCands, i));
    workPFCandsPV.emplace_back(edm::Ptr<l1t::PFCandidate>(&l1PFCands, i));
  }

  for (const auto &l1EG : l1EGs) {
    auto outL1EG(l1EG);
    float iso = 0.;
    float isoPV = 0.;
    if (!workPFCands.empty()) {
      iso = calcIso(l1EG, workPFCands);
      isoPV = calcIso(l1EG, workPFCandsPV, true);
    }

    if (config_.pfIsoType_ == L1EGPuppiIsoAlgoConfig::kPFIso) {
      outL1EG.setPFIsol(iso);
      outL1EG.setPFIsolPV(isoPV);
    } else {
      outL1EG.setPuppiIsol(iso);
      outL1EG.setPuppiIsolPV(isoPV);
    }
    outL1EGs.emplace_back(outL1EG);
  }
}

template <typename T>
float L1EGPuppiIsoAlgo<T>::calcIso(const T& l1EG, std::list<edm::Ptr<l1t::PFCandidate>>& workPFCands, bool usePVConstraint) const {
  float sumPt = 0.;

  auto pfIt = workPFCands.cbegin();
  while (pfIt != workPFCands.cend()) {
    // use the PF candidate pT if it is within the cone and optional dz cut for charged PF candidates
    // and then remove the candidate from the collection
    const auto workPFCand = *pfIt;
    const auto dz = std::abs(l1EG.vz() - workPFCand->z0());
    if (!usePVConstraint || workPFCand->charge() == 0 || (workPFCand->charge() != 0 && dz < config_.dRMaxDz_)) {
      const auto dR = reco::deltaR(l1EG.eta(), l1EG.phi(), workPFCand->eta(), workPFCand->phi());
      if (dR >= config_.dRMinIso_ && dR < config_.dRMaxIso_ && workPFCand->pt() >= config_.pfMinPt_) {
        sumPt += workPFCand->pt();
        pfIt = workPFCands.erase(pfIt); // this returns an iterator to the next element already so no need to increase here
      } else {
        ++pfIt;
      }
    } else {
      ++pfIt;
    }
  }

  return sumPt / l1EG.pt();
}
}  // namespace l1ct

#endif

