#include <utility>

#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"
#include "DataFormats/EcalDetId/interface/EcalIdHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalConstants.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalSrFlagHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalPnDiodeDigiHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalPseudoStripInputDigiHostCollection.h"
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigiHostCollection.h"
#include "DataFormats/EcalRawData/interface/EcalDCCHeaderBlockHostCollection.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

class EcalDigisFromPortableProducer : public edm::stream::EDProducer<> {
public:
  explicit EcalDigisFromPortableProducer(edm::ParameterSet const& ps);
  ~EcalDigisFromPortableProducer() override = default;
  static void fillDescriptions(edm::ConfigurationDescriptions&);

  void produce(edm::Event&, edm::EventSetup const&) override;

private:
  ////////////////////////////////////////////////////////////////////////////
  // input collections
  // input digi collections on host in SoA format
  using InputProduct = EcalDigiHostCollection;
  edm::EDGetTokenT<InputProduct> digisInEBToken_;
  edm::EDGetTokenT<InputProduct> digisInEEToken_;

  // input SR flag collections on host in SoA format
  edm::EDGetTokenT<EcalSrFlagHostCollection> srFlagInEBToken_;
  edm::EDGetTokenT<EcalSrFlagHostCollection> srFlagInEEToken_;

  // input integrity DetId collections for xtal data - EB
  edm::EDGetTokenT<EcalIdHostCollection> ebIntegrityGainErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> ebIntegrityGainSwitchErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> ebIntegrityChIdErrorsInToken_;

  // input integrity DetId collections for xtal data - EE
  edm::EDGetTokenT<EcalIdHostCollection> eeIntegrityGainErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> eeIntegrityGainSwitchErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> eeIntegrityChIdErrorsInToken_;

  edm::EDGetTokenT<EcalPnDiodeDigiHostCollection> pnDiodeDigisInToken_;

  // input TCC collections
  edm::EDGetTokenT<EcalTriggerPrimitiveDigiHostCollection> ecalTriggerPrimitivesInToken_;
  edm::EDGetTokenT<EcalPseudoStripInputDigiHostCollection> ecalPseudoStripInputsInToken_;

  // input integrity errors
  edm::EDGetTokenT<EcalIdHostCollection> integrityTTIdErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> integrityZSXtalIdErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> integrityBlockSizeErrorsInToken_;

  // input mem integrity collections
  edm::EDGetTokenT<EcalIdHostCollection> integrityMemTtIdErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> integrityMemBlockSizeErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> integrityMemChIdErrorsInToken_;
  edm::EDGetTokenT<EcalIdHostCollection> integrityMemGainErrorsInToken_;

  // input ECAL raw data collection
  edm::EDGetTokenT<EcalDCCHeaderBlockHostCollection> ecalRawDataInToken_;

  ////////////////////////////////////////////////////////////////////////////
  // output collections
  // output digi collections in legacy format
  edm::EDPutTokenT<EBDigiCollection> digisOutEBToken_;
  edm::EDPutTokenT<EEDigiCollection> digisOutEEToken_;

  // output SR flag collections in legacy format
  edm::EDPutTokenT<EBSrFlagCollection> srFlagsOutEBToken_;
  edm::EDPutTokenT<EESrFlagCollection> srFlagsOutEEToken_;

  // output integrity DetId collections for xtal data - EB
  edm::EDPutTokenT<EBDetIdCollection> ebIntegrityGainErrorsOutToken_;
  edm::EDPutTokenT<EBDetIdCollection> ebIntegrityGainSwitchErrorsOutToken_;
  edm::EDPutTokenT<EBDetIdCollection> ebIntegrityChIdErrorsOutToken_;

  // output integrity DetId collections for xtal data - EE
  edm::EDPutTokenT<EEDetIdCollection> eeIntegrityGainErrorsOutToken_;
  edm::EDPutTokenT<EEDetIdCollection> eeIntegrityGainSwitchErrorsOutToken_;
  edm::EDPutTokenT<EEDetIdCollection> eeIntegrityChIdErrorsOutToken_;

  edm::EDPutTokenT<EcalPnDiodeDigiCollection> pnDiodeDigisOutToken_;

  // output TCC collections
  edm::EDPutTokenT<EcalTrigPrimDigiCollection> ecalTriggerPrimitivesOutToken_;
  edm::EDPutTokenT<EcalPSInputDigiCollection> ecalPseudoStripInputsOutToken_;

  // output integrity errors
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityTTIdErrorsOutToken_;
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityZSXtalIdErrorsOutToken_;
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityBlockSizeErrorsOutToken_;

  // output mem integrity collections
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityMemTtIdErrorsOutToken_;
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityMemBlockSizeErrorsOutToken_;
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityMemChIdErrorsOutToken_;
  edm::EDPutTokenT<EcalElectronicsIdCollection> integrityMemGainErrorsOutToken_;

  // output ECAL raw data collection
  edm::EDPutTokenT<EcalRawDataCollection> ecalRawDataOutToken_;
};

void EcalDigisFromPortableProducer::fillDescriptions(edm::ConfigurationDescriptions& confDesc) {
  edm::ParameterSetDescription desc;

  desc.add<edm::InputTag>("digisInLabelEB", edm::InputTag{"ecalRawToDigiPortable", "ebDigis"});
  desc.add<edm::InputTag>("digisInLabelEE", edm::InputTag{"ecalRawToDigiPortable", "eeDigis"});
  desc.add<std::string>("digisOutLabelEB", "ebDigis");
  desc.add<std::string>("digisOutLabelEE", "eeDigis");
  desc.add<edm::InputTag>("srFlagInLabelEB", edm::InputTag{"ecalRawToDigiPortable", "ebSrFlags"});
  desc.add<edm::InputTag>("srFlagInLabelEE", edm::InputTag{"ecalRawToDigiPortable", "eeSrFlags"});
  desc.add<edm::InputTag>("integrityGainErrorsInLabelEB",
                          edm::InputTag{"ecalRawToDigiPortable", "ebIntegrityGainErrors"});
  desc.add<edm::InputTag>("integrityGainSwitchErrorsInLabelEB",
                          edm::InputTag{"ecalRawToDigiPortable", "ebIntegrityGainSwitchErrors"});
  desc.add<edm::InputTag>("integrityChIdErrorsInLabelEB",
                          edm::InputTag{"ecalRawToDigiPortable", "ebIntegrityChIdErrors"});
  desc.add<edm::InputTag>("integrityGainErrorsInLabelEE",
                          edm::InputTag{"ecalRawToDigiPortable", "eeIntegrityGainErrors"});
  desc.add<edm::InputTag>("integrityGainSwitchErrorsInLabelEE",
                          edm::InputTag{"ecalRawToDigiPortable", "eeIntegrityGainSwitchErrors"});
  desc.add<edm::InputTag>("integrityChIdErrorsInLabelEE",
                          edm::InputTag{"ecalRawToDigiPortable", "eeIntegrityChIdErrors"});
  desc.add<edm::InputTag>("pnDiodeDigisInLabel", edm::InputTag{"ecalRawToDigiPortable"});
  desc.add<edm::InputTag>("ecalTriggerPrimitivesInLabel", edm::InputTag{"ecalRawToDigiPortable"});
  desc.add<edm::InputTag>("ecalPseudoStripInputsInLabel", edm::InputTag{"ecalRawToDigiPortable"});
  desc.add<edm::InputTag>("integrityTTIdErrorsInLabel", edm::InputTag{"ecalRawToDigiPortable", "integrityTTIdErrors"});
  desc.add<edm::InputTag>("integrityZSXtalIdErrorsInLabel",
                          edm::InputTag{"ecalRawToDigiPortable", "integrityZSXtalIdErrors"});
  desc.add<edm::InputTag>("integrityBlockSizeErrorsInLabel",
                          edm::InputTag{"ecalRawToDigiPortable", "integrityBlockSizeErrors"});
  desc.add<edm::InputTag>("integrityMemTtIdErrorsInLabel",
                          edm::InputTag{"ecalRawToDigiPortable", "ecalIntegrityMemTtIdErrors"});
  desc.add<edm::InputTag>("integrityMemBlockSizeErrorsInLabel",
                          edm::InputTag{"ecalRawToDigiPortable", "ecalIntegrityMemBlockSizeErrors"});
  desc.add<edm::InputTag>("integrityMemChIdErrorsInLabel",
                          edm::InputTag{"ecalRawToDigiPortable", "ecalIntegrityMemChIdErrors"});
  desc.add<edm::InputTag>("integrityMemGainErrorsInLabel",
                          edm::InputTag{"ecalRawToDigiPortable", "ecalIntegrityMemGainErrors"});
  desc.add<edm::InputTag>("ecalRawDataInLabel", edm::InputTag{"ecalRawToDigiPortable"});

  confDesc.add("ecalDigisFromPortableProducer", desc);
}

EcalDigisFromPortableProducer::EcalDigisFromPortableProducer(const edm::ParameterSet& ps)
    :  // input collections on host in SoA format
      digisInEBToken_{consumes(ps.getParameter<edm::InputTag>("digisInLabelEB"))},
      digisInEEToken_{consumes(ps.getParameter<edm::InputTag>("digisInLabelEE"))},
      srFlagInEBToken_{consumes(ps.getParameter<edm::InputTag>("srFlagInLabelEB"))},
      srFlagInEEToken_{consumes(ps.getParameter<edm::InputTag>("srFlagInLabelEE"))},
      ebIntegrityGainErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityGainErrorsInLabelEB"))},
      ebIntegrityGainSwitchErrorsInToken_{
          consumes(ps.getParameter<edm::InputTag>("integrityGainSwitchErrorsInLabelEB"))},
      ebIntegrityChIdErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityChIdErrorsInLabelEB"))},
      eeIntegrityGainErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityGainErrorsInLabelEE"))},
      eeIntegrityGainSwitchErrorsInToken_{
          consumes(ps.getParameter<edm::InputTag>("integrityGainSwitchErrorsInLabelEE"))},
      eeIntegrityChIdErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityChIdErrorsInLabelEE"))},
      pnDiodeDigisInToken_{consumes(ps.getParameter<edm::InputTag>("pnDiodeDigisInLabel"))},
      ecalTriggerPrimitivesInToken_{consumes(ps.getParameter<edm::InputTag>("ecalTriggerPrimitivesInLabel"))},
      ecalPseudoStripInputsInToken_{consumes(ps.getParameter<edm::InputTag>("ecalPseudoStripInputsInLabel"))},
      integrityTTIdErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityTTIdErrorsInLabel"))},
      integrityZSXtalIdErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityZSXtalIdErrorsInLabel"))},
      integrityBlockSizeErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityBlockSizeErrorsInLabel"))},
      integrityMemTtIdErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityMemTtIdErrorsInLabel"))},
      integrityMemBlockSizeErrorsInToken_{
          consumes(ps.getParameter<edm::InputTag>("integrityMemBlockSizeErrorsInLabel"))},
      integrityMemChIdErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityMemChIdErrorsInLabel"))},
      integrityMemGainErrorsInToken_{consumes(ps.getParameter<edm::InputTag>("integrityMemGainErrorsInLabel"))},
      ecalRawDataInToken_{consumes(ps.getParameter<edm::InputTag>("ecalRawDataInLabel"))},

      // output collections in legacy format
      digisOutEBToken_{produces(ps.getParameter<std::string>("digisOutLabelEB"))},
      digisOutEEToken_{produces(ps.getParameter<std::string>("digisOutLabelEE"))},
      srFlagsOutEBToken_{produces()},
      srFlagsOutEEToken_{produces()},
      ebIntegrityGainErrorsOutToken_{produces("EcalIntegrityGainErrors")},
      ebIntegrityGainSwitchErrorsOutToken_{produces("EcalIntegrityGainSwitchErrors")},
      ebIntegrityChIdErrorsOutToken_{produces("EcalIntegrityChIdErrors")},
      eeIntegrityGainErrorsOutToken_{produces("EcalIntegrityGainErrors")},
      eeIntegrityGainSwitchErrorsOutToken_{produces("EcalIntegrityGainSwitchErrors")},
      eeIntegrityChIdErrorsOutToken_{produces("EcalIntegrityChIdErrors")},
      pnDiodeDigisOutToken_{produces()},
      ecalTriggerPrimitivesOutToken_{produces("EcalTriggerPrimitives")},
      ecalPseudoStripInputsOutToken_{produces("EcalPseudoStripInputs")},
      integrityTTIdErrorsOutToken_{produces("EcalIntegrityTTIdErrors")},
      integrityZSXtalIdErrorsOutToken_{produces("EcalIntegrityZSXtalIdErrors")},
      integrityBlockSizeErrorsOutToken_{produces("EcalIntegrityBlockSizeErrors")},
      integrityMemTtIdErrorsOutToken_{produces("EcalIntegrityMemTtIdErrors")},
      integrityMemBlockSizeErrorsOutToken_{produces("EcalIntegrityMemBlockSizeErrors")},
      integrityMemChIdErrorsOutToken_{produces("EcalIntegrityMemChIdErrors")},
      integrityMemGainErrorsOutToken_{produces("EcalIntegrityMemGainErrors")},
      ecalRawDataOutToken_{produces()} {}

void EcalDigisFromPortableProducer::produce(edm::Event& event, edm::EventSetup const& setup) {
  ////////////////////////////////////////////////////////////////////////////
  // digis
  auto digisEB = std::make_unique<EBDigiCollection>();
  auto digisEE = std::make_unique<EEDigiCollection>();

  auto const& digisEBSoAHostColl = event.get(digisInEBToken_);
  auto const& digisEESoAHostColl = event.get(digisInEEToken_);
  auto& digisEBSoAView = digisEBSoAHostColl.view();
  auto& digisEESoAView = digisEESoAHostColl.view();

  auto const digisEBSize = digisEBSoAView.size();
  auto const digisEESize = digisEESoAView.size();
  auto const digisEBDataSize = digisEBSize * ecalPh1::sampleSize;
  auto const digisEEDataSize = digisEESize * ecalPh1::sampleSize;

  // Intermediate containers because the DigiCollection containers are accessible only as const
  EBDigiCollection::IdContainer digisIdsEB(digisEBSoAView.id(), digisEBSoAView.id() + digisEBSize);
  EEDigiCollection::IdContainer digisIdsEE(digisEESoAView.id(), digisEESoAView.id() + digisEESize);
  EBDigiCollection::DataContainer digisDataEB(digisEBSoAView.data()->data(),
                                              digisEBSoAView.data()->data() + digisEBDataSize);
  EEDigiCollection::DataContainer digisDataEE(digisEESoAView.data()->data(),
                                              digisEESoAView.data()->data() + digisEEDataSize);

  digisEB->swap(digisIdsEB, digisDataEB);
  digisEE->swap(digisIdsEE, digisDataEE);

  digisEB->sort();
  digisEE->sort();

  event.put(digisOutEBToken_, std::move(digisEB));
  event.put(digisOutEEToken_, std::move(digisEE));

  ////////////////////////////////////////////////////////////////////////////
  // SR flags
  auto srFlagsEB = std::make_unique<EBSrFlagCollection>();
  auto srFlagsEE = std::make_unique<EESrFlagCollection>();

  auto const& srFlagEBSoAHostColl = event.get(srFlagInEBToken_);
  auto const& srFlagEESoAHostColl = event.get(srFlagInEEToken_);
  auto& srFlagEBSoAView = srFlagEBSoAHostColl.view();
  auto& srFlagEESoAView = srFlagEESoAHostColl.view();

  auto const srFlagEBSize = srFlagEBSoAView.size();
  auto const srFlagEESize = srFlagEESoAView.size();

  srFlagsEB->reserve(srFlagEBSize);
  srFlagsEE->reserve(srFlagEESize);

  for (size_t i = 0; i < srFlagEBSize; ++i) {
    EcalTrigTowerDetId const tt(srFlagEBSoAView.id()[i]);
    EBSrFlag const elem(tt, static_cast<int>(srFlagEBSoAView.flag()[i]));
    srFlagsEB->push_back(elem);
  }

  for (size_t i = 0; i < srFlagEESize; ++i) {
    EcalScDetId const scId(srFlagEESoAView.id()[i]);
    EESrFlag const elem(scId, static_cast<int>(srFlagEESoAView.flag()[i]));
    srFlagsEE->push_back(elem);
  }

  event.put(srFlagsOutEBToken_, std::move(srFlagsEB));
  event.put(srFlagsOutEEToken_, std::move(srFlagsEE));

  ////////////////////////////////////////////////////////////////////////////
  // channel integrity errors EB
  auto integrityGainErrorsEBOut = std::make_unique<EBDetIdCollection>();
  auto integrityGainSwitchErrorsEBOut = std::make_unique<EBDetIdCollection>();
  auto integrityChIdErrorsEBOut = std::make_unique<EBDetIdCollection>();

  auto const& integrityGainErrorsEBIn = event.get(ebIntegrityGainErrorsInToken_);
  auto const& integrityGainSwitchErrorsEBIn = event.get(ebIntegrityGainSwitchErrorsInToken_);
  auto const& integrityChIdErrorsEBIn = event.get(ebIntegrityChIdErrorsInToken_);
  auto& integrityGainErrorsEBInView = integrityGainErrorsEBIn.view();
  auto& integrityGainSwitchErrorsEBInView = integrityGainSwitchErrorsEBIn.view();
  auto& integrityChIdErrorsEBInView = integrityChIdErrorsEBIn.view();

  auto const integrityGainErrorsEBSize = integrityGainErrorsEBInView.size();
  auto const integrityGainSwitchErrorsEBSize = integrityGainSwitchErrorsEBInView.size();
  auto const integrityChIdErrorsEBSize = integrityChIdErrorsEBInView.size();

  integrityGainErrorsEBOut->reserve(integrityGainErrorsEBSize);
  integrityGainSwitchErrorsEBOut->reserve(integrityGainSwitchErrorsEBSize);
  integrityChIdErrorsEBOut->reserve(integrityChIdErrorsEBSize);

  for (size_t i = 0; i < integrityGainErrorsEBSize; ++i) {
    EBDetId const id(integrityGainErrorsEBInView.id()[i]);
    integrityGainErrorsEBOut->push_back(id);
  }
  for (size_t i = 0; i < integrityGainSwitchErrorsEBSize; ++i) {
    EBDetId const id(integrityGainSwitchErrorsEBInView.id()[i]);
    integrityGainSwitchErrorsEBOut->push_back(id);
  }
  for (size_t i = 0; i < integrityChIdErrorsEBSize; ++i) {
    EBDetId const id(integrityChIdErrorsEBInView.id()[i]);
    integrityChIdErrorsEBOut->push_back(id);
  }

  event.put(ebIntegrityGainErrorsOutToken_, std::move(integrityGainErrorsEBOut));
  event.put(ebIntegrityGainSwitchErrorsOutToken_, std::move(integrityGainSwitchErrorsEBOut));
  event.put(ebIntegrityChIdErrorsOutToken_, std::move(integrityChIdErrorsEBOut));

  ////////////////////////////////////////////////////////////////////////////
  // channel integrity errors EE
  auto integrityGainErrorsEEOut = std::make_unique<EEDetIdCollection>();
  auto integrityGainSwitchErrorsEEOut = std::make_unique<EEDetIdCollection>();
  auto integrityChIdErrorsEEOut = std::make_unique<EEDetIdCollection>();

  auto const& integrityGainErrorsEEIn = event.get(eeIntegrityGainErrorsInToken_);
  auto const& integrityGainSwitchErrorsEEIn = event.get(eeIntegrityGainSwitchErrorsInToken_);
  auto const& integrityChIdErrorsEEIn = event.get(eeIntegrityChIdErrorsInToken_);
  auto& integrityGainErrorsEEInView = integrityGainErrorsEEIn.view();
  auto& integrityGainSwitchErrorsEEInView = integrityGainSwitchErrorsEEIn.view();
  auto& integrityChIdErrorsEEInView = integrityChIdErrorsEEIn.view();

  auto const integrityGainErrorsEESize = integrityGainErrorsEEInView.size();
  auto const integrityGainSwitchErrorsEESize = integrityGainSwitchErrorsEEInView.size();
  auto const integrityChIdErrorsEESize = integrityChIdErrorsEEInView.size();

  integrityGainErrorsEEOut->reserve(integrityGainErrorsEESize);
  integrityGainSwitchErrorsEEOut->reserve(integrityGainSwitchErrorsEESize);
  integrityChIdErrorsEEOut->reserve(integrityChIdErrorsEESize);

  for (size_t i = 0; i < integrityGainErrorsEESize; ++i) {
    EEDetId const id(integrityGainErrorsEEInView.id()[i]);
    integrityGainErrorsEEOut->push_back(id);
  }
  for (size_t i = 0; i < integrityGainSwitchErrorsEESize; ++i) {
    EEDetId const id(integrityGainSwitchErrorsEEInView.id()[i]);
    integrityGainSwitchErrorsEEOut->push_back(id);
  }
  for (size_t i = 0; i < integrityChIdErrorsEESize; ++i) {
    EEDetId const id(integrityChIdErrorsEEInView.id()[i]);
    integrityChIdErrorsEEOut->push_back(id);
  }

  event.put(eeIntegrityGainErrorsOutToken_, std::move(integrityGainErrorsEEOut));
  event.put(eeIntegrityGainSwitchErrorsOutToken_, std::move(integrityGainSwitchErrorsEEOut));
  event.put(eeIntegrityChIdErrorsOutToken_, std::move(integrityChIdErrorsEEOut));

  ////////////////////////////////////////////////////////////////////////////
  // PN diode digis
  auto pnDiodeDigis = std::make_unique<EcalPnDiodeDigiCollection>();

  auto const& pnDiodeDigisHostColl = event.get(pnDiodeDigisInToken_);
  auto& pnDiodeDigisCollView = pnDiodeDigisHostColl.view();

  auto const pnDiodeDigisCollSize = pnDiodeDigisCollView.size();

  pnDiodeDigis->reserve(pnDiodeDigisCollSize);

  for (size_t i = 0; i < pnDiodeDigisCollSize; ++i) {
    EcalPnDiodeDetId const id(pnDiodeDigisCollView.id()[i]);
    EcalPnDiodeDigi elem(id);
    elem.setSize(pnDiodeDigisCollView.data()[i].size());
    for (size_t j = 0; j < pnDiodeDigisCollView.data()[i].size(); ++j) {
      EcalFEMSample const femSample(pnDiodeDigisCollView.data()[i][j]);
      elem.setSample(j, femSample);
    }
    pnDiodeDigis->push_back(elem);
  }

  event.put(pnDiodeDigisOutToken_, std::move(pnDiodeDigis));

  ////////////////////////////////////////////////////////////////////////////
  // Pseudo strip input digis
  auto psInputDigis = std::make_unique<EcalPSInputDigiCollection>();

  auto const& pseudoStripInputs = event.get(ecalPseudoStripInputsInToken_);
  auto& pseudoStripInputsView = pseudoStripInputs.view();

  auto const pseudoStripInputsSize = pseudoStripInputsView.size();

  psInputDigis->reserve(pseudoStripInputsSize);

  for (size_t i = 0; i < pseudoStripInputsSize; ++i) {
    EcalTriggerElectronicsId const id(pseudoStripInputsView.id()[i]);
    EcalPseudoStripInputDigi elem(id);
    elem.setSize(pseudoStripInputsView.data()[i].size());
    for (size_t j = 0; j < pseudoStripInputsView.data()[i].size(); ++j) {
      elem.setSampleValue(j, pseudoStripInputsView.data()[i][j]);
    }
    psInputDigis->push_back(elem);
  }

  event.put(ecalPseudoStripInputsOutToken_, std::move(psInputDigis));

  ////////////////////////////////////////////////////////////////////////////
  // Trigger primitive digis
  auto triggerPrimitives = std::make_unique<EcalTrigPrimDigiCollection>();

  auto const& trigPrimColl = event.get(ecalTriggerPrimitivesInToken_);
  auto& trigPrimCollView = trigPrimColl.view();

  auto const trigPrimCollSize = trigPrimCollView.size();

  triggerPrimitives->reserve(trigPrimCollSize);

  for (size_t i = 0; i < trigPrimCollSize; ++i) {
    EcalTrigTowerDetId const id(trigPrimCollView.id()[i]);
    EcalTriggerPrimitiveDigi elem(id);
    elem.setSize(trigPrimCollView.data()[i].size());
    for (size_t j = 0; j < trigPrimCollView.data()[i].size(); ++j) {
      elem.setSampleValue(j, trigPrimCollView.data()[i][j]);
    }
    triggerPrimitives->push_back(elem);
  }

  event.put(ecalTriggerPrimitivesOutToken_, std::move(triggerPrimitives));

  ////////////////////////////////////////////////////////////////////////////
  // integrity errors
  auto integrityTTIdErrorsOut = std::make_unique<EcalElectronicsIdCollection>();
  auto integrityZSXtalIdErrorsOut = std::make_unique<EcalElectronicsIdCollection>();
  auto integrityBlockSizeErrorsOut = std::make_unique<EcalElectronicsIdCollection>();

  auto const& integrityTTIdErrorsIn = event.get(integrityTTIdErrorsInToken_);
  auto const& integrityZSXtalIdErrorsIn = event.get(integrityZSXtalIdErrorsInToken_);
  auto const& integrityBlockSizeErrorsIn = event.get(integrityBlockSizeErrorsInToken_);
  auto& integrityTTIdErrorsInView = integrityTTIdErrorsIn.view();
  auto& integrityZSXtalIdErrorsInView = integrityZSXtalIdErrorsIn.view();
  auto& integrityBlockSizeErrorsInView = integrityBlockSizeErrorsIn.view();

  auto const integrityTTIdErrorsSize = integrityTTIdErrorsInView.size();
  auto const integrityZSXtalIdErrorsSize = integrityZSXtalIdErrorsInView.size();
  auto const integrityBlockSizeErrorsSize = integrityBlockSizeErrorsInView.size();

  integrityTTIdErrorsOut->reserve(integrityTTIdErrorsSize);
  integrityZSXtalIdErrorsOut->reserve(integrityZSXtalIdErrorsSize);
  integrityBlockSizeErrorsOut->reserve(integrityBlockSizeErrorsSize);

  for (size_t i = 0; i < integrityTTIdErrorsSize; ++i) {
    EcalElectronicsId const id(integrityTTIdErrorsInView.id()[i]);
    integrityTTIdErrorsOut->push_back(id);
  }
  for (size_t i = 0; i < integrityZSXtalIdErrorsSize; ++i) {
    EcalElectronicsId const id(integrityZSXtalIdErrorsInView.id()[i]);
    integrityZSXtalIdErrorsOut->push_back(id);
  }
  for (size_t i = 0; i < integrityBlockSizeErrorsSize; ++i) {
    EcalElectronicsId const id(integrityBlockSizeErrorsInView.id()[i]);
    integrityBlockSizeErrorsOut->push_back(id);
  }

  event.put(integrityTTIdErrorsOutToken_, std::move(integrityTTIdErrorsOut));
  event.put(integrityZSXtalIdErrorsOutToken_, std::move(integrityZSXtalIdErrorsOut));
  event.put(integrityBlockSizeErrorsOutToken_, std::move(integrityBlockSizeErrorsOut));

  ////////////////////////////////////////////////////////////////////////////
  // mem integrity errors
  auto integrityMemTtIdErrorsOut = std::make_unique<EcalElectronicsIdCollection>();
  auto integrityMemBlockSizeErrorsOut = std::make_unique<EcalElectronicsIdCollection>();
  auto integrityMemChIdErrorsOut = std::make_unique<EcalElectronicsIdCollection>();
  auto integrityMemGainErrorsOut = std::make_unique<EcalElectronicsIdCollection>();

  auto const& integrityMemTtIdErrorsIn = event.get(integrityMemTtIdErrorsInToken_);
  auto const& integrityMemBlockSizeErrorsIn = event.get(integrityMemBlockSizeErrorsInToken_);
  auto const& integrityMemChIdErrorsIn = event.get(integrityMemChIdErrorsInToken_);
  auto const& integrityMemGainErrorsIn = event.get(integrityMemGainErrorsInToken_);
  auto& integrityMemTtIdErrorsInView = integrityMemTtIdErrorsIn.view();
  auto& integrityMemBlockSizeErrorsInView = integrityMemBlockSizeErrorsIn.view();
  auto& integrityMemChIdErrorsInView = integrityMemChIdErrorsIn.view();
  auto& integrityMemGainErrorsInView = integrityMemGainErrorsIn.view();

  auto const integrityMemTtIdErrorsSize = integrityMemTtIdErrorsInView.size();
  auto const integrityMemBlockSizeErrorsSize = integrityMemBlockSizeErrorsInView.size();
  auto const integrityMemChIdErrorsSize = integrityMemChIdErrorsInView.size();
  auto const integrityMemGainErrorsSize = integrityMemGainErrorsInView.size();

  integrityMemTtIdErrorsOut->reserve(integrityMemTtIdErrorsSize);
  integrityMemBlockSizeErrorsOut->reserve(integrityMemBlockSizeErrorsSize);
  integrityMemChIdErrorsOut->reserve(integrityMemChIdErrorsSize);
  integrityMemGainErrorsOut->reserve(integrityMemGainErrorsSize);

  for (size_t i = 0; i < integrityMemTtIdErrorsSize; ++i) {
    EcalElectronicsId const id(integrityMemTtIdErrorsInView.id()[i]);
    integrityMemTtIdErrorsOut->push_back(id);
  }
  for (size_t i = 0; i < integrityMemBlockSizeErrorsSize; ++i) {
    EcalElectronicsId const id(integrityMemBlockSizeErrorsInView.id()[i]);
    integrityMemBlockSizeErrorsOut->push_back(id);
  }
  for (size_t i = 0; i < integrityMemChIdErrorsSize; ++i) {
    EcalElectronicsId const id(integrityMemChIdErrorsInView.id()[i]);
    integrityMemChIdErrorsOut->push_back(id);
  }
  for (size_t i = 0; i < integrityMemGainErrorsSize; ++i) {
    EcalElectronicsId const id(integrityMemGainErrorsInView.id()[i]);
    integrityMemGainErrorsOut->push_back(id);
  }

  event.put(integrityMemTtIdErrorsOutToken_, std::move(integrityMemTtIdErrorsOut));
  event.put(integrityMemBlockSizeErrorsOutToken_, std::move(integrityMemBlockSizeErrorsOut));
  event.put(integrityMemChIdErrorsOutToken_, std::move(integrityMemChIdErrorsOut));
  event.put(integrityMemGainErrorsOutToken_, std::move(integrityMemGainErrorsOut));

  ////////////////////////////////////////////////////////////////////////////
  // DCC header block
  auto dccHeaderBlocksOut = std::make_unique<EcalRawDataCollection>();

  auto const& dccHeaderBlocksIn = event.get(ecalRawDataInToken_);
  auto& dccHeaderBlocksInView = dccHeaderBlocksIn.view();

  auto const dccHeaderBlocksSize = dccHeaderBlocksInView.size();

  dccHeaderBlocksOut->reserve(dccHeaderBlocksSize);

  for (size_t i = 0; i < dccHeaderBlocksSize; ++i) {
    EcalDCCHeaderBlock elem(dccHeaderBlocksInView.dccId()[i]);
    elem.setFedId(dccHeaderBlocksInView.fedId()[i]);
    elem.setErrors(dccHeaderBlocksInView.dccErrors()[i]);
    elem.setDccInTTCCommand(dccHeaderBlocksInView.dccInTTCCommand()[i]);
    elem.setOrbit(dccHeaderBlocksInView.orbitNumber()[i]);
    elem.setRunType(dccHeaderBlocksInView.runType()[i]);
    elem.setZs(dccHeaderBlocksInView.zs()[i]);
    elem.setBasicTriggerType(dccHeaderBlocksInView.basicTriggerType()[i]);
    elem.setLV1(dccHeaderBlocksInView.LV1event()[i]);
    elem.setRunNumber(dccHeaderBlocksInView.runNumber()[i]);
    elem.setBX(dccHeaderBlocksInView.BX()[i]);
    elem.setEventSettings(dccHeaderBlocksInView.eventSettings()[i]);
    elem.setRtHalf(dccHeaderBlocksInView.rtHalf()[i]);
    elem.setMgpaGain(dccHeaderBlocksInView.mgpaGain()[i]);
    elem.setMemGain(dccHeaderBlocksInView.memGain()[i]);
    elem.setSelectiveReadout(dccHeaderBlocksInView.selectiveReadout()[i]);
    elem.setTestZeroSuppression(dccHeaderBlocksInView.testZeroSuppression()[i]);
    elem.setZeroSuppression(dccHeaderBlocksInView.zeroSuppression()[i]);

    auto const tccStatusBegin = dccHeaderBlocksInView.tccStatus()[i].begin();
    std::vector<short> const tccStatusVec(tccStatusBegin, tccStatusBegin + dccHeaderBlocksInView.tccStatusSize()[i]);
    elem.setTccStatus(tccStatusVec);

    auto const ttFlagBegin = dccHeaderBlocksInView.triggerTowerFlag()[i].begin();
    std::vector<short> const ttFlagVec(ttFlagBegin, ttFlagBegin + dccHeaderBlocksInView.triggerTowerFlagSize()[i]);
    elem.setTriggerTowerFlags(ttFlagVec);

    auto const feStatusBegin = dccHeaderBlocksInView.feStatus()[i].begin();
    std::vector<short> const feStatusVec(feStatusBegin, feStatusBegin + dccHeaderBlocksInView.feStatusSize()[i]);
    elem.setFEStatus(feStatusVec);

    auto const feBxBegin = dccHeaderBlocksInView.feBx()[i].begin();
    std::vector<short> const feBxVec(feBxBegin, feBxBegin + dccHeaderBlocksInView.feBxSize()[i]);
    elem.setFEBx(feBxVec);

    auto const tccBxBegin = dccHeaderBlocksInView.tccBx()[i].begin();
    std::vector<short> const tccBxVec(tccBxBegin, tccBxBegin + dccHeaderBlocksInView.tccBxSize()[i]);
    elem.setTCCBx(tccBxVec);

    auto const feLv1Begin = dccHeaderBlocksInView.feLv1()[i].begin();
    std::vector<short> const feLv1Vec(feLv1Begin, feLv1Begin + dccHeaderBlocksInView.feLv1Size()[i]);
    elem.setFELv1(feLv1Vec);

    auto const tccLv1Begin = dccHeaderBlocksInView.tccLv1()[i].begin();
    std::vector<short> const tccLv1Vec(tccLv1Begin, tccLv1Begin + dccHeaderBlocksInView.tccLv1Size()[i]);
    elem.setTCCLv1(tccLv1Vec);

    elem.setSrpStatus(dccHeaderBlocksInView.srpStatus()[i]);
    elem.setSRPBx(dccHeaderBlocksInView.srpBx()[i]);
    elem.setSRPLv1(dccHeaderBlocksInView.srpLv1()[i]);

    dccHeaderBlocksOut->push_back(elem);
  }

  event.put(ecalRawDataOutToken_, std::move(dccHeaderBlocksOut));
}

DEFINE_FWK_MODULE(EcalDigisFromPortableProducer);
