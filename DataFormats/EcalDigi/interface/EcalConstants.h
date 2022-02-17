//Namespaces for Phase1 and Phase2
#ifndef DataFormats_EcalDigi_EcalConstants_h
#define DataFormats_EcalDigi_EcalConstants_h

class ecalPh2 {
public:
  static constexpr double Samp_Period = 6.25;               // ADC sampling period in ns
  static constexpr unsigned int NGAINS = 2;                 // Number of CATIA gains
  static constexpr float gains[NGAINS] = {10., 1.};         // CATIA gain values
  static constexpr unsigned int gainId1 = 1;                // Position of gain 1 in gains array
  static constexpr unsigned int gainId10 = 0;               // Position of gain 10 in gains array
  static constexpr unsigned int kMaxGainId = gainId10;      // Position of the maximum gain in the gains array
  static constexpr unsigned int sampleSize = 16;            // Number of samples per event
  static constexpr unsigned int kMaxSampleIdx = 5;          // Index of the sample at the maximum of the pulse
  static constexpr unsigned int NBITS = 12;                 // Number of available bits
  static constexpr unsigned int MAXADC = (1 << NBITS) - 1;  // 2^NBITS - 1,  ADC max range
  static constexpr unsigned int kEBChannels = 61200;        // Number of channels in the barrel
  static constexpr double maxEneEB = 2000.;                 // Max attainable energy in the barrel in GeV
                                                            // ~(MAXADC * 10(gain) * 0.05 GeV(LSB at gain 10))
  static constexpr unsigned int kNOffsets = 2000;           // Number of time offsets generated for APD pulse shape
                                                            // simulation and reused for every kNOffsets^th channel
  static constexpr unsigned int kAdcMask = 0xFFF;           // ADC sample mask for unpacking
  static constexpr unsigned int kGainIdMask = 0x1;          // Gain id mask for unpacking
  static constexpr unsigned int kNActiveLHCPeriods = 5;     // Number of active LHC periods for multifit algorithm
  static constexpr unsigned int kNSamplesPerLHCPeriod = 25 / Samp_Period;    // Number of samples per LHC 25ns period
  static constexpr unsigned int kPulseShapeTemplateSampleSize = sampleSize;  // Pulse shape template sample size for
                                                                             // multifit Eigen matrix types
  static constexpr unsigned int kFullSampleVectorSize =
      (kNActiveLHCPeriods - 1) * kNSamplesPerLHCPeriod + sampleSize;  // Size of full sample vector for multifit
                                                                      // Eigen matrix types
};

class ecalPh1 {
public:
  static constexpr double Samp_Period = 25.;                 // ADC sampling period in ns
  static constexpr unsigned int NGAINS = 4;                  // Number of MGPA gains including a zero gain that
                                                             // could be encoded in the gain id mask
  static constexpr float gains[NGAINS] = {0., 12., 6., 1.};  // MGPA gain values including a zero gain
  static constexpr unsigned int kMaxGainId = 1;              // Position of the maximum gain in the gains array
  static constexpr unsigned int sampleSize = 10;             // Number of samples per event
  static constexpr unsigned int kMaxSampleIdx = 5;           // Index of the sample at the maximum of the pulse
  static constexpr unsigned int NBITS = 12;                  // Number of available bits
  static constexpr unsigned int kNOffsets = 2000;            // Number of time offsets generated for APD pulse shape
                                                             // simulation and reused for every kNOffsets^th channel
  static constexpr unsigned int kAdcMask = 0xFFF;            // ADC sample mask for unpacking
  static constexpr unsigned int kGainIdMask = 0x3;           // Gain id mask for unpacking
  static constexpr unsigned int kNActiveLHCPeriods = 10;     // Number of active LHC periods for multifit algorithm
  static constexpr unsigned int kNSamplesPerLHCPeriod = 25 / Samp_Period;  // Number of samples per LHC 25ns period
  static constexpr unsigned int kPulseShapeTemplateSampleSize = 12;        // Pulse shape template sample size for
                                                                           // multifit Eigen matrix types
  static constexpr unsigned int kFullSampleVectorSize =
      (kNActiveLHCPeriods - 1) * kNSamplesPerLHCPeriod + sampleSize;  // Size of full sample vector for multifit
                                                                      // Eigen matrix types
};
#endif
