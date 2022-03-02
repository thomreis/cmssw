#ifndef RecoLocalCalo_EcalRecProducers_plugins_EigenMatrixTypes_gpu_h
#define RecoLocalCalo_EcalRecProducers_plugins_EigenMatrixTypes_gpu_h

#include <array>

#include <Eigen/Dense>

#include "DataFormats/EcalDigi/interface/EcalConstants.h"
#include "CUDADataFormats/EcalRecHitSoA/interface/RecoTypes.h"

namespace ecal {
  namespace multifit {

    constexpr int SampleVectorSize = static_cast<int>(ecalPh1::sampleSize);
    constexpr int FullSampleVectorSize = static_cast<int>(ecalPh1::kFullSampleVectorSize);
    constexpr int PulseVectorSize = static_cast<int>(ecalPh1::kPulseShapeTemplateSampleSize);
    constexpr int NGains = static_cast<int>(ecalPh1::NGAINS) - 1;  // do not count 0 gain here

    using data_type = ::ecal::reco::ComputationScalarType;

    typedef Eigen::Matrix<data_type, SampleVectorSize, SampleVectorSize> PulseMatrixType;
    typedef Eigen::Matrix<char, SampleVectorSize, 1> BXVectorType;
    using SampleMatrixD = Eigen::Matrix<double, SampleVectorSize, SampleVectorSize>;

    typedef Eigen::Matrix<data_type, SampleVectorSize, 1> SampleVector;
    typedef Eigen::Matrix<data_type, FullSampleVectorSize, 1> FullSampleVector;
    typedef Eigen::Matrix<data_type, Eigen::Dynamic, 1, 0, PulseVectorSize, 1> PulseVector;
    typedef Eigen::Matrix<char, Eigen::Dynamic, 1, 0, PulseVectorSize, 1> BXVector;
    typedef Eigen::Matrix<char, SampleVectorSize, 1> SampleGainVector;
    typedef Eigen::Matrix<data_type, SampleVectorSize, SampleVectorSize> SampleMatrix;
    typedef Eigen::Matrix<data_type, FullSampleVectorSize, FullSampleVectorSize> FullSampleMatrix;
    typedef Eigen::Matrix<data_type, Eigen::Dynamic, Eigen::Dynamic, 0, PulseVectorSize, PulseVectorSize> PulseMatrix;
    typedef Eigen::Matrix<data_type, SampleVectorSize, Eigen::Dynamic, 0, SampleVectorSize, PulseVectorSize>
        SamplePulseMatrix;
    typedef Eigen::LLT<SampleMatrix> SampleDecompLLT;
    typedef Eigen::LLT<SampleMatrixD> SampleDecompLLTD;
    typedef Eigen::LLT<PulseMatrix> PulseDecompLLT;
    typedef Eigen::LDLT<PulseMatrix> PulseDecompLDLT;

    typedef Eigen::Matrix<data_type, 1, 1> SingleMatrix;
    typedef Eigen::Matrix<data_type, 1, 1> SingleVector;

    typedef std::array<SampleMatrixD, NGains> SampleMatrixGainArray;

    using PermutationMatrix = Eigen::PermutationMatrix<SampleMatrix::RowsAtCompileTime>;

  }  // namespace multifit
}  // namespace ecal

#endif  // RecoLocalCalo_EcalRecProducers_plugins_EigenMatrixTypes_gpu_h
