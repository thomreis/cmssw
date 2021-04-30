#include <string>
#include <vector>

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

template <typename T>
class L1TEGPuppiIsoProducer : public edm::stream::EDProducer<> {
public:
  explicit L1TEGPuppiIsoProducer(const edm::ParameterSet &);
  ~L1TEGPuppiIsoProducer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  enum {kPFIso, kPuppiIso};

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

  float calcIso(const T& l1EG, std::list<edm::Ptr<l1t::PFCandidate>>& workPFCands, bool usePVConstraint = false);

  edm::EDGetTokenT<std::vector<T>> emToken_;
  edm::EDGetTokenT<std::vector<l1t::PFCandidate>> pfCandToken_;

  int pfIsoType_;
  const std::string outInstanceName_;
  const double pfMinPt_;
  const double dRMinIso_;
  const double dRMaxIso_;
  const double dRMaxDz_;
};

template <typename T>
L1TEGPuppiIsoProducer<T>::L1TEGPuppiIsoProducer(const edm::ParameterSet& iConfig)
    : emToken_(consumes<std::vector<T>>(iConfig.getParameter<edm::InputTag>("l1TkEmCollection"))),
      pfCandToken_(consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PFCandidates"))),
      outInstanceName_(iConfig.getParameter<std::string>("outInstanceName")),
      pfMinPt_(iConfig.getParameter<double>("pfMinPt")),
      dRMinIso_(iConfig.getParameter<double>("dRMinIso")),
      dRMaxIso_(iConfig.getParameter<double>("dRMaxIso")),
      dRMaxDz_(iConfig.getParameter<double>("dRMaxDz"))
{
  if (iConfig.getParameter<std::string>("pfIsoType") == "PF") {
    pfIsoType_ = kPFIso;
  } else {
    pfIsoType_ = kPuppiIso;
  }

  produces<std::vector<T>>(outInstanceName_);
}

template <typename T>
void L1TEGPuppiIsoProducer<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  auto outL1EGs = std::make_unique<std::vector<T>>();

  edm::Handle<std::vector<T>> l1EGs;
  iEvent.getByToken(emToken_, l1EGs);

  edm::Handle<l1t::PFCandidateCollection> l1PFCands;
  iEvent.getByToken(pfCandToken_, l1PFCands);

  outL1EGs->reserve(l1EGs->size());

  // make a list of pointers to PF candidates
  // the pointer will be removed from the list once the candidate has been used
  std::list<edm::Ptr<l1t::PFCandidate>> workPFCands;
  std::list<edm::Ptr<l1t::PFCandidate>> workPFCandsPV;
  for (unsigned i = 0; i < (*l1PFCands).size(); ++i) {
    workPFCands.emplace_back(edm::Ptr<l1t::PFCandidate>(l1PFCands, i));
    workPFCandsPV.emplace_back(edm::Ptr<l1t::PFCandidate>(l1PFCands, i));
  }

  for (const auto &l1EG : *l1EGs) {
    auto outL1EG(l1EG);
    float iso = 0.;
    float isoPV = 0.;
    if (!workPFCands.empty()) {
      //std::cout << "calcIso" << std::endl;
      iso = calcIso(l1EG, workPFCands);
      //std::cout << "L1 EG: pfIsol " << l1EG.pfIsol() << ", iso " << iso << std::endl;
      //std::cout << "calcIso PV" << std::endl;
      isoPV = calcIso(l1EG, workPFCandsPV, true);
      //std::cout << "L1 EG: pfIsolPV " << l1EG.pfIsolPV() << ", isoPV " << isoPV << std::endl;
    }

    if (pfIsoType_ == kPFIso) {
      outL1EG.setPFIsol(iso);
      outL1EG.setPFIsolPV(isoPV);
    } else {
      outL1EG.setPuppiIsol(iso);
      outL1EG.setPuppiIsolPV(isoPV);
    }
    outL1EGs->emplace_back(outL1EG);
  }

  iEvent.put(std::move(outL1EGs), outInstanceName_);
}

template <typename T>
void L1TEGPuppiIsoProducer<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("l1TkEmCollection", edm::InputTag("l1TkEmCollection"))->setComment("TkEm or TkElectron collection");
  desc.add<edm::InputTag>("l1PFCandidates", edm::InputTag("l1PFCandidates"))->setComment("L1 PF or PUPPI collection");
  desc.add<std::string>("pfIsoType", "PUPPI")->setComment("Type of particle flow isolation (PF or PUPPI)");
  desc.add<std::string>("outInstanceName", "")->setComment("Output collection instance name");
  desc.add<double>("pfMinPt", 0.)->setComment("Minimum pT for PF candidates to be considered in isolation");
  desc.add<double>("dRMinIso", 0.03)->setComment("Inner cone size for isolation");
  desc.add<double>("dRMaxIso", 0.2)->setComment("Outer cone size for isolation");
  desc.add<double>("dRMaxDz", 0.6)->setComment("Maximum distance to PV in cm for PF candidate to be considered in isolation");
  descriptions.addWithDefaultLabel(desc);
}

template <typename T>
float L1TEGPuppiIsoProducer<T>::calcIso(const T& l1EG, std::list<edm::Ptr<l1t::PFCandidate>>& workPFCands, bool usePVConstraint) {
  float sumPt = 0.;

  //std::cout << "L1 EG: pt=" << l1EG.pt() << ", eta=" << l1EG.eta() << ", phi=" << l1EG.phi() << ", pfIsol=" << l1EG.pfIsol() << ", pfIsolPV=" << l1EG.pfIsolPV() << std::endl;
  auto pfIt = workPFCands.cbegin();
  while (pfIt != workPFCands.cend()) {
    // use the PF candidate pT if it is within the cone and optional dz cut for charged PF candidates
    // and then remove the candidate from the collection
    const auto workPFCand = *pfIt;
    const auto dz = std::abs(l1EG.vz() - workPFCand->z0());
    //std::cout << "PF candidate: pt=" << workPFCand->pt() << ", eta=" << workPFCand->eta() << ", phi=" << workPFCand->phi() << ", dz=" << dz;
    if (!usePVConstraint || workPFCand->charge() == 0 || (workPFCand->charge() != 0 && dz < dRMaxDz_)) {
      const auto dR = reco::deltaR(l1EG.eta(), l1EG.phi(), workPFCand->eta(), workPFCand->phi());
      //std::cout << ", dR=" << dR;
      if (dR >= dRMinIso_ && dR < dRMaxIso_ && workPFCand->pt() >= pfMinPt_) {
        sumPt += workPFCand->pt();
        pfIt = workPFCands.erase(pfIt); // this returns an iterator to the next element already so no need to increase here
        //std::cout << ", match!";
      } else {
        //std::cout << ", fail dR cut";
        ++pfIt;
      }
    } else {
      //std::cout << ", fail dz cut";
      ++pfIt;
    }
    //std::cout << std::endl;
  }

  return sumPt / l1EG.pt();
}

#include "DataFormats/L1TCorrelator/interface/TkEm.h"
#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
typedef L1TEGPuppiIsoProducer<l1t::TkEm> L1TTkEmPuppiIsoProducer;
typedef L1TEGPuppiIsoProducer<l1t::TkElectron> L1TTkElectronPuppiIsoProducer;
DEFINE_FWK_MODULE(L1TTkEmPuppiIsoProducer);
DEFINE_FWK_MODULE(L1TTkElectronPuppiIsoProducer);

