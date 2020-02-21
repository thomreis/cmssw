#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_EcalBcpPayloadParamsHelper_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_EcalBcpPayloadParamsHelper_h

#include <iostream>
#include <string>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/EcalObjects/interface/EcalBcpPayloadParams.h"

class EcalBcpPayloadParamsHelper : public EcalBcpPayloadParams {
 public:
  EcalBcpPayloadParamsHelper();
  EcalBcpPayloadParamsHelper(const EcalBcpPayloadParams &params);
  EcalBcpPayloadParamsHelper(const edm::ParameterSet& config);
  ~EcalBcpPayloadParamsHelper() {};

  void createFromPSet(const edm::ParameterSet& config);

  // Global parameters
  unsigned int sampleOfInterest(const EBDetId &detId) const;
  void setSampleOfInterest(const EBDetId &detId, const unsigned int soi);

  // Spike tagger LD parameters
  std::string spikeTaggerLdType() const;
  void setSpikeTaggerLdType(const std::string &type);

  double spikeTaggerLdThreshold(const EBDetId &detId) const;
  void setSpikeTaggerLdThreshold(const EBDetId &detId, const double &thr);

  std::vector<double> spikeTaggerLdWeights(const EBDetId &detId) const;
  void setSpikeTaggerLdWeights(const EBDetId &detId, const std::vector<double> &weights);

  // print parameters to stream:
  void print(std::ostream &out) const;
  friend std::ostream & operator<<(std::ostream &out, const EcalBcpPayloadParamsHelper &params);

 private:
  // Defines the content of each node
  // New nodes can only be added before NUM_NODES
  enum EcalBcpPayloadParamNode {
    kGlobalAlgoParams = 0, // assuming there will be some global parameters valid for all crystals
    kGlobalSpikeTaggerLdParams,
    NUM_NODES
  };

  // Defines the content of each crystal node
  // New nodes can only be added before NUM_CRYSTAL_NODES
  enum EcalBcpPayloadParamCrystalNode {
    kCrystalAlgoParams = 0,
    kCrystalSpikeTaggerLdParams,
    kCrystalSpikeTaggerLdWeights,
    NUM_CRYSTAL_NODES
  };

  // index of variabe inside a node vector
  enum DIdx {kSpikeThreshold = 0};
  enum UIdx {kSampleOfInterest = 0};
  enum IIdx {};
  enum SIdx {kSpikeTaggerLdType = 0};

  void setSamplesOfInterest(const std::vector<edm::ParameterSet> &pSets);
  void setPerCrystalSpikeTaggerParams(const std::vector<edm::ParameterSet> &pSets);
  void parseCrystalRange(const std::string &rangeStr, int &iMin, int &iMax, const bool isEta = true);
};

#endif
