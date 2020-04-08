#ifndef TkTrigger_TkEtMiss_h
#define TkTrigger_TkEtMiss_h
// Package:     L1Trigger
// Class  :     TkEtMiss
// Original Author:  E. Perez
//         Created:  Nov 14, 2013

// system include files
// user include files
#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Phase2L1Correlator/interface/TkPrimaryVertex.h"

namespace l1t {
  class TkEtMiss : public L1Candidate {
  public:
    enum EtMissType { kMET, kMHT, kNumTypes };
    TkEtMiss();
    TkEtMiss(const LorentzVector& p4,
                       EtMissType type,
                       const double& etTotal,
                       const double& etMissPU,
                       const double& etTotalPU,
                       const edm::Ref<TkPrimaryVertexCollection>& aVtxRef = edm::Ref<TkPrimaryVertexCollection>(),
                       int bx = 0);

    TkEtMiss(const LorentzVector& p4,
                       EtMissType type,
                       const double& etTotal,
                       const double& etMissPU,
                       const double& etTotalPU,
                       int bx = 0);

    // ---------- const member functions ---------------------
    EtMissType type() const { return type_; }  // kET or kHT
    // For type = kET, this is |MET|; for type = kHT, this is |MHT|
    double etMiss() const { return et(); }
    // For type = kET, this is total ET; for type = kHT, this is total HT
    const double& etTotal() const { return etTot_; }
    // EtMiss and EtTot from PU vertices
    double etMissPU() const { return etMissPU_; }
    double etTotalPU() const { return etTotalPU_; }
    int bx() const { return bx_; }
    const edm::Ref<TkPrimaryVertexCollection>& vtxRef() const { return vtxRef_; }

    // ---------- member functions ---------------------------
    void setEtTotal(const double& etTotal) { etTot_ = etTotal; }
    void setBx(int bx) { bx_ = bx; }

  private:
    // ---------- member data --------------------------------
    EtMissType type_;
    double etTot_;
    double etMissPU_;
    double etTotalPU_;
    edm::Ref<TkPrimaryVertexCollection> vtxRef_;
    int bx_;
  };
}  // namespace l1t

#endif
