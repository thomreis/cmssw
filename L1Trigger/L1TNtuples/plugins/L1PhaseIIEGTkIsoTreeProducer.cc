// -*- C++ -*-
//
// Package:    L1Trigger/L1TNtuples
// Class:      L1PhaseIIEGTkIsoTreeProducer
//
/**\class L1PhaseIIEGTkIsoTreeProducer L1PhaseIIEGTkIsoTreeProducer.cc L1Trigger/L1TNtuples/plugins/L1PhaseIIEGTkIsoTreeProducer.cc

 Description: Produces an L1 ntuple branch for phase II EG tracker isolation studies

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis
//         Created:  Wed, 11 Sep 2019 14:55:42 GMT
//
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

// data formats
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TParticleFlow/interface/PFCandidate.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisPhaseIIEGTkIso.h"

//
// class declaration
//
class L1PhaseIIEGTkIsoTreeProducer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
    explicit L1PhaseIIEGTkIsoTreeProducer(const edm::ParameterSet&);
    ~L1PhaseIIEGTkIsoTreeProducer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    std::shared_ptr<L1Analysis::L1AnalysisPhaseIIEGTkIso> l1Phase2EGTkIso_;
    L1Analysis::L1AnalysisPhaseIIEGTkIsoDataFormat *l1Phase2EGTkIsoData_;

    typedef TTTrack<Ref_Phase2TrackerDigi_> L1TTTrackType;
    typedef std::vector<L1TTTrackType> L1TTTrackCollectionType;

  private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    // ----------member data ---------------------------

    // output file
    edm::Service<TFileService> fs_;

    // tree
    TTree* tree_;

    edm::EDGetTokenT<l1t::EGammaBxCollection> egToken_;
    edm::EDGetTokenT<l1t::EGammaBxCollection> egTokenHGC_;
    edm::EDGetTokenT<L1TTTrackCollectionType> tttrackToken_;
    edm::EDGetTokenT<std::vector<l1t::PFCandidate>> pfCandToken_;
    edm::ESGetToken<TrackerGeometry, TrackerDigiGeometryRecord> tGeomToken_;
    edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> magFieldToken_;

    std::string trackerGeom_;
    bool storeAllPFCands_;
};

//
// constructors and destructor
//
L1PhaseIIEGTkIsoTreeProducer::L1PhaseIIEGTkIsoTreeProducer(const edm::ParameterSet& iConfig) :
  l1Phase2EGTkIso_(std::make_shared<L1Analysis::L1AnalysisPhaseIIEGTkIso>(iConfig)),
  tree_(fs_->make<TTree>("L1PhaseIIEGTkIsoTree", "L1PhaseIIEGTkIsoTree")),
  egToken_(consumes<l1t::EGammaBxCollection>(iConfig.getParameter<edm::InputTag>("l1EgBarrel"))),
  egTokenHGC_(consumes<l1t::EGammaBxCollection>(iConfig.getParameter<edm::InputTag>("l1EgHGC"))),
  tttrackToken_(consumes<L1TTTrackCollectionType>(iConfig.getParameter<edm::InputTag>("l1Tracks"))),
  pfCandToken_(consumes<std::vector<l1t::PFCandidate>>(iConfig.getParameter<edm::InputTag>("l1PFCandidates"))),
  tGeomToken_(esConsumes<TrackerGeometry, TrackerDigiGeometryRecord>()),
  magFieldToken_(esConsumes<MagneticField, IdealMagneticFieldRecord>()),
  trackerGeom_(iConfig.getParameter<std::string>("trackerGeometry")),
  storeAllPFCands_(iConfig.getParameter<bool>("storeAllPFCands"))
{
  l1Phase2EGTkIsoData_ = l1Phase2EGTkIso_->GetData();

  tree_->Branch("L1PhaseIIEGTkIso", "L1Analysis::L1AnalysisPhaseIIEGTkIsoDataFormat", &l1Phase2EGTkIsoData_, 32000, 3);
}


L1PhaseIIEGTkIsoTreeProducer::~L1PhaseIIEGTkIsoTreeProducer()
{
}


//
// member functions
//

// ------------ method called for each event  ------------
void
L1PhaseIIEGTkIsoTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // geometry needed to call pTFrom2Stubs
  const auto* tGeom = &iSetup.getData(tGeomToken_);
  const float bFieldZ = iSetup.getData(magFieldToken_).inTesla(GlobalPoint(0, 0, 0)).z();


  // first reset the branch
  l1Phase2EGTkIso_->Reset();

  // get the collections
  edm::Handle<l1t::EGammaBxCollection> egBarrel;
  iEvent.getByToken(egToken_, egBarrel);
  if (!egBarrel.isValid()) {
    edm::LogWarning("MissingProduct") << "Barrel Egamma BX collection not found in the event. Branch will not be filled";
    return;
  }

  edm::Handle<l1t::EGammaBxCollection> egHGC;
  iEvent.getByToken(egTokenHGC_, egHGC);
  if (!egHGC.isValid()) {
    edm::LogWarning("MissingProduct") << "HGC Egamma BX collection not found in the event. Branch will not be filled";
    return;
  }

  edm::Handle<L1TTTrackCollectionType> tttrack;
  iEvent.getByToken(tttrackToken_, tttrack);
  if (!tttrack.isValid()) {
    edm::LogWarning("MissingProduct") << "TT track collection not found in the event. Branch will not be filled";
    return;
  }

  edm::Handle<std::vector<l1t::PFCandidate>> pfCands;
  iEvent.getByToken(pfCandToken_, pfCands);
  if (!pfCands.isValid()) {
    edm::LogWarning("MissingProduct") << "L1 PF candidate collection not found in the event. Branch will not be filled";
    return;
  }

  // set the branches
  l1Phase2EGTkIso_->SetEGWithTracks(egBarrel, egHGC, tttrack, tGeom, pfCands, bFieldZ);
  if (storeAllPFCands_) {
    l1Phase2EGTkIso_->SetPFCands(pfCands, bFieldZ);
  }

  //fill the tree
  tree_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void
L1PhaseIIEGTkIsoTreeProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1PhaseIIEGTkIsoTreeProducer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1PhaseIIEGTkIsoTreeProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("l1EgBarrel");
  desc.add<edm::InputTag>("l1EgHGC");
  desc.add<edm::InputTag>("l1Tracks");
  desc.add<edm::InputTag>("l1PFCandidates");
  desc.add<std::string>("trackerGeometry", "idealForDigi");
  desc.add<bool>("storeAllPFCands", false);
  desc.add<double>("egBarrelMinEt", 5.);
  desc.add<double>("egHGCMinEt", 10.);
  desc.add<double>("trackMinPt", 0.);
  desc.add<double>("trackMaxChi2", 1e10);
  desc.add<bool>("useTwoStubsPt", false);
  desc.add<std::string>("trackEGammaMatchType", "PtDependentCut");
  desc.add<std::vector<double>>("trackEGammaDeltaPhi", {0.07, 0., 0.});
  desc.add<std::vector<double>>("trackEGammaDeltaR", {0.08, 0., 0.});
  desc.add<std::vector<double>>("trackEGammaDeltaEta", {1e10, 0., 0.});
  desc.add<double>("trackMinPtForIsoEB", 2.);
  desc.add<double>("trackMinPtForIsoHGC", 2.);
  desc.add<double>("trackMaxChi2ForIsoEB", 1e10);
  desc.add<double>("trackMaxChi2ForIsoHGC", 1e10);
  desc.add<unsigned int>("trackMinNStubsForIsoEB", 0);
  desc.add<unsigned int>("trackMinNStubsForIsoHGC", 0);
  desc.add<double>("dRMinForIso", 0.);
  desc.add<double>("dRMaxForIso", 1.);
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1PhaseIIEGTkIsoTreeProducer);
