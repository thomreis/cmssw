// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
//
// class declaration
//

class FilterMatchingMuonEvents : public edm::EDFilter {
   public:
      explicit FilterMatchingMuonEvents(const edm::ParameterSet&);
      ~FilterMatchingMuonEvents();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      bool checkIfValidationEvent(const l1t::RegionalMuonCandBxCollection& muons);
      bool rangeCheckRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons);
      bool rangeCheckMuon(const l1t::MuonBxCollection& muons);
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      bool applyFilter_;
      bool invertDecision_;

      edm::InputTag bmtfTag_;
      edm::InputTag omtfTag_;
      edm::InputTag emtfTag_;
      edm::InputTag ugmtEmulatorTag_;
      edm::InputTag ugmtUnpackerTag_;

      bool analyzeBmtf_;
      bool analyzeOmtf_;
      bool analyzeEmtf_;

      edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> bmtfToken_;
      edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> omtfToken_;
      edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> emtfToken_;
      edm::EDGetTokenT<l1t::MuonBxCollection> ugmtEmulatorToken_;
      edm::EDGetTokenT<l1t::MuonBxCollection> ugmtUnpackerToken_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FilterMatchingMuonEvents::FilterMatchingMuonEvents(const edm::ParameterSet& iConfig) :
  applyFilter_(iConfig.getParameter<bool>("applyFilter")),
  invertDecision_(iConfig.getParameter<bool>("invertDecision")),
  bmtfTag_(iConfig.getParameter<edm::InputTag>("bmtfTag")),
  omtfTag_(iConfig.getParameter<edm::InputTag>("omtfTag")),
  emtfTag_(iConfig.getParameter<edm::InputTag>("emtfTag")),
  ugmtEmulatorTag_(iConfig.getParameter<edm::InputTag>("ugmtEmulatorTag")),
  ugmtUnpackerTag_(iConfig.getParameter<edm::InputTag>("ugmtUnpackerTag"))
{
  if (bmtfTag_.label() != "none") {
    analyzeBmtf_ = true;
    bmtfToken_  = consumes<l1t::RegionalMuonCandBxCollection>(bmtfTag_);
  }
  if (omtfTag_.label() != "none") {
    analyzeOmtf_ = true;
    omtfToken_ = consumes<l1t::RegionalMuonCandBxCollection>(omtfTag_);
  }
  if (emtfTag_.label() != "none") {
    analyzeEmtf_ = true;
    emtfToken_ = consumes<l1t::RegionalMuonCandBxCollection>(emtfTag_);
  }
  ugmtEmulatorToken_ = consumes<l1t::MuonBxCollection>(ugmtEmulatorTag_);
  ugmtUnpackerToken_ = consumes<l1t::MuonBxCollection>(ugmtUnpackerTag_);
}


FilterMatchingMuonEvents::~FilterMatchingMuonEvents()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
FilterMatchingMuonEvents::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool mismatch = false;
  bool rangeGood = true;

  edm::Handle<l1t::MuonBxCollection> ugmtEmulatorMuons;
  edm::Handle<l1t::MuonBxCollection> ugmtUnpackerMuons;
  iEvent.getByToken(ugmtEmulatorToken_, ugmtEmulatorMuons);
  iEvent.getByToken(ugmtUnpackerToken_, ugmtUnpackerMuons);
  if (ugmtEmulatorMuons.isValid() && ugmtUnpackerMuons.isValid()) {
    rangeGood &= rangeCheckMuon(*ugmtEmulatorMuons);
    rangeGood &= rangeCheckMuon(*ugmtUnpackerMuons);

    // event by event comparison

    // find out if this is a validation event
    bool valEvt = false;
    if (analyzeBmtf_) {
      edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
      iEvent.getByToken(bmtfToken_, bmtfMuons);
      if (bmtfMuons.isValid()) {
        rangeGood &= rangeCheckRegionalMuonCand(*bmtfMuons);
        if (checkIfValidationEvent(*bmtfMuons)) {
          valEvt = true;
        }
      }
    } 
    if (analyzeOmtf_) {
      edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
      iEvent.getByToken(omtfToken_, omtfMuons);
      if (omtfMuons.isValid()) {
        rangeGood &= rangeCheckRegionalMuonCand(*omtfMuons);
        if (!valEvt && checkIfValidationEvent(*omtfMuons)) {
          valEvt = true;
        }
      }
    } 
    if (analyzeEmtf_) {
      edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
      iEvent.getByToken(emtfToken_, emtfMuons);
      if (emtfMuons.isValid()) {
        rangeGood &= rangeCheckRegionalMuonCand(*emtfMuons);
        if (!valEvt && checkIfValidationEvent(*emtfMuons)) {
          valEvt = true;
        }
      }
    }

    // check BX ranges of HW and emulator collections
    if (valEvt && (ugmtUnpackerMuons->getFirstBX() != ugmtEmulatorMuons->getFirstBX() || ugmtUnpackerMuons->getLastBX() != ugmtEmulatorMuons->getLastBX())) {
      valEvt = false;
      edm::LogWarning("MuonMismatchFilter") << "HW BX range and emuator BX range do not match for validation event. Override validation event flag and only analyse BX 0. Was an input TF masked or disabled?";
    }

    for (int bx = ugmtUnpackerMuons->getFirstBX(); bx <= ugmtUnpackerMuons->getLastBX(); ++bx) {
      if (!valEvt && bx != 0) continue;

      if (ugmtUnpackerMuons->size(bx) != ugmtEmulatorMuons->size(bx)) {
        mismatch = true;
      } else {
        unsigned int ctr = 0;
        auto hwMu = ugmtUnpackerMuons->begin(bx);
        auto emulMu = ugmtEmulatorMuons->begin(bx);
        while (hwMu != ugmtUnpackerMuons->end(bx) && emulMu != ugmtEmulatorMuons->end(bx) && ctr < ugmtUnpackerMuons->size(bx)) {
          if (hwMu->hwPt() != emulMu->hwPt()) {
            mismatch = true;
          } else if (hwMu->hwEta() != emulMu->hwEta()) {
            mismatch = true;
          } else if (hwMu->hwPhi() != emulMu->hwPhi()) {
            mismatch = true;
          } else if (hwMu->hwQual() != emulMu->hwQual()) {
            mismatch = true;
          } else if (hwMu->hwCharge() != emulMu->hwCharge()) {
            mismatch = true;
          } else if (hwMu->hwChargeValid() != emulMu->hwChargeValid()) {
            mismatch = true;
          } else if (hwMu->tfMuonIndex() != emulMu->tfMuonIndex()) {
            mismatch = true;
          } else if (hwMu->hwIso() != emulMu->hwIso()) {
            mismatch = true;
          }
          ++hwMu;
          ++emulMu;
          ++ctr;
        }
      }
    }
  }

  if (applyFilter_) {
    if (invertDecision_) {
      return !(mismatch || !rangeGood);
    } else {
      return (mismatch || !rangeGood);
    }
  } else {
    return true;
  }
}

bool
FilterMatchingMuonEvents::checkIfValidationEvent(const l1t::RegionalMuonCandBxCollection& muons)
{
  bool valEvt = false;
  int firstBx = muons.getFirstBX();
  int lastBx = muons.getLastBX();
  if (lastBx - firstBx == 4) { // val events have BX range from -2 to +2
    valEvt = true;
  }
  return valEvt;
}


bool
FilterMatchingMuonEvents::rangeCheckRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons)
{
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    unsigned int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < muons.size(bx); ++mu, ++ctr) {
      if (mu->hwPt() < 0 || mu->hwPt() > 511) {
        return false;
      }
      if (mu->hwEta() < -230 || mu->hwEta() > 230) {
        return false;
      }
      if (mu->trackFinderType() == 0) {
        if (mu->hwPhi() < -8 || mu->hwPhi() > 56) {
          return false;
        }
      } else if (mu->trackFinderType() > 2) { // EMTF
        if (mu->hwPhi() < -35 || mu->hwPhi() > 100) {
          return false;
        }
      } else { // OMTF
        if (mu->hwPhi() < -16 || mu->hwPhi() > 101) {
          return false;
        }
      }
    }
  }
  return true;
}

bool
FilterMatchingMuonEvents::rangeCheckMuon(const l1t::MuonBxCollection& muons)
{
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    unsigned int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < muons.size(bx); ++mu, ++ctr) {
      if (mu->hwPt() < 0 || mu->hwPt() > 511) {
        return false;
      }
      if (mu->hwEta() < -230 || mu->hwEta() > 230) {
        return false;
      }
      if (mu->hwPhi() < 0 || mu->hwPhi() > 575) {
        return false;
      }
    }
  }
  return true;
}

// ------------ method called once each job just before starting event loop  ------------
void
FilterMatchingMuonEvents::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
FilterMatchingMuonEvents::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
FilterMatchingMuonEvents::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
FilterMatchingMuonEvents::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
FilterMatchingMuonEvents::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
FilterMatchingMuonEvents::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FilterMatchingMuonEvents::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(FilterMatchingMuonEvents);
