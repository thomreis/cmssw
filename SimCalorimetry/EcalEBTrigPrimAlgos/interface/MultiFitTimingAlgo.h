#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_MultiFitTimingAlgo_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_MultiFitTimingAlgo_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

namespace ecalPh2 {

class MultiFitTimingAlgo {
 public:
  MultiFitTimingAlgo(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup) : ecalBcpPayloadParamsHelper_(ecalBcpPayloadParamsHelper) {};
  virtual ~MultiFitTimingAlgo() {};

  virtual void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) = 0;

 protected:
  // object holding the configuration
  const std::shared_ptr<EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper_;
};

} // namespace ecalPh2
#endif
