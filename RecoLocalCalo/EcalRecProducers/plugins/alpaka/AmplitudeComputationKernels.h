#ifndef RecoLocalCalo_EcalRecProducers_plugins_alpaka_AmplitudeComputationKernels_h
#define RecoLocalCalo_EcalRecProducers_plugins_alpaka_AmplitudeComputationKernels_h

#include "CondFormats/EcalObjects/interface/alpaka/EcalMultifitConditionsDevice.h"
#include "DataFormats/EcalDigi/interface/alpaka/EcalDigiDeviceCollection.h"
#include "DataFormats/EcalRecHit/interface/alpaka/EcalUncalibratedRecHitDeviceCollection.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/traits.h"
#include "DeclsForKernels.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::multifit {

  using InputProduct = EcalDigiDeviceCollection;
  using OutputProduct = EcalUncalibratedRecHitDeviceCollection;

  void minimization_procedure(Queue&,
                              InputProduct const&,
                              InputProduct const&,
                              OutputProduct&,
                              OutputProduct&,
                              EventDataForScratchDevice&,
                              EcalMultifitConditionsDevice const&,
                              ConfigurationParameters const&,
                              uint32_t const);

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::ecal::multifit

#endif  // RecoLocalCalo_EcalRecProducers_plugins_AmplitudeComputationKernels_h
