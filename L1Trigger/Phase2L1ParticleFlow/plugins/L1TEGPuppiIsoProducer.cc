#include <memory>
#include <string>
#include <vector>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/Phase2L1ParticleFlow/interface/L1EGPuppiIsoAlgo.h"

template <typename T>
class L1TEGPuppiIsoProducer : public edm::stream::EDProducer<> {
public:
  explicit L1TEGPuppiIsoProducer(const edm::ParameterSet &);
  ~L1TEGPuppiIsoProducer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

  edm::EDGetTokenT<std::vector<T>> emToken_;
  edm::EDGetTokenT<std::vector<l1t::PFCandidate>> pfCandToken_;

  std::unique_ptr<l1ct::L1EGPuppiIsoAlgo<T>> algo_;

  const std::string outInstanceName_;
};

template <typename T>
L1TEGPuppiIsoProducer<T>::L1TEGPuppiIsoProducer(const edm::ParameterSet& iConfig)
    : emToken_(consumes<std::vector<T>>(iConfig.getParameter<edm::InputTag>("l1TkEmCollection"))),
      pfCandToken_(consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PFCandidates"))),
      algo_(nullptr),
      outInstanceName_(iConfig.getParameter<std::string>("outInstanceName"))
{
  algo_ = std::make_unique<l1ct::L1EGPuppiIsoAlgo<T>>(l1ct::L1EGPuppiIsoAlgoConfig(
    iConfig.getParameter<std::string>("pfIsoType"),
    iConfig.getParameter<double>("pfMinPt"),
    iConfig.getParameter<double>("dRMinIso"),
    iConfig.getParameter<double>("dRMaxIso"),
    iConfig.getParameter<double>("dRMaxDz")));

  produces<std::vector<T>>(outInstanceName_);
}

template <typename T>
void L1TEGPuppiIsoProducer<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  auto outL1EGs = std::make_unique<std::vector<T>>();

  edm::Handle<std::vector<T>> l1EGs;
  iEvent.getByToken(emToken_, l1EGs);

  edm::Handle<l1t::PFCandidateCollection> l1PFCands;
  iEvent.getByToken(pfCandToken_, l1PFCands);

  algo_->run(*l1EGs, *l1PFCands, *outL1EGs);

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

#include "DataFormats/L1TCorrelator/interface/TkEm.h"
#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
typedef L1TEGPuppiIsoProducer<l1t::TkEm> L1TTkEmPuppiIsoProducer;
typedef L1TEGPuppiIsoProducer<l1t::TkElectron> L1TTkElectronPuppiIsoProducer;
DEFINE_FWK_MODULE(L1TTkEmPuppiIsoProducer);
DEFINE_FWK_MODULE(L1TTkElectronPuppiIsoProducer);

