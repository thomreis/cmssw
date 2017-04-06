// -*- C++ -*-
//
// Package:    L1TMicroGMTEmptyCaloCollsProducer
// Class:      L1TMicroGMTEmptyCaloCollsProducer
//
/**\class L1TMicroGMTEmptyCaloCollsProducer L1TMicroGMTEmptyCaloCollsProducer.cc L1Trigger/L1TGlobalMuon/plugins/L1TMicroGMTEmptyCaloCollsProducer.cc

 Description: Produces empty Calo Collections for microGMT emulator tests

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis
// $Id$
//
//


// system include files
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1TMuon/interface/MuonCaloSumFwd.h"
#include "DataFormats/L1TMuon/interface/MuonCaloSum.h"
#include "DataFormats/L1TCalorimeter/interface/CaloTower.h"

#include "TMath.h"
#include "TRandom3.h"

//
// class declaration
//
using namespace l1t;

class L1TMicroGMTEmptyCaloCollsProducer : public edm::EDProducer {
   public:
      explicit L1TMicroGMTEmptyCaloCollsProducer(const edm::ParameterSet&);
      ~L1TMicroGMTEmptyCaloCollsProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
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
L1TMicroGMTEmptyCaloCollsProducer::L1TMicroGMTEmptyCaloCollsProducer(const edm::ParameterSet& iConfig) {
  //register your products
  produces<CaloTowerBxCollection>("EmptyCaloTowers");
  produces<MuonCaloSumBxCollection>("EmptyTriggerTowerSums");
  produces<MuonCaloSumBxCollection>("EmptyTriggerTower2x2s");
}


L1TMicroGMTEmptyCaloCollsProducer::~L1TMicroGMTEmptyCaloCollsProducer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//


// ------------ method called to produce the data  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  std::unique_ptr<CaloTowerBxCollection> caloTowers (new CaloTowerBxCollection());
  std::unique_ptr<MuonCaloSumBxCollection> towerSums (new MuonCaloSumBxCollection());
  std::unique_ptr<MuonCaloSumBxCollection> tower2x2s (new MuonCaloSumBxCollection());

  iEvent.put(std::move(caloTowers), "EmptyCaloTowers");
  iEvent.put(std::move(towerSums), "EmptyTriggerTowerSums");
  iEvent.put(std::move(tower2x2s), "EmptyTriggerTower2x2s");

}

// ------------ method called once each job just before starting event loop  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TMicroGMTEmptyCaloCollsProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TMicroGMTEmptyCaloCollsProducer);
