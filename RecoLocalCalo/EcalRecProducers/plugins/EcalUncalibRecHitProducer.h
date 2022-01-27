#ifndef RecoLocalCalo_EcalRecProducers_EcalUncalibRecHitProducer_h
#define RecoLocalCalo_EcalRecProducers_EcalUncalibRecHitProducer_h

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "RecoLocalCalo/EcalRecProducers/interface/EcalUncalibRecHitWorkerBaseClass.h"

class EBDigiCollection;
class EEDigiCollection;
class EBDigiCollectionPh2;

class EcalUncalibRecHitProducer : public edm::stream::EDProducer<> {
public:
  explicit EcalUncalibRecHitProducer(const edm::ParameterSet& ps);
  ~EcalUncalibRecHitProducer() override;
  void produce(edm::Event& evt, const edm::EventSetup& es) override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  const bool isPhase2_;

  edm::EDGetTokenT<EBDigiCollection> ebDigiCollectionToken_;
  edm::EDGetTokenT<EEDigiCollection> eeDigiCollectionToken_;
  edm::EDGetTokenT<EBDigiCollectionPh2> ebPh2DigiCollectionToken_;
  const edm::EDPutTokenT<EBUncalibratedRecHitCollection> ebHitCollectionToken_;
  edm::EDPutTokenT<EEUncalibratedRecHitCollection> eeHitCollectionToken_;

  std::unique_ptr<EcalUncalibRecHitWorkerBaseClass> worker_;
};
#endif
