// -*- C++ -*-
//
/**\class L1EgammaPfIsoProducer

 Description: Producer PF or PF PUPPI isolation for a L1EG object

 Implementation:
     [Notes on implementation]
*/
//
// system include files

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
#include "DataFormats/L1TCorrelator/interface/TkElectronFwd.h"
#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "L1Trigger/L1TTrackMatch/interface/L1TkElectronTrackMatchAlgo.h"
#include "L1Trigger/Phase2L1ParticleFlow/interface/L1TPFUtils.h"

//
// class declaration
//

class L1EgammaPfIsoProducer : public edm::stream::EDProducer<> {
public:
  explicit L1EgammaPfIsoProducer(const edm::ParameterSet&);
  ~L1EgammaPfIsoProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;

  float calcIso(const GlobalPoint& l1EgCaloPos, const edm::Handle<std::vector<l1t::PFCandidate>>& l1PfCandCollHandle) const;
  // ----------member data ---------------------------
  const edm::EDGetTokenT<l1t::TkElectronCollection> l1EgammaToken_;
  const edm::EDGetTokenT<std::vector<l1t::PFCandidate>> l1PfCandToken_;

  // label of the collection produced
  std::string label_;

  float isoDRMin2_;
  float isoDRMax2_;
  float isoPtMin_;
  float isoCut_;
  bool relIso_;

  float bFieldZ_;
};

//
// constructors and destructor
//
L1EgammaPfIsoProducer::L1EgammaPfIsoProducer(const edm::ParameterSet& iConfig)
    : l1EgammaToken_(consumes<l1t::TkElectronCollection>(iConfig.getParameter<edm::InputTag>("l1EgammaInputTag"))),
      l1PfCandToken_(consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PfCandidateInputTag"))),
      label_(iConfig.getParameter<std::string>("label")),
      isoDRMin2_(static_cast<float>(iConfig.getParameter<double>("isoDRMin") * iConfig.getParameter<double>("isoDRMin"))),
      isoDRMax2_(static_cast<float>(iConfig.getParameter<double>("isoDRMax") * iConfig.getParameter<double>("isoDRMax"))),
      isoPtMin_(static_cast<float>(iConfig.getParameter<double>("isoPTMin"))),
      isoCut_(static_cast<float>(iConfig.getParameter<double>("isoCut"))),
      relIso_(iConfig.getParameter<bool>("relativeIsolation")),
      bFieldZ_(0.)
{
  produces<l1t::TkElectronCollection>(label_);
}

L1EgammaPfIsoProducer::~L1EgammaPfIsoProducer() {}

// ------------ method called to produce the data  ------------
void L1EgammaPfIsoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // get the L1EGamma objects
  edm::Handle<l1t::TkElectronCollection> l1EgammaCollHandle;
  iEvent.getByToken(l1EgammaToken_, l1EgammaCollHandle);
  if (!l1EgammaCollHandle.isValid()) {
    throw cms::Exception("MissingProduct") << "L1 EGamma collection not found in the event. Exit.";
    return;
  }
  const auto l1EgammaColl = l1EgammaCollHandle.product();

  // get the L1 PF candidate objects
  edm::Handle<std::vector<l1t::PFCandidate>> l1PfCandCollHandle;
  iEvent.getByToken(l1PfCandToken_, l1PfCandCollHandle);
  if (!l1PfCandCollHandle.isValid()) {
    throw cms::Exception("MissingProduct") << "L1 PF candidate collection not found in the event. Exit.";
    return;
  }

  auto outColl = std::make_unique<l1t::TkElectronCollection>();

  // loop over L1 EG objects in the collection
  for (const auto &l1Egamma : *l1EgammaColl) {
    // L1 EG position at the calorimeter
    const auto l1EgPos = L1TkElectronTrackMatchAlgo::calorimeterPosition(l1Egamma.phi(), l1Egamma.eta(), l1Egamma.energy());

    // calculate the PF isolation
    auto iso = calcIso(l1EgPos, l1PfCandCollHandle);

    // relative or absolute isolation
    if (relIso_ and l1Egamma.et() > 0.) {
      iso /= l1Egamma.et();
    }

    l1t::TkElectron l1eg(l1Egamma);
    // set the isolation variable
    // for the moment use the track isolation variable to store the value
    l1eg.setTrkIsol(iso);

    // write to the output collection
    if (isoCut_ <= 0.) {
      // irrespective of its relative isolation
      outColl->emplace_back(l1eg);
    } else {
      // if it passes the isolation cut
      if (iso <= isoCut_) {
        outColl->emplace_back(l1eg);
      }
    }
  }

  iEvent.put(std::move(outColl), label_);
}

void L1EgammaPfIsoProducer::beginRun(const edm::Run &run, const edm::EventSetup &setup)
{
  // magnetic field for particle propagation
  edm::ESHandle<MagneticField> magneticField;
  setup.get<IdealMagneticFieldRecord>().get(magneticField);
  bFieldZ_ = magneticField->inTesla(GlobalPoint(0., 0., 0.)).z();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void L1EgammaPfIsoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("l1EgammaInputTag");
  desc.add<edm::InputTag>("l1PfCandidateInputTag");
  desc.add<std::string>("label", "");
  desc.add<double>("isoDRMin", 0.03);
  desc.add<double>("isoDRMax", 0.2);
  desc.add<double>("isoPTMin", 1.);
  desc.add<double>("isoCut", 0.);
  desc.add<bool>("relativeIsolation", true);
  descriptions.addDefault(desc);
}

// method to calculate isolation
float L1EgammaPfIsoProducer::calcIso(const GlobalPoint& l1EgCaloPos, const edm::Handle<std::vector<l1t::PFCandidate>>& l1PfCandCollHandle) const
{
  float sumPt = 0.;

  // loop over PF candidates
  for (size_t i = 0; i < l1PfCandCollHandle->size(); ++i) {
    const auto l1PfCand = l1PfCandCollHandle->at(i);

    // do not add PF candidate to iso if pT below threshold
    if (l1PfCand.pt() < isoPtMin_) {
      continue;
    }

    const auto vtx = math::XYZTLorentzVector(l1PfCand.vx(), l1PfCand.vy(), l1PfCand.vz(), 0.);
    const auto charge = l1PfCand.charge();
    const auto etaPhiAtCalo = l1tpf::propagateToCalo(l1PfCand.p4(), vtx, charge, bFieldZ_);

    double dR2 = 999.;
    dR2 = reco::deltaR2(l1EgCaloPos.eta(), l1EgCaloPos.phi(), etaPhiAtCalo.first, etaPhiAtCalo.second);

    // store PF cand info if close enough to the EG object
    if (dR2 < 999. and dR2 >= isoDRMin2_ and dR2 < isoDRMax2_) {
      sumPt += l1PfCand.pt();
    }
  }

  return sumPt;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1EgammaPfIsoProducer);
