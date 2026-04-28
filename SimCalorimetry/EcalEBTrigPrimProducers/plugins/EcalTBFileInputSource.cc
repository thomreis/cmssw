// -*- C++ -*-
//
// Package:    SimCalorimetry/EcalEBTrigPrimProducers
// Class:      EcalTBFileInputSource
//
/**\class EcalTBFileInputSource EcalTBFileInputSource.cc SimCalorimetry/EcalEBTrigPrimProducers/plugins/EcalTBFileInputSource.cc

 Description: Produces Ecal digis (legacy or Phase 2) from TB data text files

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis
//         Created:  Tue, 10 Mar 2020 15:45:37 GMT
//
//

// system include files
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Sources/interface/ProducerSourceBase.h"
#include "FWStorage/Catalog/interface/FromFiles.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalLiteDTUSample.h"
#include "DataFormats/EcalDigi/interface/EcalMGPASample.h"

//
// class declaration
//

template <typename T>
class EcalTBFileInputSource : public edm::ProducerSourceBase {
public:
  explicit EcalTBFileInputSource(const edm::ParameterSet&, edm::InputSourceDescription const&);
  ~EcalTBFileInputSource();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  bool setRunAndEventInfo(edm::EventID& id,
                          edm::TimeValue_t& time,
                          edm::EventAuxiliary::ExperimentType& eType) override;
  virtual void produce(edm::Event&) override;

  void openFile();
  void readHeader();
  void setSample(T& digis, unsigned int chidx, unsigned int sampleidx, int adc, int gain);

  // ----------member data ---------------------------
  edm::FromFiles fromFiles_;
  std::string fname_;
  std::ifstream fstream_;

  const unsigned long runnr_;
  unsigned long evtnr_;
  const unsigned int startSample_;
  const int skipUntilEventNr_;
  const double attenuation_;

  std::string line_;
  unsigned int nchannels_;

  std::vector<EBDetId> detIds_;
  T ebDigis_;
  const edm::EDPutTokenT<T> ebDigiToken_;
};

//
// constructors and destructor
//
template <typename T>
EcalTBFileInputSource<T>::EcalTBFileInputSource(const edm::ParameterSet& iConfig,
                                                const edm::InputSourceDescription& iSrcDesc)
    : edm::ProducerSourceBase(iConfig, iSrcDesc, true),
      fromFiles_(iConfig),
      runnr_(iConfig.getUntrackedParameter<unsigned int>("runNumber")),
      evtnr_(iConfig.getUntrackedParameter<unsigned int>("firstEventNumber")),
      startSample_(iConfig.getUntrackedParameter<unsigned int>("startSample")),
      skipUntilEventNr_(evtnr_ + iConfig.getUntrackedParameter<int>("skipEvents", 0)),
      attenuation_(iConfig.getUntrackedParameter<double>("attenuation", 1.)),
      nchannels_(0),
      ebDigiToken_(produces<T>()) {
  auto fileNames = fromFiles_.fileNames(0);
  if (fileNames.empty()) {
    throw cms::Exception("FileOpenError") << "No input file";
  }
  fname_ = fileNames[0].substr(fileNames[0].find(":") + 1);
  openFile();
  readHeader();
}

template <typename T>
EcalTBFileInputSource<T>::~EcalTBFileInputSource() {
  if (fstream_.is_open()) {
    fstream_.close();
  }
}

//
// member functions
//
template <typename T>
void EcalTBFileInputSource<T>::openFile() {
  if (!fstream_.is_open()) {
    fstream_.open(fname_);
    if (!fstream_.good()) {
      throw cms::Exception("FileOpenError") << "Failed to open input file " << fname_;
    }
  }
}

template <typename T>
void EcalTBFileInputSource<T>::readHeader() {
  nchannels_ = 0;
  if (fstream_.good()) {
    while (getline(fstream_, line_)) {
      std::istringstream strstream(line_);

      std::string labelstr;

      if (line_.find("Channels") != std::string::npos) {
        strstream >> labelstr >> nchannels_;
        edm::LogVerbatim("ECAL TB input source")
            << "Found " << nchannels_ << " channel(s) in the input file '" << fname_ << "'.";
      } else if (line_.find("Chan:") != std::string::npos) {
        std::string chan;
        strstream >> labelstr;
        // interpret letters as eta and numbers a phi
        for (unsigned int i = 0; i < nchannels_; ++i) {
          strstream >> chan;
          int ieta = chan[0] - 'A' + 1;
          int iphi = chan[1] - '0';
          edm::LogVerbatim("ECAL TB input source") << "DetId: " << chan << ", ieta: " << ieta << ", iphi: " << iphi;
          EBDetId detId(ieta, iphi);
          detIds_.emplace_back(detId);
        }
        continue;
      } else if (line_.find("Frame") != std::string::npos) {
        return;
      }
    }
  }
}

template <typename T>
void EcalTBFileInputSource<T>::setSample(T& digis, unsigned int chidx, unsigned int sampleidx, int adc, int gain) {
  const EcalMGPASample sample(adc / attenuation_,
                              gain);  // EcalMGPASample applies a mask of 0xFFF on adc count (max. adc: 4095)
  static_cast<EcalDataFrame>(digis[chidx]).setSample(sampleidx - startSample_, sample);
}

template <>
void EcalTBFileInputSource<EBDigiCollectionPh2>::setSample(
    EBDigiCollectionPh2& digis, unsigned int chidx, unsigned int sampleidx, int adc, int gain) {
  const EcalLiteDTUSample sample(adc / attenuation_,
                                 gain);  // EcalLiteDTUSample applies a mask ecalPh2::kAdcMask on adc count
  static_cast<EcalDataFrame_Ph2>(digis[chidx]).setSample(sampleidx - startSample_, sample);
}

template <typename T>
bool EcalTBFileInputSource<T>::setRunAndEventInfo(edm::EventID& id,
                                                  edm::TimeValue_t& time,
                                                  edm::EventAuxiliary::ExperimentType& eType) {
  // stop if there are no more data in the file
  if (fstream_.eof()) {
    return false;
  }

  // Assume the same gain for all samples as a start
  constexpr int gain = 1;

  do {
    id = edm::EventID(runnr_, id.luminosityBlock(), evtnr_);

    T ebDigis;
    ebDigis.reserve(nchannels_);
    for (unsigned int i = 0; i < nchannels_; ++i) {
      ebDigis.push_back(detIds_[i].rawId());
    }
    unsigned int s = 0;
    while (not fstream_.eof()) {
      std::istringstream strstream(line_);
      std::stringstream logstr("sample ");
      logstr << std::setw(2) << s << ", line: " << line_;

      // Only take as many frames as the ECAL digis format can handle
      if (s >= startSample_ and s < startSample_ + T::MAXSAMPLES) {
        logstr << ", --> take";
        std::string labelstr;
        std::string framenumberstr;
        std::string colonstr;
        int adc;
        bool first_sample;
        strstream >> labelstr >> framenumberstr >> colonstr;
        for (unsigned int i = 0; i < nchannels_; ++i) {
          strstream >> adc >> first_sample;
          setSample(ebDigis, i, s, adc, gain);
        }
      }
      edm::LogVerbatim("ECAL TB input source") << logstr.str();

      ++s;
      // get the next line
      getline(fstream_, line_);
      // if first_sample is 1 this is already the line of the next event
      if (line_.rfind("1") == line_.size() - 1) {
        break;
      }
    }

    ebDigis_.swap(ebDigis);
    ++evtnr_;
  } while (static_cast<long>(evtnr_) <= skipUntilEventNr_);

  return true;
}

// ------------ method called to produce the data  ------------
template <typename T>
void EcalTBFileInputSource<T>::produce(edm::Event& iEvent) {
  //for (unsigned int i = 0; i < ebDigis_.size(); ++i) {
  //  EBDataFrame ebDigi = ebDigis_[i];
  //  for (int j = 0; j < ebDigi.size(); ++j) {
  //    edm::LogVerbatim("ECAL TB input source") << "channel " << i << ", sample " << j << ", adc: " << ebDigi[j].adc();
  //  }
  //}

  // set digi object and put it in the event
  auto ebDigiOut = std::make_unique<T>(ebDigis_);
  iEvent.put(ebDigiToken_, std::move(ebDigiOut));
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
template <typename T>
void EcalTBFileInputSource<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  edm::ProducerSourceBase::fillDescription(desc);
  edm::FromFiles::fillDescription(desc);
  desc.addUntracked<unsigned int>("runNumber", 1);
  desc.addUntracked<unsigned int>("firstEventNumber", 1);
  desc.addUntracked<unsigned int>("startSample", 0);
  desc.addUntracked<int>("skipEvents", 0);
  desc.addUntracked<double>("attenuation", 1.);
  descriptions.addWithDefaultLabel(desc);
}

//define plug-ins for legacy and Phase 2 digis
typedef EcalTBFileInputSource<EBDigiCollection> EcalTBFileInputToDigiSource;
typedef EcalTBFileInputSource<EBDigiCollectionPh2> EcalTBFileInputToDigiPh2Source;
DEFINE_FWK_INPUT_SOURCE(EcalTBFileInputToDigiSource);
DEFINE_FWK_INPUT_SOURCE(EcalTBFileInputToDigiPh2Source);
