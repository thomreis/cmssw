// input: L1TkTracks and  RegionalMuonCand (standalone with component details)
// match the two and produce a collection of TkMuon
// eventually, this should be made modular and allow to swap out different algorithms

// user include files
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/L1TCorrelator/interface/TkMuon.h"
#include "DataFormats/L1TCorrelator/interface/TkMuonFwd.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkMuCorrDynamicWindows.h"
#include "L1Trigger/L1TTrackMatch/interface/L1TkMuMantra.h"
#include "L1Trigger/L1TMuonEndCap/interface/Common.h"

// system include files
#include <memory>
#include <string>

using namespace l1t;

class L1TkMuonProducer : public edm::EDProducer {
public:

  typedef TTTrack< Ref_Phase2TrackerDigi_ >  L1TTTrackType;
  typedef std::vector< L1TTTrackType > L1TTTrackCollectionType;

  struct PropState { //something simple, imagine it's hardware emulation
    PropState() :
      pt(-99),  eta(-99), phi(-99),
      sigmaPt(-99),  sigmaEta(-99), sigmaPhi(-99),
      valid(false) {}
    float pt;
    float eta;
    float phi;
    float sigmaPt;
    float sigmaEta;
    float sigmaPhi;
    bool valid;

  };

  enum AlgoType {
    kTP = 1,
    kDynamicWindows = 2,
    kMantra = 3
  };

  explicit L1TkMuonProducer(const edm::ParameterSet&);
  ~L1TkMuonProducer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void produce(edm::Event&, const edm::EventSetup&);
  PropState propagateToGMT(const L1TTTrackType& l1tk) const;
  double sigmaEtaTP(const RegionalMuonCand& mu) const;
  double sigmaPhiTP(const RegionalMuonCand& mu) const;

  // the TP algorithm
  void runOnMTFCollection_v1(const edm::Handle<RegionalMuonCandBxCollection>&,
                          const edm::Handle<L1TTTrackCollectionType>&,
                          TkMuonCollection& tkMuons, const int detector) const;

  // algo for endcap regions using dynamic windows for making the match
  void runOnMTFCollection_v2(const edm::Handle<EMTFTrackCollection>&,
                          const edm::Handle<L1TTTrackCollectionType>&,
                          TkMuonCollection& tkMuons) const;

  // given the matching indexes, build the output collection of track muons
  void build_tkMuons_from_idxs (TkMuonCollection& tkMuons,
                                const std::vector<int>& matches,
                                const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                const edm::Handle<RegionalMuonCandBxCollection>& muonH,
                                int detector) const;
  
  // as above, but for the TkMu that were built from a EMTFCollection - do not produce a valid muon ref
  void build_tkMuons_from_idxs (TkMuonCollection& tkMuons,
                                const std::vector<int>& matches,
                                const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                int detector) const;

  // dump and convert tracks to the format needed for the MAnTra correlator
  std::vector<L1TkMuMantraDF::track_df> product_to_trkvec(const L1TTTrackCollectionType& l1tks); // tracks
  std::vector<L1TkMuMantraDF::muon_df>  product_to_muvec (const RegionalMuonCandBxCollection& l1mtfs); // regional muon finder
  std::vector<L1TkMuMantraDF::muon_df>  product_to_muvec (const EMTFTrackCollection& l1mus); // endcap muon finder - eventually to be moved to regional candidate

  float ETAMIN_;
  float ETAMAX_;
  float ZMAX_;             // |z_track| < ZMAX in cm
  float CHI2MAX_;
  float PTMINTRA_;
  float DRmax_;
  int nStubsmin_ ;         // minimum number of stubs
  //  bool closest_ ;
  bool correctGMTPropForTkZ_;
  bool use5ParameterFit_;
  bool useTPMatchWindows_;

  // int emtfMatchAlgoVersion_ ;
  AlgoType bmtfMatchAlgoVersion_ ;
  AlgoType omtfMatchAlgoVersion_ ;
  AlgoType emtfMatchAlgoVersion_ ;

  std::unique_ptr<L1TkMuCorrDynamicWindows> dwcorr_;

  std::unique_ptr<L1TkMuMantra> mantracorr_barr_;
  std::unique_ptr<L1TkMuMantra> mantracorr_ovrl_;
  std::unique_ptr<L1TkMuMantra> mantracorr_endc_;
  int mantra_n_trk_par_;

  const edm::EDGetTokenT< RegionalMuonCandBxCollection > bmtfToken;
  const edm::EDGetTokenT< RegionalMuonCandBxCollection > omtfToken;
  const edm::EDGetTokenT< RegionalMuonCandBxCollection > emtfToken;
  const edm::EDGetTokenT< EMTFTrackCollection >          emtfTCToken; // the track collection, directly from the EMTF and not formatted by GT
  const edm::EDGetTokenT< std::vector< TTTrack< Ref_Phase2TrackerDigi_ > > > trackToken;
} ;


L1TkMuonProducer::L1TkMuonProducer(const edm::ParameterSet& iConfig) :
  bmtfToken  (consumes< RegionalMuonCandBxCollection >(iConfig.getParameter<edm::InputTag>("L1BMTFInputTag"))),
  omtfToken  (consumes< RegionalMuonCandBxCollection >(iConfig.getParameter<edm::InputTag>("L1OMTFInputTag"))),
  emtfToken  (consumes< RegionalMuonCandBxCollection >(iConfig.getParameter<edm::InputTag>("L1EMTFInputTag"))),
  emtfTCToken(consumes< EMTFTrackCollection >         (iConfig.getParameter<edm::InputTag>("L1EMTFTrackCollectionInputTag"))),
  trackToken (consumes< std::vector<TTTrack< Ref_Phase2TrackerDigi_> > > (iConfig.getParameter<edm::InputTag>("L1TrackInputTag")))
{
   ETAMIN_ = (float)iConfig.getParameter<double>("ETAMIN");
   ETAMAX_ = (float)iConfig.getParameter<double>("ETAMAX");
   ZMAX_ = (float)iConfig.getParameter<double>("ZMAX");
   CHI2MAX_ = (float)iConfig.getParameter<double>("CHI2MAX");
   PTMINTRA_ = (float)iConfig.getParameter<double>("PTMINTRA");
   DRmax_ = (float)iConfig.getParameter<double>("DRmax");
   nStubsmin_ = iConfig.getParameter<int>("nStubsmin");
   //   closest_ = iConfig.getParameter<bool>("closest");
   // emtfMatchAlgoVersion_ = iConfig.getParameter<int>("emtfMatchAlgoVersion");

   // --- mantra corr params

   mantra_n_trk_par_ = iConfig.getParameter<int>("mantra_n_trk_par");

   // --------------------- configuration of the muon algorithm type

   std::string bmtfMatchAlgoVersionString = iConfig.getParameter<std::string>("bmtfMatchAlgoVersion");
   std::transform(bmtfMatchAlgoVersionString.begin(), bmtfMatchAlgoVersionString.end(), bmtfMatchAlgoVersionString.begin(), ::tolower); // make lowercase

   std::string omtfMatchAlgoVersionString = iConfig.getParameter<std::string>("omtfMatchAlgoVersion");
   std::transform(omtfMatchAlgoVersionString.begin(), omtfMatchAlgoVersionString.end(), omtfMatchAlgoVersionString.begin(), ::tolower); // make lowercase

   std::string emtfMatchAlgoVersionString = iConfig.getParameter<std::string>("emtfMatchAlgoVersion");
   std::transform(emtfMatchAlgoVersionString.begin(), emtfMatchAlgoVersionString.end(), emtfMatchAlgoVersionString.begin(), ::tolower); // make lowercase

   if (bmtfMatchAlgoVersionString == "tp")
      bmtfMatchAlgoVersion_ = kTP;
   else if (bmtfMatchAlgoVersionString == "mantra")
      bmtfMatchAlgoVersion_ = kMantra;
   else
    throw cms::Exception("TkMuAlgoConfig") << "the ID of the BMTF algo matcher passed is invalid\n";

   //

   if (omtfMatchAlgoVersionString == "tp")
      omtfMatchAlgoVersion_ = kTP;
   else if (omtfMatchAlgoVersionString == "mantra")
      omtfMatchAlgoVersion_ = kMantra;
   else
    throw cms::Exception("TkMuAlgoConfig") << "the ID of the OMTF algo matcher passed is invalid\n";

   //

   if (emtfMatchAlgoVersionString == "tp")
      emtfMatchAlgoVersion_ = kTP;
   else if (emtfMatchAlgoVersionString == "dynamicwindows")
      emtfMatchAlgoVersion_ = kDynamicWindows;
   else if (emtfMatchAlgoVersionString == "mantra")
      emtfMatchAlgoVersion_ = kMantra;
   else
    throw cms::Exception("TkMuAlgoConfig") << "the ID of the EMTF algo matcher passed is invalid\n";

   correctGMTPropForTkZ_ = iConfig.getParameter<bool>("correctGMTPropForTkZ");

   use5ParameterFit_     = iConfig.getParameter<bool>("use5ParameterFit");
   useTPMatchWindows_     = iConfig.getParameter<bool>("useTPMatchWindows");
   produces<TkMuonCollection>();

   // initializations
   if (emtfMatchAlgoVersion_ == kDynamicWindows)
   {
      // FIXME: to merge eventually into an unique file with bith phi and theta boundaries
      std::string fIn_bounds_name = iConfig.getParameter<edm::FileInPath>("emtfcorr_boundaries").fullPath();
      std::string fIn_theta_name  = iConfig.getParameter<edm::FileInPath>("emtfcorr_theta_windows").fullPath();
      std::string fIn_phi_name    = iConfig.getParameter<edm::FileInPath>("emtfcorr_phi_windows").fullPath();
      auto bounds = L1TkMuCorrDynamicWindows::prepare_corr_bounds(fIn_bounds_name.c_str(), "h_dphi_l");
      TFile* fIn_theta = TFile::Open (fIn_theta_name.c_str());
      TFile* fIn_phi   = TFile::Open (fIn_phi_name.c_str());
      dwcorr_ = std::unique_ptr<L1TkMuCorrDynamicWindows> (new L1TkMuCorrDynamicWindows(bounds, fIn_theta, fIn_phi));

      // files can be closed since the correlator code clones the TF1s
      fIn_theta->Close();
      fIn_phi->Close();

      // FIXME: more initialisation using the parameters passed from the cfg
      dwcorr_->set_safety_factor  (iConfig.getParameter<double>("final_window_factor"));
      dwcorr_->set_sf_initialrelax(iConfig.getParameter<double>("initial_window_factor"));

      dwcorr_->set_relaxation_pattern(
        iConfig.getParameter<double>("pt_start_relax"),
        iConfig.getParameter<double>("pt_end_relax")
        );

      dwcorr_->set_do_relax_factor(iConfig.getParameter<bool>("do_relax_factors"));

      //
      dwcorr_ -> set_n_trk_par       (iConfig.getParameter<int>("n_trk_par"));
      dwcorr_ -> set_min_trk_p       (iConfig.getParameter<double>("min_trk_p"));
      dwcorr_ -> set_max_trk_aeta    (iConfig.getParameter<double>("max_trk_aeta"));
      dwcorr_ -> set_max_trk_chi2    (iConfig.getParameter<double>("max_trk_chi2"));
      dwcorr_ -> set_min_trk_nstubs  (iConfig.getParameter<int>("min_trk_nstubs"));
      dwcorr_ -> set_do_trk_qual_presel(true);
   }

   if (bmtfMatchAlgoVersion_ == kMantra)
   {
      std::string fIn_bounds_name = iConfig.getParameter<edm::FileInPath>("mantra_bmtfcorr_boundaries")    .fullPath();
      std::string fIn_theta_name  = iConfig.getParameter<edm::FileInPath>("mantra_bmtfcorr_theta_windows") .fullPath();
      std::string fIn_phi_name    = iConfig.getParameter<edm::FileInPath>("mantra_bmtfcorr_phi_windows")   .fullPath();

      auto bounds = L1TkMuMantra::prepare_corr_bounds(fIn_bounds_name.c_str(), "h_dphi_l");

      TFile* fIn_theta = TFile::Open (fIn_theta_name.c_str());
      TFile* fIn_phi   = TFile::Open (fIn_phi_name.c_str());

      mantracorr_barr_ = std::unique_ptr<L1TkMuMantra> (new L1TkMuMantra(bounds, fIn_theta, fIn_phi, "mantra_barrel"));
      
      fIn_theta->Close();
      fIn_phi->Close();

      // settings : NB : now hardcoded, could be read from cfg
      mantracorr_barr_->set_safety_factor(0.5, 0.5);
      mantracorr_barr_->setArbitrationType("MaxPt");
   }

   if (omtfMatchAlgoVersion_ == kMantra)
   {
      std::string fIn_bounds_name = iConfig.getParameter<edm::FileInPath>("mantra_omtfcorr_boundaries")    .fullPath();
      std::string fIn_theta_name  = iConfig.getParameter<edm::FileInPath>("mantra_omtfcorr_theta_windows") .fullPath();
      std::string fIn_phi_name    = iConfig.getParameter<edm::FileInPath>("mantra_omtfcorr_phi_windows")   .fullPath();

      auto bounds = L1TkMuMantra::prepare_corr_bounds(fIn_bounds_name.c_str(), "h_dphi_l");

      TFile* fIn_theta = TFile::Open (fIn_theta_name.c_str());
      TFile* fIn_phi   = TFile::Open (fIn_phi_name.c_str());

      mantracorr_ovrl_ = std::unique_ptr<L1TkMuMantra> (new L1TkMuMantra(bounds, fIn_theta, fIn_phi, "mantra_overlap"));
      
      fIn_theta->Close();
      fIn_phi->Close();

      // settings : NB : now hardcoded, could be read from cfg
      mantracorr_ovrl_->set_safety_factor(0.5, 0.5);
      mantracorr_ovrl_->setArbitrationType("MaxPt");

   }

   if (emtfMatchAlgoVersion_ == kMantra)
   {
      std::string fIn_bounds_name = iConfig.getParameter<edm::FileInPath>("mantra_emtfcorr_boundaries")    .fullPath();
      std::string fIn_theta_name  = iConfig.getParameter<edm::FileInPath>("mantra_emtfcorr_theta_windows") .fullPath();
      std::string fIn_phi_name    = iConfig.getParameter<edm::FileInPath>("mantra_emtfcorr_phi_windows")   .fullPath();

      auto bounds = L1TkMuMantra::prepare_corr_bounds(fIn_bounds_name.c_str(), "h_dphi_l");

      TFile* fIn_theta = TFile::Open (fIn_theta_name.c_str());
      TFile* fIn_phi   = TFile::Open (fIn_phi_name.c_str());

      mantracorr_endc_ = std::unique_ptr<L1TkMuMantra> (new L1TkMuMantra(bounds, fIn_theta, fIn_phi, "mantra_endcap"));
      
      fIn_theta->Close();
      fIn_phi->Close();

      // settings : NB : now hardcoded, could be read from cfg
      mantracorr_endc_->set_safety_factor(0.5, 0.5);
      mantracorr_endc_->setArbitrationType("MaxPt");
   }

}

L1TkMuonProducer::~L1TkMuonProducer() {
}

// ------------ method called to produce the data  ------------
void
L1TkMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // the L1Mu objects
  edm::Handle<RegionalMuonCandBxCollection> l1bmtfH;
  edm::Handle<RegionalMuonCandBxCollection> l1omtfH;
  edm::Handle<RegionalMuonCandBxCollection> l1emtfH;
  edm::Handle<EMTFTrackCollection> l1emtfTCH;

  iEvent.getByToken(bmtfToken, l1bmtfH);
  iEvent.getByToken(omtfToken, l1omtfH);
  iEvent.getByToken(emtfToken, l1emtfH);
  iEvent.getByToken(emtfTCToken, l1emtfTCH);

  // the L1Tracks
  edm::Handle<L1TTTrackCollectionType> l1tksH;
  iEvent.getByToken(trackToken, l1tksH);

  TkMuonCollection oc_bmtf_tkmuon;
  TkMuonCollection oc_omtf_tkmuon;
  TkMuonCollection oc_emtf_tkmuon;

  std::vector<L1TkMuMantraDF::track_df> mantradf_tracks; // if needed, just encode once for all trk finders
  if (bmtfMatchAlgoVersion_ == kMantra || omtfMatchAlgoVersion_ == kMantra || emtfMatchAlgoVersion_ == kMantra){
      mantradf_tracks = product_to_trkvec(*l1tksH.product());
  }
    
  // process each of the MTF collections separately! -- we don't want to filter the muons

  // ----------------------------------------------------- barrel
  if (bmtfMatchAlgoVersion_ == kTP)
    runOnMTFCollection_v1(l1bmtfH, l1tksH, oc_bmtf_tkmuon,1);
  else if (bmtfMatchAlgoVersion_ == kMantra){
    auto muons     = product_to_muvec (*l1bmtfH.product());
    auto match_idx = mantracorr_barr_->find_match(mantradf_tracks, muons);
    build_tkMuons_from_idxs(oc_bmtf_tkmuon, match_idx, l1tksH, l1bmtfH, 1);
  }
  else
    throw cms::Exception("TkMuAlgoConfig") << " barrel : trying to run an invalid algorithm (this should never happen)\n";
  
  // ----------------------------------------------------- overlap
  if (omtfMatchAlgoVersion_ == kTP)
    runOnMTFCollection_v1(l1omtfH, l1tksH, oc_omtf_tkmuon,2);
  else if (omtfMatchAlgoVersion_ == kMantra){
    auto muons     = product_to_muvec (*l1omtfH.product());
    auto match_idx = mantracorr_ovrl_->find_match(mantradf_tracks, muons);
    build_tkMuons_from_idxs(oc_omtf_tkmuon, match_idx, l1tksH, l1omtfH, 2);
  }
  else
    throw cms::Exception("TkMuAlgoConfig") << " overlap : trying to run an invalid algorithm (this should never happen)\n";
  
  // ----------------------------------------------------- endcap
  if(emtfMatchAlgoVersion_ == kTP)
    runOnMTFCollection_v1(l1emtfH, l1tksH, oc_emtf_tkmuon,3);
  else if (emtfMatchAlgoVersion_ == kDynamicWindows)
    runOnMTFCollection_v2(l1emtfTCH, l1tksH, oc_emtf_tkmuon);
  else if (emtfMatchAlgoVersion_ == kMantra){
    auto muons     = product_to_muvec (*l1emtfTCH.product());
    auto match_idx = mantracorr_endc_->find_match(mantradf_tracks, muons);
    build_tkMuons_from_idxs(oc_emtf_tkmuon, match_idx, l1tksH, 3);
  }
  else
    throw cms::Exception("TkMuAlgoConfig") << "endcap : trying to run an invalid algorithm (this should never happen)\n";

  // now combine all trk muons into a single output collection!
  std::unique_ptr<TkMuonCollection> oc_tkmuon(new TkMuonCollection());
  for (const auto& p : {oc_bmtf_tkmuon, oc_omtf_tkmuon, oc_emtf_tkmuon}){
    oc_tkmuon->insert(oc_tkmuon->end(), p.begin(), p.end());
  }

  // put the new track+muon objects in the event!
  iEvent.put( std::move(oc_tkmuon));
};

void
L1TkMuonProducer::runOnMTFCollection_v1(const edm::Handle<RegionalMuonCandBxCollection>& muonH,
                                     const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                     TkMuonCollection& tkMuons,const int detector) const
{
  const L1TTTrackCollectionType& l1tks = (*l1tksH.product());
  const RegionalMuonCandBxCollection& l1mtfs = (*muonH.product());

  int imu = 0;
  for (auto l1mu = l1mtfs.begin(0); l1mu != l1mtfs.end(0);  ++l1mu){ // considering BX = only

    edm::Ref< RegionalMuonCandBxCollection > l1muRef( muonH, imu );
    imu++;

    float l1mu_eta = l1mu->hwEta()*0.010875;
    // get the global phi
    float l1mu_phi = MicroGMTConfiguration::calcGlobalPhi( l1mu->hwPhi(), l1mu->trackFinderType(), l1mu->processor() )*2*M_PI/576.;

    float l1mu_feta = fabs( l1mu_eta );
    if (l1mu_feta < ETAMIN_) continue;
    if (l1mu_feta > ETAMAX_) continue;


    float drmin = 999;
    float ptmax = -1;
    if (ptmax < 0) ptmax = -1;	// dummy

    PropState matchProp;
    int match_idx = -1;
    int il1tk = -1;

    int nTracksMatch=0;

    for (const auto& l1tk : l1tks ){
      il1tk++;

      // unsigned int nPars = 4;
      // if (use5ParameterFit_) nPars = 5;
      float l1tk_pt = l1tk.momentum().perp();
      if (l1tk_pt < PTMINTRA_) continue;

      float l1tk_z  = l1tk.POCA().z();
      if (fabs(l1tk_z) > ZMAX_) continue;

      float l1tk_chi2 = l1tk.chi2();
      if (l1tk_chi2 > CHI2MAX_) continue;

      int l1tk_nstubs = l1tk.getStubRefs().size();
      if ( l1tk_nstubs < nStubsmin_) continue;

      float l1tk_eta = l1tk.momentum().eta();
      float l1tk_phi = l1tk.momentum().phi();

      float dr2 = deltaR2(l1mu_eta, l1mu_phi, l1tk_eta, l1tk_phi);
      if (dr2 > 0.3) continue;

      nTracksMatch++;

      const PropState& pstate = propagateToGMT(l1tk);
      if (!pstate.valid) continue;

      float dr2prop = deltaR2(l1mu_eta, l1mu_phi, pstate.eta, pstate.phi);
      // FIXME: check if this matching procedure can be improved with
      // a pT dependent dR window
      if (dr2prop < drmin){
        drmin = dr2prop;
        match_idx = il1tk;
        matchProp = pstate;
      }
    }// over l1tks

    LogDebug("MYDEBUG")<<"matching index is "<<match_idx;
    if (match_idx >= 0){
      const L1TTTrackType& matchTk = l1tks[match_idx];

      float sigmaEta = sigmaEtaTP(*l1mu);
      float sigmaPhi = sigmaPhiTP(*l1mu);

      float etaCut = 3.*sqrt(sigmaEta*sigmaEta + matchProp.sigmaEta*matchProp.sigmaEta);
      float phiCut = 4.*sqrt(sigmaPhi*sigmaPhi + matchProp.sigmaPhi*matchProp.sigmaPhi);

      float dEta = std::abs(matchProp.eta - l1mu_eta);
      float dPhi = std::abs(deltaPhi(matchProp.phi, l1mu_phi));

      // LogDebug("MYDEBUG")<<"match details: prop "<<matchProp.pt<<" "<<matchProp.eta<<" "<<matchProp.phi
			//  <<" mutk "<<l1mu->pt()<<" "<<l1mu->eta()<<" "<<l1mu->phi()<<" delta "<<dEta<<" "<<dPhi<<" cut "<<etaCut<<" "<<phiCut;

      bool matchCondition = useTPMatchWindows_ ? dEta < etaCut && dPhi < phiCut : drmin < DRmax_;

      if (matchCondition){
        edm::Ptr< L1TTTrackType > l1tkPtr(l1tksH, match_idx);

        // unsigned int nPars = 4;
        // if (use5ParameterFit_) nPars = 5;
        const auto& p3 = matchTk.momentum();
        float p4e = sqrt(0.105658369*0.105658369 + p3.mag2() );

        math::XYZTLorentzVector l1tkp4(p3.x(), p3.y(), p3.z(), p4e);

        const auto& tkv3=matchTk.POCA();
        math::XYZPoint v3(tkv3.x(), tkv3.y(), tkv3.z());  // why is this defined?

        float trkisol = -999;

        TkMuon l1tkmu(l1tkp4, l1muRef, l1tkPtr, trkisol);

//        std::cout<<"carga?"<<matchTk.rInv(nPars)<<std::endl;
        l1tkmu.setTrackCurvature(matchTk.rInv());
        l1tkmu.setTrkzVtx( (float)tkv3.z() );
        l1tkmu.setdR(drmin);
        l1tkmu.setNTracksMatched(nTracksMatch);
        l1tkmu.setMuonDetector(detector);
        tkMuons.push_back(l1tkmu);
      }
    }
  }//over l1mus
}


void
L1TkMuonProducer::runOnMTFCollection_v2(const edm::Handle<EMTFTrackCollection>& muonH,
                                     const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                     TkMuonCollection& tkMuons) const
{
  const EMTFTrackCollection& l1mus = (*muonH.product());
  const L1TTTrackCollectionType& l1trks = (*l1tksH.product());
  auto corr_mu_idxs = dwcorr_->find_match(l1mus, l1trks);
  // it's a vector with as many entries as the L1TT vector.
  // >= 0 : the idx in the EMTF vector of matched mu
  // < 0: no match

  // sanity check
  if (corr_mu_idxs.size() != l1trks.size())
    throw cms::Exception("TkMuAlgoOutput") << "the size of tkmu indices does not match the size of input trk collection\n";

  for (uint il1ttrack = 0; il1ttrack < corr_mu_idxs.size(); ++il1ttrack)
  {
    int emtf_idx = corr_mu_idxs.at(il1ttrack);
    if (emtf_idx < 0)
      continue;

    const L1TTTrackType& matchTk = l1trks[il1ttrack];
    const auto& p3 = matchTk.momentum();
    const auto& tkv3 = matchTk.POCA();
    float p4e = sqrt(0.105658369*0.105658369 + p3.mag2() );
    math::XYZTLorentzVector l1tkp4(p3.x(), p3.y(), p3.z(), p4e);

    edm::Ref< RegionalMuonCandBxCollection > l1muRef; // FIXME! The reference to the muon is null
    edm::Ptr< L1TTTrackType > l1tkPtr(l1tksH, il1ttrack);
    float trkisol = -999; // FIXME: now doing as in the TP algo
    TkMuon l1tkmu(l1tkp4, l1muRef, l1tkPtr, trkisol);
    l1tkmu.setTrackCurvature(matchTk.rInv());
    l1tkmu.setTrkzVtx( (float)tkv3.z() );
    l1tkmu.setMuonDetector(3);
    tkMuons.push_back(l1tkmu);
  }


  return;
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TkMuonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


L1TkMuonProducer::PropState L1TkMuonProducer::propagateToGMT(const L1TkMuonProducer::L1TTTrackType& tk) const
{
  auto p3 = tk.momentum();
  float tk_pt = p3.perp();
  float tk_p = p3.mag();
  float tk_eta = p3.eta();
  float tk_aeta = std::abs(tk_eta);
  float tk_phi = p3.phi();
  float tk_q = tk.rInv()>0? 1.: -1.;
  float tk_z  = tk.POCA().z();
  if (!correctGMTPropForTkZ_) tk_z = 0;

  L1TkMuonProducer::PropState dest;
  if (tk_p<3.5 ) return dest;
  if (tk_aeta <1.1 && tk_pt < 3.5) return dest;
  if (tk_aeta > 2.5) return dest;

  //0th order:
  dest.valid = true;

  float dzCorrPhi = 1.;
  float deta = 0;
  float etaProp = tk_aeta;

  if (tk_aeta < 1.1){
    etaProp = 1.1;
    deta = tk_z/550./cosh(tk_aeta);
  } else {
    float delta = tk_z/850.; //roughly scales as distance to 2nd station
    if (tk_eta > 0) delta *=-1;
    dzCorrPhi = 1. + delta;

    float zOzs = tk_z/850.;
    if (tk_eta > 0) deta = zOzs/(1. - zOzs);
    else deta = zOzs/(1.+zOzs);
    deta = deta*tanh(tk_eta);
  }
  float resPhi = tk_phi - 1.464*tk_q*cosh(1.7)/cosh(etaProp)/tk_pt*dzCorrPhi - M_PI/144.;
  if (resPhi > M_PI) resPhi -= 2.*M_PI;
  if (resPhi < -M_PI) resPhi += 2.*M_PI;

  dest.eta = tk_eta + deta;
  dest.phi = resPhi;
  dest.pt = tk_pt; //not corrected for eloss

  dest.sigmaEta = 0.100/tk_pt; //multiple scattering term
  dest.sigmaPhi = 0.106/tk_pt; //need a better estimate for these
  return dest;
}

double L1TkMuonProducer::sigmaEtaTP(const RegionalMuonCand& l1mu) const
{
  float l1mu_eta = l1mu.hwEta()*0.010875;
  if (std::abs(l1mu_eta) <= 1.55) return 0.0288;
  else if (std::abs(l1mu_eta) > 1.55 && std::abs(l1mu_eta) <= 1.65) return 0.025;
  else if (std::abs(l1mu_eta) > 1.65 && std::abs(l1mu_eta) <= 2.4) return 0.0144;
  return 0.0288;
}

double L1TkMuonProducer::sigmaPhiTP(const RegionalMuonCand& mu) const
{
  return 0.0126;
}

// ------------------------------------------------------------------------------------------------------------

std::vector<L1TkMuMantraDF::track_df> L1TkMuonProducer::product_to_trkvec(const L1TTTrackCollectionType& l1tks)
{
  std::vector<L1TkMuMantraDF::track_df> result (l1tks.size());
  for (uint itrk = 0; itrk < l1tks.size(); ++itrk)
  {
    auto& trk = l1tks.at(itrk);

    result.at(itrk).pt      =  trk.momentum().perp();
    result.at(itrk).eta     =  trk.momentum().eta();
    result.at(itrk).theta   =  L1TkMuMantra::to_mpio2_pio2(L1TkMuMantra::eta_to_theta(trk.momentum().eta()));
    result.at(itrk).phi     =  trk.momentum().phi();
    result.at(itrk).nstubs  =  trk.getStubRefs().size();
    result.at(itrk).chi2    =  trk.chi2();
    result.at(itrk).charge  =  (trk.rInv() > 0 ? 1 : -1);
  }

  return result;
}

std::vector<L1TkMuMantraDF::muon_df>  L1TkMuonProducer::product_to_muvec (const RegionalMuonCandBxCollection& l1mtfs)
{
  std::vector<L1TkMuMantraDF::muon_df> result;
  for (auto l1mu = l1mtfs.begin(0); l1mu != l1mtfs.end(0);  ++l1mu) // considering BX = 0 only
  {
    L1TkMuMantraDF::muon_df this_mu;
    this_mu.pt     = l1mu->hwPt() * 0.5;
    this_mu.eta    = l1mu->hwEta()*0.010875;
    this_mu.theta  = L1TkMuMantra::to_mpio2_pio2(L1TkMuMantra::eta_to_theta(l1mu->hwEta()*0.010875));
    this_mu.phi    = MicroGMTConfiguration::calcGlobalPhi( l1mu->hwPhi(), l1mu->trackFinderType(), l1mu->processor() )*2*M_PI/576.;
    this_mu.charge = (l1mu->hwSign() == 0 ? 1 : -1); // charge sign bit (charge = (-1)^(sign))
    result.push_back(this_mu);
  }
  return result;
}

std::vector<L1TkMuMantraDF::muon_df>  L1TkMuonProducer::product_to_muvec (const EMTFTrackCollection& l1mus)
{
  std::vector<L1TkMuMantraDF::muon_df> result (l1mus.size());
  for (uint imu = 0; imu < l1mus.size(); ++imu)
  {
    auto& mu = l1mus.at(imu);
    result.at(imu).pt     = mu.Pt();
    result.at(imu).eta    = mu.Eta();
    result.at(imu).theta  = L1TkMuMantra::to_mpio2_pio2(L1TkMuMantra::eta_to_theta(mu.Eta()));
    result.at(imu).phi    = L1TkMuMantra::deg_to_rad(mu.Phi_glob());
    result.at(imu).charge = mu.Charge();
  }
  return result;
}

void L1TkMuonProducer::build_tkMuons_from_idxs (TkMuonCollection& tkMuons,
                                const std::vector<int>& matches,
                                const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                const edm::Handle<RegionalMuonCandBxCollection>& muonH,
                                int detector) const
{

  for (uint imatch = 0; imatch < matches.size(); ++imatch)
  {
    int match_trk_idx = matches.at(imatch);
    if (match_trk_idx < 0) continue; // this muon was not matched to any candidate

    // take properties of the track
    const L1TTTrackType& matchTk = (*l1tksH.product())[match_trk_idx];
    const auto& p3 = matchTk.momentum();
    const auto& tkv3 = matchTk.POCA();
    float p4e = sqrt(0.105658369*0.105658369 + p3.mag2() );
    math::XYZTLorentzVector l1tkp4(p3.x(), p3.y(), p3.z(), p4e);


    edm::Ptr< L1TTTrackType > l1tkPtr(l1tksH, match_trk_idx);
    edm::Ref< RegionalMuonCandBxCollection > l1muRef(muonH, imatch);

    float trkisol = -999; // FIXME
    TkMuon l1tkmu(l1tkp4, l1muRef, l1tkPtr, trkisol);
    l1tkmu.setTrackCurvature(matchTk.rInv());
    l1tkmu.setTrkzVtx( (float)tkv3.z() );
    l1tkmu.setMuonDetector(detector);
    tkMuons.push_back(l1tkmu);
  }
  return;
}


void L1TkMuonProducer::build_tkMuons_from_idxs (TkMuonCollection& tkMuons,
                                const std::vector<int>& matches,
                                const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                int detector) const
{
  for (uint imatch = 0; imatch < matches.size(); ++imatch)
  {
    int match_trk_idx = matches.at(imatch);
    if (match_trk_idx < 0) continue; // this muon was not matched to any candidate

    // take properties of the track
    const L1TTTrackType& matchTk = (*l1tksH.product())[match_trk_idx];
    const auto& p3 = matchTk.momentum();
    const auto& tkv3 = matchTk.POCA();
    float p4e = sqrt(0.105658369*0.105658369 + p3.mag2() );
    math::XYZTLorentzVector l1tkp4(p3.x(), p3.y(), p3.z(), p4e);

    edm::Ptr< L1TTTrackType > l1tkPtr(l1tksH, match_trk_idx);
    edm::Ref< RegionalMuonCandBxCollection > l1muRef; // NOTE: this is the only difference from the function above, but could not find a way to make a conditional constructor

    float trkisol = -999; // FIXME
    TkMuon l1tkmu(l1tkp4, l1muRef, l1tkPtr, trkisol);
    l1tkmu.setTrackCurvature(matchTk.rInv());
    l1tkmu.setTrkzVtx( (float)tkv3.z() );
    l1tkmu.setMuonDetector(detector);
    tkMuons.push_back(l1tkmu);
  }
  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TkMuonProducer);



