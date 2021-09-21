#include <memory>
#include <string>
#include <vector>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/Phase2L1ParticleFlow/src/newfirmware/egamma/L1EGPuppiIsoAlgo.h"

template <typename T>
class L1TEGPuppiIsoProducer : public edm::stream::EDProducer<> {
public:
  explicit L1TEGPuppiIsoProducer(const edm::ParameterSet &);
  ~L1TEGPuppiIsoProducer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

  void prepareAlgoEgInput(const std::vector<T> &l1EGs, l1ct::EGIsoObjsEmu &egIsoObjs);
  void prepareAlgoPuppiInput(const l1t::PFCandidateCollection &l1PFCands, l1ct::PuppiObjs &puppiObjs);
  void addPuppiIsoToOutput(const l1ct::EGIsoObjsEmu &outEgIsoObjsEmu, std::vector<T> &outL1EGs);

  edm::EDGetTokenT<std::vector<T>> emToken_;
  edm::EDGetTokenT<std::vector<l1t::PFCandidate>> pfCandToken_;

  std::unique_ptr<l1ct::L1EGPuppiIsoAlgo> algo_;

  const std::string outInstanceName_;
};

template <typename T>
L1TEGPuppiIsoProducer<T>::L1TEGPuppiIsoProducer(const edm::ParameterSet& iConfig)
    : emToken_(consumes<std::vector<T>>(iConfig.getParameter<edm::InputTag>("l1TkEmCollection"))),
      pfCandToken_(consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PFCandidates"))),
      algo_(nullptr),
      outInstanceName_(iConfig.getParameter<std::string>("outInstanceName"))
{
  algo_ = std::make_unique<l1ct::L1EGPuppiIsoAlgo>(l1ct::L1EGPuppiIsoAlgoConfig(
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

  // get the EDM collections
  edm::Handle<std::vector<T>> l1EGs;
  iEvent.getByToken(emToken_, l1EGs);

  edm::Handle<l1t::PFCandidateCollection> l1PFCands;
  iEvent.getByToken(pfCandToken_, l1PFCands);

  // duplicate the L1 EG input collection since this module only calculates the PUPPI isolation
  for (const auto &l1EG : *l1EGs) {
    outL1EGs->emplace_back(l1EG);
  }

  // the collections of the isolation algo
  l1ct::EGIsoObjsEmu egIsoObjs;
  l1ct::EGIsoObjsEmu outEgIsoObjs;
  l1ct::PuppiObjs puppiObjs;

  // prepare the algo object collections from the EDM collections
  prepareAlgoEgInput(*l1EGs, egIsoObjs);
  prepareAlgoPuppiInput(*l1PFCands, puppiObjs);

  // run the iso algo
  algo_->run(egIsoObjs, puppiObjs, outEgIsoObjs);

  // decode the algo output collections and make EDM collections to put in the event
  addPuppiIsoToOutput(outEgIsoObjs, *outL1EGs);

  iEvent.put(std::move(outL1EGs), outInstanceName_);
}

template <typename T>
void L1TEGPuppiIsoProducer<T>::prepareAlgoEgInput(const std::vector<T> &l1EGs, l1ct::EGIsoObjsEmu &egIsoObjs)
{
  for (const auto &l1EG : l1EGs) {
    l1ct::EGIsoObjEmu obj;
    obj.hwPt = l1ct::Scales::makePtFromFloat(l1EG.pt());
    obj.hwEta = l1ct::Scales::makeEta(l1EG.eta());
    obj.hwPhi = l1ct::Scales::makePhi(l1EG.phi());
    obj.hwQual = l1EG.hwQual();
    obj.hwIso = l1EG.hwIso();
    egIsoObjs.emplace_back(obj);
  }
}

template <typename T>
void L1TEGPuppiIsoProducer<T>::prepareAlgoPuppiInput(const l1t::PFCandidateCollection &l1PFCands, l1ct::PuppiObjs &puppiObjs)
{
  for (const auto &l1PFCand : l1PFCands) {
    l1ct::PuppiObj obj;
    obj.hwPt = l1ct::Scales::makePtFromFloat(l1PFCand.pt());
    obj.hwEta = l1ct::Scales::makeEta(l1PFCand.eta());
    obj.hwPhi = l1ct::Scales::makePhi(l1PFCand.phi());
    if (l1PFCand.id() == l1t::PFCandidate::Muon) {
      obj.hwId = l1ct::ParticleID::mkMuon(l1PFCand.charge() > 0);
    } else if (l1PFCand.id() == l1t::PFCandidate::Electron) {
      obj.hwId = l1ct::ParticleID::mkElectron(l1PFCand.charge() > 0);
    } else if (l1PFCand.id() == l1t::PFCandidate::ChargedHadron) {
      obj.hwId = l1ct::ParticleID::mkChHad(l1PFCand.charge() > 0);
    } else if (l1PFCand.id() == l1t::PFCandidate::Photon) {
      obj.hwId = l1ct::ParticleID::PHOTON;
    } else {
      obj.hwId = l1ct::ParticleID::HADZERO;
    }
    if (obj.hwId.charged()) {
      obj.setHwZ0(l1PFCand.hwZ0());
      obj.setHwDxy(l1PFCand.hwDxy());
      obj.setHwTkQuality(l1PFCand.hwTkQuality());
    } else {
      obj.setHwPuppiW(l1PFCand.hwPuppiWeight());
    }
    puppiObjs.emplace_back(obj);
  }
}

template <typename T>
void L1TEGPuppiIsoProducer<T>::addPuppiIsoToOutput(const l1ct::EGIsoObjsEmu &outEgIsoObjsEmu, std::vector<T> &outL1EGs)
{
  for (size_t i = 0; i < outEgIsoObjsEmu.size() && i < outL1EGs.size(); ++i) {
    outL1EGs[i].setPuppiIsol(outEgIsoObjsEmu[i].floatIso(l1ct::EGIsoObjEmu::IsoType::PfIso));
    outL1EGs[i].setPuppiIsolPV(outEgIsoObjsEmu[i].floatIso(l1ct::EGIsoObjEmu::IsoType::PfIsoPV));
  }
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

