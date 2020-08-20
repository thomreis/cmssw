// -*- C++ -*-
//
/**\class L1CandPfIsoProducer

 Description: Produces PF or PF PUPPI isolations for a L1Candidate object collection

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

#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "L1Trigger/L1TTrackMatch/interface/L1TkElectronTrackMatchAlgo.h"
#include "L1Trigger/Phase2L1ParticleFlow/interface/L1TPFUtils.h"

//
// class declaration
//

template <typename T>
class L1CandPfIsoProducer : public edm::stream::EDProducer<> {
public:
  explicit L1CandPfIsoProducer(const edm::ParameterSet&);
  ~L1CandPfIsoProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;

  float calcIso(const GlobalPoint& l1EgCaloPos,
                const edm::Handle<std::vector<l1t::PFCandidate>>& l1PfCandCollHandle) const;
  // ----------member data ---------------------------
  const edm::EDGetTokenT<T> l1CandToken_;
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
template <typename T>
L1CandPfIsoProducer<T>::L1CandPfIsoProducer(const edm::ParameterSet& iConfig)
    : l1CandToken_(consumes<T>(iConfig.getParameter<edm::InputTag>("l1CandInputTag"))),
      l1PfCandToken_(
          consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PfCandidateInputTag"))),
      label_(iConfig.getParameter<std::string>("label")),
      isoDRMin2_(
          static_cast<float>(iConfig.getParameter<double>("isoDRMin") * iConfig.getParameter<double>("isoDRMin"))),
      isoDRMax2_(
          static_cast<float>(iConfig.getParameter<double>("isoDRMax") * iConfig.getParameter<double>("isoDRMax"))),
      isoPtMin_(static_cast<float>(iConfig.getParameter<double>("isoPTMin"))),
      isoCut_(static_cast<float>(iConfig.getParameter<double>("isoCut"))),
      relIso_(iConfig.getParameter<bool>("relativeIsolation")),
      bFieldZ_(0.) {
  produces<T>(label_);
}

template <typename T>
L1CandPfIsoProducer<T>::~L1CandPfIsoProducer() {}

// ------------ method called to produce the data  ------------
template <typename T>
void L1CandPfIsoProducer<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // get the L1EGamma objects
  edm::Handle<T> l1CandCollHandle;
  iEvent.getByToken(l1CandToken_, l1CandCollHandle);
  if (!l1CandCollHandle.isValid()) {
    throw cms::Exception("MissingProduct") << "L1 EGamma collection not found in the event. Exit.";
    return;
  }
  const auto l1CandColl = l1CandCollHandle.product();

  // get the L1 PF candidate objects
  edm::Handle<std::vector<l1t::PFCandidate>> l1PfCandCollHandle;
  iEvent.getByToken(l1PfCandToken_, l1PfCandCollHandle);
  if (!l1PfCandCollHandle.isValid()) {
    throw cms::Exception("MissingProduct") << "L1 PF candidate collection not found in the event. Exit.";
    return;
  }

  auto outColl = std::make_unique<T>();

  // loop over L1 objects in the collection
  for (const auto& l1Cand : *l1CandColl) {
    // L1 EG position at the calorimeter
    const auto l1CandPos = L1TkElectronTrackMatchAlgo::calorimeterPosition(l1Cand.phi(), l1Cand.eta(), l1Cand.energy());

    // calculate the PF isolation
    auto iso = calcIso(l1CandPos, l1PfCandCollHandle);

    // relative or absolute isolation
    if (relIso_ and l1Cand.et() > 0.) {
      iso /= l1Cand.et();
    }

    auto l1CandWithPfIso = l1Cand;
    // set the isolation variable
    // for the moment use the track isolation variable to store the value
    l1CandWithPfIso.setTrkIsol(iso);

    // write to the output collection
    if (isoCut_ <= 0.) {
      // irrespective of its relative isolation
      outColl->emplace_back(l1CandWithPfIso);
    } else {
      // if it passes the isolation cut
      if (iso <= isoCut_) {
        outColl->emplace_back(l1CandWithPfIso);
      }
    }
  }

  iEvent.put(std::move(outColl), label_);
}

template <typename T>
void L1CandPfIsoProducer<T>::beginRun(const edm::Run& run, const edm::EventSetup& setup) {
  // magnetic field for particle propagation
  edm::ESHandle<MagneticField> magneticField;
  setup.get<IdealMagneticFieldRecord>().get(magneticField);
  bFieldZ_ = magneticField->inTesla(GlobalPoint(0., 0., 0.)).z();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
template <typename T>
void L1CandPfIsoProducer<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("l1CandInputTag");
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
template <typename T>
float L1CandPfIsoProducer<T>::calcIso(const GlobalPoint& l1EgCaloPos,
                                      const edm::Handle<std::vector<l1t::PFCandidate>>& l1PfCandCollHandle) const {
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

//define plugins for L1 TkElectrons and L1 TkEm
#include "DataFormats/L1TCorrelator/interface/TkEm.h"
#include "DataFormats/L1TCorrelator/interface/TkEmFwd.h"
#include "DataFormats/L1TCorrelator/interface/TkElectron.h"
#include "DataFormats/L1TCorrelator/interface/TkElectronFwd.h"
typedef L1CandPfIsoProducer<l1t::TkEmCollection> L1TkEmPfIsoProducer;
typedef L1CandPfIsoProducer<l1t::TkElectronCollection> L1TkElectronPfIsoProducer;
DEFINE_FWK_MODULE(L1TkEmPfIsoProducer);
DEFINE_FWK_MODULE(L1TkElectronPfIsoProducer);
