// -*- C++ -*-
//
// L1TRegionalMuonCandSorter
//
// Module to produce RegionalMuonCands in the order the uGMT expects them to arrive
// BX N
//   (neg side)
//     processor 0, mu 0
//     processor 0, mu 1
//     processor 0, mu 2
//     processor 1, mu 0
//     processor 1, mu 1
//     processor 1, mu 2
//     processor 2, mu 0
//       ...
//   (pos side)
//     processor 0, mu 0
//       ...
// BX N+1
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

#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"

using namespace l1t;

#include <iostream>
//
// class declaration
//

class L1TRegionalMuonCandSorter : public edm::EDProducer {
public:
  explicit L1TRegionalMuonCandSorter(const edm::ParameterSet&);
  ~L1TRegionalMuonCandSorter();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  // ----------member data ---------------------------
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_barrelTfInputToken;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_overlapTfInputToken;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_endCapTfInputToken;
  edm::InputTag m_barrelTfInputTag;
  edm::InputTag m_overlapTfInputTag;
  edm::InputTag m_endCapTfInputTag;
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
L1TRegionalMuonCandSorter::L1TRegionalMuonCandSorter(const edm::ParameterSet& iConfig)
{
  m_barrelTfInputTag  = iConfig.getParameter<edm::InputTag>("bmtfInput");
  m_overlapTfInputTag = iConfig.getParameter<edm::InputTag>("omtfInput");
  m_endCapTfInputTag  = iConfig.getParameter<edm::InputTag>("emtfInput");
  m_barrelTfInputToken  = consumes<l1t::RegionalMuonCandBxCollection>(m_barrelTfInputTag);
  m_overlapTfInputToken = consumes<l1t::RegionalMuonCandBxCollection>(m_overlapTfInputTag);
  m_endCapTfInputToken  = consumes<l1t::RegionalMuonCandBxCollection>(m_endCapTfInputTag);
  //register your products
  produces<RegionalMuonCandBxCollection>("BMTF");
  produces<RegionalMuonCandBxCollection>("OMTF");
  produces<RegionalMuonCandBxCollection>("EMTF");
}


L1TRegionalMuonCandSorter::~L1TRegionalMuonCandSorter()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//


// ------------ method called to produce the data  ------------
void
L1TRegionalMuonCandSorter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  std::unique_ptr<l1t::RegionalMuonCandBxCollection> sortedBMTFMuons (new l1t::RegionalMuonCandBxCollection());
  std::unique_ptr<l1t::RegionalMuonCandBxCollection> sortedOMTFMuons (new l1t::RegionalMuonCandBxCollection());
  std::unique_ptr<l1t::RegionalMuonCandBxCollection> sortedEMTFMuons (new l1t::RegionalMuonCandBxCollection());

  Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
  Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
  Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;

  iEvent.getByToken(m_barrelTfInputToken, bmtfMuons);
  iEvent.getByToken(m_overlapTfInputToken, omtfMuons);
  iEvent.getByToken(m_endCapTfInputToken, emtfMuons);

  
  if (bmtfMuons.isValid()){
    sortedBMTFMuons->setBXRange(bmtfMuons->getFirstBX(), bmtfMuons->getLastBX());
    for (int bx = bmtfMuons->getFirstBX(); bx <= bmtfMuons->getLastBX(); ++bx) {
      int highestLinkNumber = 0;
      std::map<int, std::vector<l1t::RegionalMuonCand> > muonsMap;
      // fill the muons map
      for (auto mu = bmtfMuons->begin(bx); mu != bmtfMuons->end(bx); ++mu) {
        muonsMap[mu->processor()].push_back((*mu));
        if (mu->link() < highestLinkNumber) {
          edm::LogWarning("L1T") << "TF muons from module " << m_barrelTfInputTag.label() << " are not ordered by ascending link number as expected by the uGMT. Link number of this muon: " << mu->link() << ", highest link number so far: " << highestLinkNumber;
        } else {
          highestLinkNumber = mu->link();
        }
      }
      // write the output muons in the correct order
      for (int proc = 0; proc < 12; ++proc) {
        for (auto mu = muonsMap[proc].begin(); mu != muonsMap[proc].end(); ++mu) {
          l1t::RegionalMuonCand newMu((*mu));
          sortedBMTFMuons->push_back(bx, newMu);
        }
      }
    }
  } else {
    //cout << "ERROR:  did not find BMTF muons in event with label:  " << m_barrelTfInputTag << "\n";
  }

  if (emtfMuons.isValid()){
    sortedEMTFMuons->setBXRange(emtfMuons->getFirstBX(), emtfMuons->getLastBX());
    for (int bx = emtfMuons->getFirstBX(); bx <= emtfMuons->getLastBX(); ++bx) {
      int highestLinkNumber = 0;
      std::map<int, std::vector<l1t::RegionalMuonCand> > muonsMapPos;
      std::map<int, std::vector<l1t::RegionalMuonCand> > muonsMapNeg;
      // fill the muons map
      for (auto mu = emtfMuons->begin(bx); mu != emtfMuons->end(bx); ++mu) {
        if (mu->trackFinderType() == tftype::emtf_pos) {
          muonsMapPos[mu->processor()].push_back((*mu));
        } else {
          muonsMapNeg[mu->processor()].push_back((*mu));
        }
        if (mu->link() < highestLinkNumber) {
          edm::LogWarning("L1T") << "TF muons from module " << m_endCapTfInputTag.label() << " are not ordered by ascending link number as expected by the uGMT. Link number of this muon: " << mu->link() << ", highest link number so far: " << highestLinkNumber;
        } else {
          highestLinkNumber = mu->link();
        }
      }
      // write the output muons in the correct order (positive side first)
      for (int proc = 0; proc < 6; ++proc) {
        for (auto mu = muonsMapPos[proc].begin(); mu != muonsMapPos[proc].end(); ++mu) {
          l1t::RegionalMuonCand newMu((*mu));
          sortedEMTFMuons->push_back(bx, newMu);
        }
      }
      for (int proc = 0; proc < 6; ++proc) {
        for (auto mu = muonsMapNeg[proc].begin(); mu != muonsMapNeg[proc].end(); ++mu) {
          l1t::RegionalMuonCand newMu((*mu));
          sortedEMTFMuons->push_back(bx, newMu);
        }
      }
    }
  } else {
    //cout << "ERROR:  did not find EMTF muons in event with label:  " << m_endCapTfInputTag << "\n";
  }

  if (omtfMuons.isValid()){
    sortedOMTFMuons->setBXRange(omtfMuons->getFirstBX(), omtfMuons->getLastBX());
    for (int bx = omtfMuons->getFirstBX(); bx <= omtfMuons->getLastBX(); ++bx) {
      int highestLinkNumber = 0;
      std::map<int, std::vector<l1t::RegionalMuonCand> > muonsMapPos;
      std::map<int, std::vector<l1t::RegionalMuonCand> > muonsMapNeg;
      // fill the muons map
      for (auto mu = omtfMuons->begin(bx); mu != omtfMuons->end(bx); ++mu) {
        if (mu->trackFinderType() == tftype::omtf_pos) {
          muonsMapPos[mu->processor()].push_back((*mu));
        } else {
          muonsMapNeg[mu->processor()].push_back((*mu));
        }
        if (mu->link() < highestLinkNumber) {
          edm::LogWarning("L1T") << "TF muons from module " << m_overlapTfInputTag.label() << " are not ordered by ascending link number as expected by the uGMT. Link number of this muon: " << mu->link() << ", highest link number so far: " << highestLinkNumber;
        } else {
          highestLinkNumber = mu->link();
        }
      }
      // write the output muons in the correct order (positive side first)
      for (int proc = 0; proc < 6; ++proc) {
        for (auto mu = muonsMapPos[proc].begin(); mu != muonsMapPos[proc].end(); ++mu) {
          l1t::RegionalMuonCand newMu((*mu));
          sortedOMTFMuons->push_back(bx, newMu);
        }
      }
      for (int proc = 0; proc < 6; ++proc) {
        for (auto mu = muonsMapNeg[proc].begin(); mu != muonsMapNeg[proc].end(); ++mu) {
          l1t::RegionalMuonCand newMu((*mu));
          sortedOMTFMuons->push_back(bx, newMu);
        }
      }
    } 
  } else {
    //cout << "ERROR:  did not find OMTF muons in event with label:  " << m_overlapTfInputTag << "\n";
  }

  //cout << "Size BMTF:  " << sortedBMTFMuons->size(0) << "\n";
  //cout << "Size OMTF:  " << sortedOMTFMuons->size(0) << "\n";
  //cout << "Size EMTF:  " << sortedEMTFMuons->size(0) << "\n";

  iEvent.put(std::move(sortedBMTFMuons), "BMTF");
  iEvent.put(std::move(sortedOMTFMuons), "OMTF");
  iEvent.put(std::move(sortedEMTFMuons), "EMTF");
}

// ------------ method called once each job just before starting event loop  ------------
void
L1TRegionalMuonCandSorter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1TRegionalMuonCandSorter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void
L1TRegionalMuonCandSorter::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
L1TRegionalMuonCandSorter::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
L1TRegionalMuonCandSorter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
L1TRegionalMuonCandSorter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TRegionalMuonCandSorter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(L1TRegionalMuonCandSorter);
