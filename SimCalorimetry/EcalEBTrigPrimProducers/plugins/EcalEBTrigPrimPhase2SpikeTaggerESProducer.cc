// -*- C++ -*-
//
// Package:    SimCalorimetry/EcalEBTrigPrimProducers
// Class:      EcalEBTrigPrimPhase2SpikeTaggerESProducer
//
/**\class EcalEBTrigPrimPhase2SpikeTaggerESProducer

 Description: Produces the configuration parameters for the BCP payload

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis <Thomas.Reis@stfc.ac.uk>
//         Created:  Tue, 19 Nov 2019 16:15:26 GMT
//
//

// system include files
#include <iostream>
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/EcalEBPhase2TPGSpikeTaggerParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalEBPhase2TPGSpikeTaggerParamsHelper.h"

//
// class declaration
//

class EcalEBTrigPrimPhase2SpikeTaggerESProducer : public edm::ESProducer {
public:
  EcalEBTrigPrimPhase2SpikeTaggerESProducer(const edm::ParameterSet&);
  ~EcalEBTrigPrimPhase2SpikeTaggerESProducer() override;

  using ReturnType = std::unique_ptr<EcalEBPhase2TPGSpikeTaggerParams>;

  ReturnType produce(const EcalEBPhase2TPGSpikeTaggerParamsRcd&);

private:
  EcalEBPhase2TPGSpikeTaggerParams params_;
};

//
// constructors and destructor
//
EcalEBTrigPrimPhase2SpikeTaggerESProducer::EcalEBTrigPrimPhase2SpikeTaggerESProducer(const edm::ParameterSet& iConfig) {
  setWhatProduced(this);

  EcalEBPhase2TPGSpikeTaggerParamsHelper paramsHelper(iConfig);
  params_ = static_cast<EcalEBPhase2TPGSpikeTaggerParams>(paramsHelper);
}

EcalEBTrigPrimPhase2SpikeTaggerESProducer::~EcalEBTrigPrimPhase2SpikeTaggerESProducer() {}

//
// member functions
//

// ------------ method called to produce the data  ------------
EcalEBTrigPrimPhase2SpikeTaggerESProducer::ReturnType EcalEBTrigPrimPhase2SpikeTaggerESProducer::produce(
    const EcalEBPhase2TPGSpikeTaggerParamsRcd& iRecord) {
  auto product = std::make_unique<EcalEBPhase2TPGSpikeTaggerParams>(params_);
  return product;
}

//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(EcalEBTrigPrimPhase2SpikeTaggerESProducer);
