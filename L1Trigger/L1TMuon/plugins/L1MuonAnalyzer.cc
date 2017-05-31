
#include <vector>
#include <memory>
// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

//#include "L1TriggerDPG/L1Ntuples/interface/L1AnalysisUGMT.h"
//#include "L1TriggerDPG/L1Ntuples/interface/L1AnalysisUGMTDataFormat.h"

#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
// output
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "L1Trigger/L1TMuon/interface/MuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/RegionalMuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"

class MuonHistos {
public:
  MuonHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~MuonHistos() {};

  int selBx_;

  TH1I *hMuSize_;
  TH2I *h2MuSize_;
  TH1I *hMuPerBx_;
  TH1I *hMuPerBxId_;

  void FillBx(const int bx, const int bxId);
  void FillSize(const int size, const int bx);

protected:
  float ptScale_;
  float etaScale_;
  float phiScale_;
};

MuonHistos::MuonHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : selBx_(0), ptScale_(0.5), etaScale_(0.010875), phiScale_(0.010908)
{
  const std::string selBxStr = std::to_string(selBx_);
  hMuSize_ = fs->make<TH1I>(("hMuSize"+label).c_str(), (titlePrefix+" muons in BX"+selBxStr+";Number of muons").c_str(), 10, 0, 10);
  h2MuSize_ = fs->make<TH2I>(("h2MuSize"+label).c_str(), (titlePrefix+" muons per BX;Number of muons;BX").c_str(), 9, 1, 10, 15, -7, 8);
  hMuPerBx_ = fs->make<TH1I>(("hMuPerBx"+label).c_str(), (titlePrefix+" muons per BX;BX").c_str(), 15, -7, 8);
  hMuPerBxId_ = fs->make<TH1I>(("hMuPerBxId"+label).c_str(), (titlePrefix+" muons per BX ID;BX ID").c_str(), 3700, 0, 3700);
}

void
MuonHistos::FillBx(const int bx, const int bxId)
{
  hMuPerBx_->Fill(bx);
  hMuPerBxId_->Fill(bxId+bx);
}

void
MuonHistos::FillSize(const int size, const int bx)
{
  if (bx == selBx_) {
    hMuSize_->Fill(size);
  }
  h2MuSize_->Fill(size, bx);
}


class TfHistos : public MuonHistos {
public:
  TfHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~TfHistos() {};

  TH2I *h2HwPtVsProc_;
  TH2I *h2HwEtaVsProc_;
  TH2I *h2HwPhiVsProc_;
  TH2I *h2HwPtVsProcNeg_;
  TH2I *h2HwEtaVsProcNeg_;
  TH2I *h2HwPhiVsProcNeg_;
  TH2I *h2HwPtVsProcPos_;
  TH2I *h2HwEtaVsProcPos_;
  TH2I *h2HwPhiVsProcPos_;

  TH2I *h2ProcId_;
  TH2I *h2ProcIdNeg_;
  TH2I *h2ProcIdPos_;
  TH2I *h2LinkId_;
  TH2I *h2HwPt_;
  TH2I *h2HwEta_;
  TH2I *h2HwPhi_;
  TH2I *h2HwGlobalPhi_;
  TH2I *h2HwGlobalPhiNeg_;
  TH2I *h2HwGlobalPhiPos_;
  TH2I *h2HwSign_;
  TH2I *h2HwSignValid_;
  TH2I *h2HwQual_;
  TH2I *h2TfType_;
  TH2F *h2Pt_;
  TH2F *h2Eta_;
  TH2F *h2GlobalPhi_;
  TH2F *h2GlobalPhiNeg_;
  TH2F *h2GlobalPhiPos_;

  TH1I *hHwPt_;
  TH1I *hHwEta_;
  TH1I *hHwPhi_;
  TH1I *hHwGlobalPhi_;
  TH1I *hHwGlobalPhiNeg_;
  TH1I *hHwGlobalPhiPos_;
  TH1I *hHwSign_;
  TH1I *hHwSignValid_;
  TH1I *hHwQual_;
  TH1I *hTfType_;
  TH1I *hMuPerBxNeg_;
  TH1I *hMuPerBxPos_;
  TH1F *hPt_;
  TH1F *hEta_;
  TH1F *hGlobalPhi_;
  TH1F *hGlobalPhiNeg_;
  TH1F *hGlobalPhiPos_;

  TH2F *h2EtaPhi_;

  void FillMuon(const l1t::RegionalMuonCand& muon, const int bx);
};

TfHistos::TfHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : MuonHistos(fs, label, titlePrefix)
{
  h2HwPtVsProc_ = fs->make<TH2I>(("h2HwPtVsProc"+label).c_str(), (titlePrefix+" hwPt per Processor ID;hwPt;Processor ID (EMTF-/OMTF-: ID + 6)").c_str(), 150, 0, 300, 12, 0, 12);
  h2HwEtaVsProc_ = fs->make<TH2I>(("h2HwEtaVsProc"+label).c_str(), (titlePrefix+" hwEta per Processor ID;hwEta;Processor ID (EMTF-/OMTF-: ID + 6)").c_str(), 231, -230, 232, 12, 0, 12);
  h2HwPhiVsProc_ = fs->make<TH2I>(("h2HwPhiVsProc"+label).c_str(), (titlePrefix+" hwPhi per Processor ID;hwPhi;Processor ID (EMTF-/OMTF-: ID + 6)").c_str(), 139, -35, 104, 12, 0, 12);
  h2HwPtVsProcNeg_ = fs->make<TH2I>(("h2HwPtVsProcNeg"+label).c_str(), (titlePrefix+" hwPt per Processor ID negative side;hwPt;Processor ID").c_str(), 150, 0, 300, 6, 0, 6);
  h2HwEtaVsProcNeg_ = fs->make<TH2I>(("h2HwEtaVsProcNeg"+label).c_str(), (titlePrefix+" hwEta per Processor ID negative side;hwEta;Processor ID").c_str(), 231, -230, 232, 6, 0, 6);
  h2HwPhiVsProcNeg_ = fs->make<TH2I>(("h2HwPhiVsProcNeg"+label).c_str(), (titlePrefix+" hwPhi per Processor ID negative side;hwPhi;Processor ID").c_str(), 139, -35, 104, 6, 0, 6);
  h2HwPtVsProcPos_ = fs->make<TH2I>(("h2HwPtVsProcPos"+label).c_str(), (titlePrefix+" hwPt per Processor ID positive side;hwPt;Processor ID").c_str(), 150, 0, 300, 6, 0, 6);
  h2HwEtaVsProcPos_ = fs->make<TH2I>(("h2HwEtaVsProcPos"+label).c_str(), (titlePrefix+" hwEta per Processor ID positive side;hwEta;Processor ID").c_str(), 231, -230, 232, 6, 0, 6);
  h2HwPhiVsProcPos_ = fs->make<TH2I>(("h2HwPhiVsProcPos"+label).c_str(), (titlePrefix+" hwPhi per Processor ID positive side;hwPhi;Processor ID").c_str(), 139, -35, 104, 6, 0, 6);

  h2ProcId_ = fs->make<TH2I>(("h2ProcId"+label).c_str(), (titlePrefix+" Processer ID per BX;Processor ID;BX").c_str(), 12, 0, 12, 15, -7, 8);
  h2ProcIdNeg_ = fs->make<TH2I>(("h2ProcIdNeg"+label).c_str(), (titlePrefix+" Processer ID per BX negative side;Processor ID;BX").c_str(), 6, 0, 6, 15, -7, 8);
  h2ProcIdPos_ = fs->make<TH2I>(("h2ProcIdPos"+label).c_str(), (titlePrefix+" Processer ID per BX positive side;Processor ID;BX").c_str(), 6, 0, 6, 15, -7, 8);
  h2LinkId_ = fs->make<TH2I>(("h2LinkId"+label).c_str(), (titlePrefix+" Link ID per BX;Link ID;BX").c_str(), 36, 36, 72, 15, -7, 8);
  h2HwPt_ = fs->make<TH2I>(("h2HwPt"+label).c_str(), (titlePrefix+" hwPt per BX;hwPt;BX").c_str(), 150, 0, 300, 15, -7, 8);
  h2HwEta_ = fs->make<TH2I>(("h2HwEta"+label).c_str(), (titlePrefix+" hwEta per BX;hwEta;BX").c_str(), 231, -230, 232, 15, -7, 8);
  h2HwPhi_ = fs->make<TH2I>(("h2HwPhi"+label).c_str(), (titlePrefix+" hwPhi per BX;hwPhi;BX").c_str(), 139, -35, 104, 15, -7, 8);
  h2HwGlobalPhi_ = fs->make<TH2I>(("h2HwGlobalPhi"+label).c_str(), (titlePrefix+" hwGlobalPhi per BX;global #phi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwGlobalPhiNeg_ = fs->make<TH2I>(("h2HwGlobalPhiNeg"+label).c_str(), (titlePrefix+" hwGlobalPhi per BX negative side;global #phi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwGlobalPhiPos_ = fs->make<TH2I>(("h2HwGlobalPhiPos"+label).c_str(), (titlePrefix+" hwGlobalPhi per BX positive side;global #phi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwSign_ = fs->make<TH2I>(("h2HwSign"+label).c_str(), (titlePrefix+" hwSign per BX;Sign;BX").c_str(), 3, -1, 2, 15, -7, 8);
  h2HwSignValid_ = fs->make<TH2I>(("h2HwSignValid"+label).c_str(), (titlePrefix+" hwSignValid per BX;Sign valid;BX").c_str(), 2, 0, 2, 15, -7, 8);
  h2HwQual_ = fs->make<TH2I>(("h2HwQual"+label).c_str(), (titlePrefix+" hwQual per BX;Quality;BX").c_str(), 16, 0, 16, 15, -7, 8);
  h2TfType_ = fs->make<TH2I>(("h2TfType"+label).c_str(), (titlePrefix+" tfType per BX;TF type;BX").c_str(), 5, 0, 5, 15, -7, 8);
  h2Pt_ = fs->make<TH2F>(("h2Pt"+label).c_str(), (titlePrefix+" p_{T} per BX;GeV;BX").c_str(), 150, 0, 150, 15, -7, 8);
  h2Eta_ = fs->make<TH2F>(("h2Eta"+label).c_str(), (titlePrefix+" #eta per BX;#eta;BX").c_str(), 100, -2.5, 2.5, 15, -7, 8);
  h2GlobalPhi_ = fs->make<TH2F>(("h2GlobalPhi"+label).c_str(), (titlePrefix+" global #phi per BX;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);
  h2GlobalPhiNeg_ = fs->make<TH2F>(("h2GlobalPhiNeg"+label).c_str(), (titlePrefix+" global #phi per BX negative side;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);
  h2GlobalPhiPos_ = fs->make<TH2F>(("h2GlobalPhiPos"+label).c_str(), (titlePrefix+" global #phi per BX positive side;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);

  hHwPt_ = fs->make<TH1I>(("hHwPt"+label).c_str(), (titlePrefix+" hwPt").c_str(), 300, 0, 300);
  hHwEta_ = fs->make<TH1I>(("hHwEta"+label).c_str(), (titlePrefix+" hwEta").c_str(), 462, -230, 232);
  hHwPhi_ = fs->make<TH1I>(("hHwPhi"+label).c_str(), (titlePrefix+" hwPhi").c_str(), 139, -35, 104);
  hHwGlobalPhi_ = fs->make<TH1I>(("hHwGlobalPhi"+label).c_str(), (titlePrefix+" hwGlobalPhi").c_str(), 580, 0, 580);
  hHwGlobalPhiNeg_ = fs->make<TH1I>(("hHwGlobalPhiNeg"+label).c_str(), (titlePrefix+" hwGlobalPhiNeg").c_str(), 580, 0, 580);
  hHwGlobalPhiPos_ = fs->make<TH1I>(("hHwGlobalPhiPos"+label).c_str(), (titlePrefix+" hwGlobalPhiPos").c_str(), 580, 0, 580);
  hHwSign_ = fs->make<TH1I>(("hHwSign"+label).c_str(), (titlePrefix+" hwSign").c_str(), 3, -1, 2);
  hHwSignValid_ = fs->make<TH1I>(("hHwSignValid"+label).c_str(), (titlePrefix+" hwSignValid").c_str(), 2, 0, 2);
  hHwQual_ = fs->make<TH1I>(("hHwQual"+label).c_str(), (titlePrefix+" hwQual").c_str(), 16, 0, 16);
  hTfType_ = fs->make<TH1I>(("hTfType"+label).c_str(), (titlePrefix+" hTfType").c_str(), 5, 0, 5);
  hMuPerBxNeg_ = fs->make<TH1I>(("hMuPerBxNeg"+label).c_str(), (titlePrefix+" muons per BX negative side;BX").c_str(), 15, -7, 8);
  hMuPerBxPos_ = fs->make<TH1I>(("hMuPerBxPos"+label).c_str(), (titlePrefix+" muons per BX positive side;BX").c_str(), 15, -7, 8);
  hPt_ = fs->make<TH1F>(("hPt"+label).c_str(), (titlePrefix+" p_{T};GeV").c_str(), 150, 0, 150);
  hEta_ = fs->make<TH1F>(("hEta"+label).c_str(), (titlePrefix+" #eta;#eta").c_str(), 100, -2.5, 2.5);
  hGlobalPhi_ = fs->make<TH1F>(("hGlobalPhi"+label).c_str(), (titlePrefix+" global #phi;#phi").c_str(), 128, 0., 6.4);
  hGlobalPhiNeg_ = fs->make<TH1F>(("hGlobalPhiNeg"+label).c_str(), (titlePrefix+" global #phi negative side;#phi").c_str(), 128, 0., 6.4);
  hGlobalPhiPos_ = fs->make<TH1F>(("hGlobalPhiPos"+label).c_str(), (titlePrefix+" global #phi positive side;#phi").c_str(), 128, 0., 6.4);

  h2EtaPhi_ = fs->make<TH2F>(("h2EtaPhi"+label).c_str(), (titlePrefix+" EtaPhi;#eta;#phi").c_str(), 100, -2.5, 2.5, 128, 0., 6.4);
}

void
TfHistos::FillMuon(const l1t::RegionalMuonCand& muon, const int bx)
{
  l1t::tftype tfType = muon.trackFinderType();
  int proc = muon.processor();
  int link = muon.link();
  int globalPhi = l1t::MicroGMTConfiguration::calcGlobalPhi(muon.hwPhi(), tfType, proc);

  h2ProcId_->Fill(proc, bx);
  h2LinkId_->Fill(link, bx);
  h2HwPt_->Fill(muon.hwPt(), bx);
  h2HwEta_->Fill(muon.hwEta(), bx);
  h2HwPhi_->Fill(muon.hwPhi(), bx);
  h2HwGlobalPhi_->Fill(globalPhi, bx);
  h2HwSign_->Fill(muon.hwSign(), bx);
  h2HwSignValid_->Fill(muon.hwSignValid(), bx);
  h2HwQual_->Fill(muon.hwQual(), bx);
  h2TfType_->Fill(tfType, bx);
  h2Pt_->Fill(muon.hwPt() * ptScale_, bx);
  h2Eta_->Fill(muon.hwEta() * etaScale_, bx);
  h2GlobalPhi_->Fill(globalPhi * phiScale_, bx);

  hHwPt_->Fill(muon.hwPt());
  hHwEta_->Fill(muon.hwEta());
  hHwPhi_->Fill(muon.hwPhi());
  hHwGlobalPhi_->Fill(globalPhi);
  hHwSign_->Fill(muon.hwSign());
  hHwSignValid_->Fill(muon.hwSignValid());
  hHwQual_->Fill(muon.hwQual());
  hTfType_->Fill((int)tfType);
  int fillProc = proc;
  if (tfType == l1t::tftype::omtf_neg || tfType == l1t::tftype::emtf_neg) {
    fillProc += 6;
    h2HwPtVsProcNeg_->Fill(muon.hwPt(), proc);
    h2HwEtaVsProcNeg_->Fill(muon.hwEta(), proc);
    h2HwPhiVsProcNeg_->Fill(muon.hwPhi(), proc);
    h2ProcIdNeg_->Fill(proc, bx);
    h2HwGlobalPhiNeg_->Fill(globalPhi, bx);
    h2GlobalPhiNeg_->Fill(globalPhi * phiScale_, bx);
    hMuPerBxNeg_->Fill(bx);
    hHwGlobalPhiNeg_->Fill(globalPhi);
    hGlobalPhiNeg_->Fill(globalPhi * phiScale_);
  } else if (tfType == l1t::tftype::omtf_pos || tfType == l1t::tftype::emtf_pos) {
    h2HwPtVsProcPos_->Fill(muon.hwPt(), proc);
    h2HwEtaVsProcPos_->Fill(muon.hwEta(), proc);
    h2HwPhiVsProcPos_->Fill(muon.hwPhi(), proc);
    h2ProcIdPos_->Fill(proc, bx);
    h2HwGlobalPhiPos_->Fill(globalPhi, bx);
    h2GlobalPhiPos_->Fill(globalPhi * phiScale_, bx);
    hMuPerBxPos_->Fill(bx);
    hHwGlobalPhiPos_->Fill(globalPhi);
    hGlobalPhiPos_->Fill(globalPhi * phiScale_);
  } else {
    if (muon.hwEta() < 0) {
      h2HwPtVsProcNeg_->Fill(muon.hwPt(), proc);
      h2HwEtaVsProcNeg_->Fill(muon.hwEta(), proc);
      h2HwPhiVsProcNeg_->Fill(muon.hwPhi(), proc);
      h2ProcIdNeg_->Fill(proc, bx);
      h2HwGlobalPhiNeg_->Fill(globalPhi, bx);
      h2GlobalPhiNeg_->Fill(globalPhi * phiScale_, bx);
      hMuPerBxNeg_->Fill(bx);
      hHwGlobalPhiNeg_->Fill(globalPhi);
      hGlobalPhiNeg_->Fill(globalPhi * phiScale_);
    } else {
      h2HwPtVsProcPos_->Fill(muon.hwPt(), proc);
      h2HwEtaVsProcPos_->Fill(muon.hwEta(), proc);
      h2HwPhiVsProcPos_->Fill(muon.hwPhi(), proc);
      h2ProcIdPos_->Fill(proc, bx);
      h2HwGlobalPhiPos_->Fill(globalPhi, bx);
      h2GlobalPhiPos_->Fill(globalPhi * phiScale_, bx);
      hMuPerBxPos_->Fill(bx);
      hHwGlobalPhiPos_->Fill(globalPhi);
      hGlobalPhiPos_->Fill(globalPhi * phiScale_);
    }
  }
  hPt_->Fill(muon.hwPt() * ptScale_);
  hEta_->Fill(muon.hwEta() * etaScale_);
  hGlobalPhi_->Fill(globalPhi * phiScale_);

  h2HwPtVsProc_->Fill(muon.hwPt(), fillProc);
  h2HwEtaVsProc_->Fill(muon.hwEta(), fillProc);
  h2HwPhiVsProc_->Fill(muon.hwPhi(), fillProc);


  h2EtaPhi_->Fill(muon.hwEta()*etaScale_, globalPhi*phiScale_);
}


class UGmtHistos : public MuonHistos {
public:
  UGmtHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~UGmtHistos() {};

  TH2I *h2HwPt_;
  TH2I *h2HwEta_;
  TH2I *h2HwPhi_;
  TH2I *h2HwCharge_;
  TH2I *h2HwChargeValid_;
  TH2I *h2HwQual_;
  TH2F *h2Pt_;
  TH2F *h2Eta_;
  TH2F *h2Phi_;

  TH1I *hHwPt_;
  TH1I *hHwEta_;
  TH1I *hHwPhi_;
  TH1I *hHwCharge_;
  TH1I *hHwChargeValid_;
  TH1I *hHwQual_;
  TH1I *hHwIso_;
  TH1I *hTfMuonIndex_;
  TH1F *hPt_;
  TH1F *hEta_;
  TH1F *hPhi_;

  TH2F *h2EtaPhi_;

  void FillMuon(const l1t::Muon& muon, const int bx);
};

UGmtHistos::UGmtHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : MuonHistos(fs, label, titlePrefix)
{
  h2HwPt_ = fs->make<TH2I>(("h2HwPt"+label).c_str(), (titlePrefix+" hwPt per BX;hwPt;BX").c_str(), 150, 0, 300, 15, -7, 8);
  h2HwEta_ = fs->make<TH2I>(("h2HwEta"+label).c_str(), (titlePrefix+" hwEta per BX;hwEta;BX").c_str(), 231, -230, 232, 15, -7, 8);
  h2HwPhi_ = fs->make<TH2I>(("h2HwPhi"+label).c_str(), (titlePrefix+" hwPhi per BX;hwPhi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwCharge_ = fs->make<TH2I>(("h2HwCharge"+label).c_str(), (titlePrefix+" hwCharge per BX;charge;BX").c_str(), 3, -1, 2, 15, -7, 8);
  h2HwChargeValid_ = fs->make<TH2I>(("h2HwChargeValid"+label).c_str(), (titlePrefix+" hwChargeValid per BX;charge valid;BX").c_str(), 2, 0, 2, 15, -7, 8);
  h2HwQual_ = fs->make<TH2I>(("h2HwQual"+label).c_str(), (titlePrefix+" hwQual per BX;Quality;BX").c_str(), 16, 0, 16, 15, -7, 8);
  h2Pt_ = fs->make<TH2F>(("h2Pt"+label).c_str(), (titlePrefix+" p_{T} per BX;GeV;BX").c_str(), 150, 0, 150, 15, -7, 8);
  h2Eta_ = fs->make<TH2F>(("h2Eta"+label).c_str(), (titlePrefix+" #eta per BX;#eta;BX").c_str(), 100, -2.5, 2.5, 15, -7, 8);
  h2Phi_ = fs->make<TH2F>(("h2Phi"+label).c_str(), (titlePrefix+" #phi per BX;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);

  hHwPt_ = fs->make<TH1I>(("hHwPt"+label).c_str(), (titlePrefix+" hwPt").c_str(), 300, 0, 300);
  hHwEta_ = fs->make<TH1I>(("hHwEta"+label).c_str(), (titlePrefix+" hwEta").c_str(), 462, -230, 232);
  hHwPhi_ = fs->make<TH1I>(("hHwPhi"+label).c_str(), (titlePrefix+" hwPhi").c_str(), 580, 0, 580);
  hHwCharge_ = fs->make<TH1I>(("hHwCharge"+label).c_str(), (titlePrefix+" hwCharge").c_str(), 3, -1, 2);
  hHwChargeValid_ = fs->make<TH1I>(("hHwChargeValid"+label).c_str(), (titlePrefix+" hwChargeValid").c_str(), 2, 0, 2);
  hHwQual_ = fs->make<TH1I>(("hHwQual"+label).c_str(), (titlePrefix+" hwQual").c_str(), 16, 0, 16);
  hHwIso_ = fs->make<TH1I>(("hHwIso"+label).c_str(), (titlePrefix+" hwIso").c_str(), 4, 0, 4);
  hTfMuonIndex_ = fs->make<TH1I>(("hTfMuonIndex"+label).c_str(), (titlePrefix+" tfMuonIndex").c_str(), 108, 0, 108);
  hPt_ = fs->make<TH1F>(("hPt"+label).c_str(), (titlePrefix+" p_{T};GeV").c_str(), 150, 0, 150);
  hEta_ = fs->make<TH1F>(("hEta"+label).c_str(), (titlePrefix+" #eta;#eta").c_str(), 100, -2.5, 2.5);
  hPhi_ = fs->make<TH1F>(("hPhi"+label).c_str(), (titlePrefix+" #phi;#phi").c_str(), 128, 0., 6.4);

  h2EtaPhi_ = fs->make<TH2F>(("h2EtaPhi"+label).c_str(), (titlePrefix+" EtaPhi;#eta;#phi").c_str(), 100, -2.5, 2.5, 128, 0., 6.4);
}

void
UGmtHistos::FillMuon(const l1t::Muon& muon, const int bx)
{
  h2HwPt_->Fill(muon.hwPt(), bx);
  h2HwEta_->Fill(muon.hwEta(), bx);
  h2HwPhi_->Fill(muon.hwPhi(), bx);
  h2HwCharge_->Fill(muon.hwCharge(), bx);
  h2HwChargeValid_->Fill(muon.hwChargeValid(), bx);
  h2HwQual_->Fill(muon.hwQual(), bx);
  h2Pt_->Fill(muon.hwPt() * ptScale_, bx);
  h2Eta_->Fill(muon.hwEta() * etaScale_, bx);
  h2Phi_->Fill(muon.hwPhi() * phiScale_, bx);

  hHwPt_->Fill(muon.hwPt());
  hHwEta_->Fill(muon.hwEta());
  hHwPhi_->Fill(muon.hwPhi());
  hHwCharge_->Fill(muon.hwCharge());
  hHwChargeValid_->Fill(muon.hwChargeValid());
  hHwQual_->Fill(muon.hwQual());
  hHwIso_->Fill(muon.hwIso());
  hTfMuonIndex_->Fill(muon.tfMuonIndex());
  hPt_->Fill(muon.hwPt() * ptScale_);
  hEta_->Fill(muon.hwEta() * etaScale_);
  hPhi_->Fill(muon.hwPhi() * phiScale_);

  h2EtaPhi_->Fill(muon.hwEta()*etaScale_, muon.hwPhi()*phiScale_);
}


class L1MuonAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1MuonAnalyzer(const edm::ParameterSet&);
  ~L1MuonAnalyzer();

protected:
  float ptScale_;
  float etaScale_;
  float phiScale_;

private:
  enum trackFinder {bmtf, omtf, emtf};

  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  bool checkIfAnalyseThis(const l1t::RegionalMuonCandBxCollection& muons);
  bool checkIfValidationEvent(const l1t::RegionalMuonCandBxCollection& muons);
  void analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, TfHistos& histos, const std::string& label, int nMin = 1);
  void analyzeMuon(const l1t::MuonBxCollection& muons, UGmtHistos& histos, const std::string& label, int nMin = 1, int bxMin = -1000, int bxMax = 1000);
  void compareTFtoGMT(const edm::Event& iEvent, const edm::InputTag& tfTag, const std::vector<int>& tfBXShifts, trackFinder);
  void compareUGMTtoGMT(const edm::Event& iEvent, const edm::InputTag& ugmtTag);
  void printErrorEvents(const std::set<edm::EventNumber_t> &evts, const std::string &line);

  int bxId_;

  int fedId_;

  edm::EventNumber_t evt_;

  // EDM input tags
  edm::InputTag fedRawDataTag_;
  edm::InputTag bmtfTag_;
  edm::InputTag omtfTag_;
  edm::InputTag emtfTag_;
  edm::InputTag ugmtEmulatorTag_;
  edm::InputTag ugmtUnpackerTag_;
  edm::InputTag gmtTag_;

  bool debug_;
  bool printMismatch_;

  bool suppressCommas_;

  bool valEvts_;
  bool nonValEvts_;

  std::vector<int> bmtfBxShifts_;
  std::vector<int> omtfBxShifts_;
  std::vector<int> emtfBxShifts_;

  bool analyzeBmtf_;
  bool analyzeOmtf_;
  bool analyzeEmtf_;
  bool analyzeUgmtEmulator_;
  bool analyzeUgmtUnpacker_;
  bool analyzeGmt_;

  std::set<edm::EventNumber_t> errEvts_;
  std::set<edm::EventNumber_t> sizeErrEvts_;
  std::set<edm::EventNumber_t> ptErrEvts_;
  std::set<edm::EventNumber_t> etaErrEvts_;
  std::set<edm::EventNumber_t> phiErrEvts_;
  std::set<edm::EventNumber_t> qualErrEvts_;
  std::set<edm::EventNumber_t> chargeErrEvts_;
  std::set<edm::EventNumber_t> chargeValidErrEvts_;
  std::set<edm::EventNumber_t> tfMuonIndexErrEvts_;
  std::set<edm::EventNumber_t> isoErrEvts_;
  std::set<edm::EventNumber_t> hwPtAOREvts_;
  std::set<edm::EventNumber_t> hwEtaAOREvts_;
  std::set<edm::EventNumber_t> hwPhiAOREvts_;
  std::set<edm::EventNumber_t> bmtfHwPhiAOREvts_;
  std::set<edm::EventNumber_t> omtfHwPhiAOREvts_;
  std::set<edm::EventNumber_t> emtfHwPhiAOREvts_;

  // output file
  edm::Service<TFileService> fs_;

  TfHistos bmtfHistos_;
  TfHistos bmtfHistosN2plus_;
  TfHistos bmtfHistosValEvt_;
  TfHistos omtfHistos_;
  TfHistos omtfHistosN2plus_;
  TfHistos omtfHistosValEvt_;
  TfHistos emtfHistos_;
  TfHistos emtfHistosN2plus_;
  TfHistos emtfHistosValEvt_;
  UGmtHistos emulatorHistos_;
  UGmtHistos emulatorHistosN2plus_;
  UGmtHistos emulatorHistosBx0_;
  UGmtHistos emulatorHistosValEvt_;
  UGmtHistos unpackerHistos_;
  UGmtHistos unpackerHistosN2plus_;
  UGmtHistos unpackerHistosBx0_;
  UGmtHistos unpackerHistosValEvt_;

  TH1I *hBxId_;

  TH1I *hMuSizeDiff_;
  TH1I *hMuPtDiff_;
  TH1I *hMuEtaDiff_;
  TH1I *hMuPhiDiff_;
  TH1I *hMuChargeDiff_;
  TH1I *hMuChargeValidDiff_;
  TH1I *hMuQualDiff_;
  TH1I *hMuIsoDiff_;

  TH2I *h2MuMuSize_;
  TH2I *h2MuMuHwPt_;
  TH2I *h2MuMuHwEta_;
  TH2I *h2MuMuHwPhi_;
  TH2I *h2MuMuHwQual_;
  TH2I *h2MuMuHwCharge_;
  TH2I *h2MuMuHwChargeValid_;
  TH2I *h2MuMuTfMuonIndex_;
  TH2I *h2MuMuHwIso_;

  TH2F *h2MatchDRBmtfVsGmtPt_;
  TH2F *h2MatchDRBmtfVsGmtEta_;
  TH2F *h2MatchDRBmtfVsGmtPhi_;
  TH2F *h2MatchDROmtfVsGmtPt_;
  TH2F *h2MatchDROmtfVsGmtEta_;
  TH2F *h2MatchDROmtfVsGmtPhi_;
  TH2F *h2MatchDREmtfVsGmtPt_;
  TH2F *h2MatchDREmtfVsGmtEta_;
  TH2F *h2MatchDREmtfVsGmtPhi_;
  TH2F *h2MatchEtaBmtfVsGmtPt_;
  TH2F *h2MatchEtaBmtfVsGmtEta_;
  TH2F *h2MatchEtaBmtfVsGmtPhi_;
  TH2F *h2MatchEtaOmtfVsGmtPt_;
  TH2F *h2MatchEtaOmtfVsGmtEta_;
  TH2F *h2MatchEtaOmtfVsGmtPhi_;
  TH2F *h2MatchEtaEmtfVsGmtPt_;
  TH2F *h2MatchEtaEmtfVsGmtEta_;
  TH2F *h2MatchEtaEmtfVsGmtPhi_;
  TH2F *h2MatchPhiBmtfVsGmtPt_;
  TH2F *h2MatchPhiBmtfVsGmtEta_;
  TH2F *h2MatchPhiBmtfVsGmtPhi_;
  TH2F *h2MatchPhiOmtfVsGmtPt_;
  TH2F *h2MatchPhiOmtfVsGmtEta_;
  TH2F *h2MatchPhiOmtfVsGmtPhi_;
  TH2F *h2MatchPhiEmtfVsGmtPt_;
  TH2F *h2MatchPhiEmtfVsGmtEta_;
  TH2F *h2MatchPhiEmtfVsGmtPhi_;
  TH2F *h2UGmtVsGmtPt_;
  TH2F *h2UGmtVsGmtEta_;
  TH2F *h2UGmtVsGmtPhi_;

  TH1I *hGmtBxId_;
  TH2F *h2GmtEtaPhi_;

  TTree *tree_;
  std::vector<float> bestMatchDRBmtfGmtDR_;
  std::vector<float> bestMatchDROmtfGmtDR_;
  std::vector<float> bestMatchDREmtfGmtDR_;
  std::vector<int> bestMatchDRTfGmtLink_;
  std::vector<float> diffMatchDRBmtfGmtPt_;
  std::vector<float> diffMatchDRBmtfGmtEta_;
  std::vector<float> diffMatchDRBmtfGmtPhi_;
  std::vector<float> diffMatchDROmtfGmtPt_;
  std::vector<float> diffMatchDROmtfGmtEta_;
  std::vector<float> diffMatchDROmtfGmtPhi_;
  std::vector<float> diffMatchDREmtfGmtPt_;
  std::vector<float> diffMatchDREmtfGmtEta_;
  std::vector<float> diffMatchDREmtfGmtPhi_;
  std::vector<int> bestMatchEtaTfGmtLink_;
  std::vector<float> diffMatchEtaBmtfGmtPt_;
  std::vector<float> diffMatchEtaBmtfGmtEta_;
  std::vector<float> diffMatchEtaBmtfGmtPhi_;
  std::vector<float> diffMatchEtaOmtfGmtPt_;
  std::vector<float> diffMatchEtaOmtfGmtEta_;
  std::vector<float> diffMatchEtaOmtfGmtPhi_;
  std::vector<float> diffMatchEtaEmtfGmtPt_;
  std::vector<float> diffMatchEtaEmtfGmtEta_;
  std::vector<float> diffMatchEtaEmtfGmtPhi_;
  std::vector<int> bestMatchPhiTfGmtLink_;
  std::vector<float> diffMatchPhiBmtfGmtPt_;
  std::vector<float> diffMatchPhiBmtfGmtEta_;
  std::vector<float> diffMatchPhiBmtfGmtPhi_;
  std::vector<float> diffMatchPhiOmtfGmtPt_;
  std::vector<float> diffMatchPhiOmtfGmtEta_;
  std::vector<float> diffMatchPhiOmtfGmtPhi_;
  std::vector<float> diffMatchPhiEmtfGmtPt_;
  std::vector<float> diffMatchPhiEmtfGmtEta_;
  std::vector<float> diffMatchPhiEmtfGmtPhi_;
  std::vector<float> diffUGmtGmtPt_;
  std::vector<float> diffUGmtGmtEta_;
  std::vector<float> diffUGmtGmtPhi_;
};


L1MuonAnalyzer::L1MuonAnalyzer(const edm::ParameterSet& iConfig) :
  ptScale_(0.5),
  etaScale_(0.010875),
  phiScale_(0.010908),
  bxId_(-1),
  fedId_(iConfig.getUntrackedParameter<int>("fedId", 1402)),
  fedRawDataTag_(iConfig.getParameter<edm::InputTag>("fedRawDataTag")),
  bmtfTag_(iConfig.getParameter<edm::InputTag>("bmtfTag")),
  omtfTag_(iConfig.getParameter<edm::InputTag>("omtfTag")),
  emtfTag_(iConfig.getParameter<edm::InputTag>("emtfTag")),
  ugmtEmulatorTag_(iConfig.getParameter<edm::InputTag>("ugmtEmulatorTag")),
  ugmtUnpackerTag_(iConfig.getParameter<edm::InputTag>("ugmtUnpackerTag")),
  gmtTag_(iConfig.getParameter<edm::InputTag>("gmtTag")),
  debug_(iConfig.getUntrackedParameter<bool>("debug", false)),
  printMismatch_(iConfig.getUntrackedParameter<bool>("printMismatch", false)),
  suppressCommas_(iConfig.getUntrackedParameter<bool>("commaSuppression", false)),
  valEvts_(iConfig.getUntrackedParameter<bool>("analyseValidationEvents", true)),
  nonValEvts_(iConfig.getUntrackedParameter<bool>("analyseNonValidationEvents", true)),
  bmtfBxShifts_(iConfig.getUntrackedParameter<std::vector<int> >("bmtfBxShifts")),
  omtfBxShifts_(iConfig.getUntrackedParameter<std::vector<int> >("omtfBxShifts")),
  emtfBxShifts_(iConfig.getUntrackedParameter<std::vector<int> >("emtfBxShifts")),
  analyzeBmtf_(false),
  analyzeOmtf_(false),
  analyzeEmtf_(false),
  analyzeUgmtEmulator_(false),
  analyzeUgmtUnpacker_(false),
  analyzeGmt_(false),
  bmtfHistos_(fs_, "Bmtf", "BMTF"),
  bmtfHistosN2plus_(fs_, "BmtfN2plus", "BMTFn2plus"),
  bmtfHistosValEvt_(fs_, "BmtfValEvt", "BMTF Validation Event"),
  omtfHistos_(fs_, "Omtf", "OMTF"),
  omtfHistosN2plus_(fs_, "OmtfN2plus", "OMTFn2plus"),
  omtfHistosValEvt_(fs_, "OmtfValEvt", "OMTF Validation Event"),
  emtfHistos_(fs_, "Emtf", "EMTF"),
  emtfHistosN2plus_(fs_, "EmtfN2plus", "EMTFn2plus"),
  emtfHistosValEvt_(fs_, "EmtfValEvt", "EMTF Validation Event"),
  emulatorHistos_(fs_, "Emulator", "Emulator uGMT"),
  emulatorHistosN2plus_(fs_, "EmulatorN2plus", "Emulator uGMT n2plus"),
  emulatorHistosBx0_(fs_, "EmulatorBX0", "Emulator uGMT BX0"),
  emulatorHistosValEvt_(fs_, "EmulatorValEvt", "Emulator uGMT Validation Event"),
  unpackerHistos_(fs_, "Unpacker", "Unpacked uGMT"),
  unpackerHistosN2plus_(fs_, "UnpackerN2plus", "Unpacked uGMT n2plus"),
  unpackerHistosBx0_(fs_, "UnpackerBX0", "Unpacked uGMT BX0"),
  unpackerHistosValEvt_(fs_, "UnpackerValEvt", "Unpacked uGMT Validation Event")
{
  consumes<FEDRawDataCollection>(fedRawDataTag_);

  if (bmtfTag_.label() != "none") {
    analyzeBmtf_ = true;
    consumes<l1t::RegionalMuonCandBxCollection>(bmtfTag_);
  }
  if (omtfTag_.label() != "none") {
    analyzeOmtf_ = true;
    consumes<l1t::RegionalMuonCandBxCollection>(omtfTag_);
  }
  if (emtfTag_.label() != "none") {
    analyzeEmtf_ = true;
    consumes<l1t::RegionalMuonCandBxCollection>(emtfTag_);
  }
  if (ugmtEmulatorTag_.label() != "none") {
    analyzeUgmtEmulator_ = true;
    consumes<l1t::MuonBxCollection>(ugmtEmulatorTag_);
  }
  if (ugmtUnpackerTag_.label() != "none") {
    analyzeUgmtUnpacker_ = true;
    consumes<l1t::MuonBxCollection>(ugmtUnpackerTag_);
  }
  if (gmtTag_.label() != "none") {
    analyzeGmt_ = true;
    consumes<L1MuGMTReadoutCollection>(gmtTag_);
  }

  hBxId_ = fs_->make<TH1I>("hBxId", "BX id", 370, 0, 3700);

  hMuSizeDiff_ = fs_->make<TH1I>("hMuSizeDiff", "Emulator muon size - unpacker muon size for BX 0", 17, -8, 9);
  hMuPtDiff_ = fs_->make<TH1I>("hMuPtDiff", "Emulator muon p_{T} - unpacker muon p_{T} for BX 0", 521, -260, 261);
  hMuEtaDiff_ = fs_->make<TH1I>("hMuEtaDiff", "Emulator muon #eta - unpacker muon #eta for BX 0", 461, -230, 231);
  hMuPhiDiff_ = fs_->make<TH1I>("hMuPhiDiff", "Emulator muon #phi - unpacker muon #phi for BX 0", 581, -290, 291);
  hMuChargeDiff_ = fs_->make<TH1I>("hMuChargeDiff", "Emulator muon charge - unpacker muon charge for BX 0", 5, -2, 3);
  hMuChargeValidDiff_ = fs_->make<TH1I>("hMuChargeValidDiff", "Emulator muon chargeValid - unpacker muon chargeValid for BX 0", 3, -1, 2);
  hMuQualDiff_ = fs_->make<TH1I>("hMuQualDiff", "Emulator muon qual - unpacker muon qual for BX 0", 31, -15, 16);
  hMuIsoDiff_ = fs_->make<TH1I>("hMuIsoDiff", "Emulator muon iso - unpacker muon iso for BX 0", 7, -3, 4);

  h2MuMuSize_ = fs_->make<TH2I>("h2MuMuSize", "HW vs. emulator size", 9, 0, 9, 9, 0, 9);
  h2MuMuHwPt_ = fs_->make<TH2I>("h2MuMuHwPt", "HW vs. emulator hwPt", 512, 0, 512, 512, 0, 512);
  h2MuMuHwEta_ = fs_->make<TH2I>("h2MuMuHwEta", "HW vs. emulator hwEta", 482, -240, 242, 482, -240, 242);
  h2MuMuHwPhi_ = fs_->make<TH2I>("h2MuMuHwPhi", "HW vs. emulator hwPhi", 580, 0, 580, 580, 0, 580);
  h2MuMuHwQual_ = fs_->make<TH2I>("h2MuMuHwQual", "HW vs. emulator hwQual", 16, 0, 16, 16, 0, 16);
  h2MuMuHwCharge_ = fs_->make<TH2I>("h2MuMuHwCharge", "HW vs. emulator hwCharge", 3, -1, 2, 3, -1, 2);
  h2MuMuHwChargeValid_ = fs_->make<TH2I>("h2MuMuHwChargeValid", "HW vs. emulator hwChargeValid", 2, 0, 2, 2, 0, 2);
  h2MuMuTfMuonIndex_ = fs_->make<TH2I>("h2MuMuTfMuonIndex", "HW vs. emulator tfMuonIndex", 108, 0, 108, 108, 0, 108);
  h2MuMuHwIso_ = fs_->make<TH2I>("h2MuMuHwIso", "HW vs. emulator hwIso", 4, 0, 4, 4, 0, 4);

  h2MatchDRBmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchDRBmtfVsGmtPt", "GMT vs. BMTF p_{T}, best #DeltaR match;GMT p_{T};BMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchDRBmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchDRBmtfVsGmtEta", "GMT vs. BMTF #eta, best #DeltaR match;GMT #eta;BMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchDRBmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchDRBmtfVsGmtPhi", "GMT vs. BMTF #phi, best #DeltaR match;GMT #phi;BMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchDROmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchDROmtfVsGmtPt", "GMT vs. OMTF p_{T}, best #DeltaR match;GMT p_{T};OMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchDROmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchDROmtfVsGmtEta", "GMT vs. OMTF #eta, best #DeltaR match;GMT #eta;OMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchDROmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchDROmtfVsGmtPhi", "GMT vs. OMTF #phi, best #DeltaR match;GMT #phi;OMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchDREmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchDREmtfVsGmtPt", "GMT vs. EMTF p_{T}, best #DeltaR match;GMT p_{T};EMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchDREmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchDREmtfVsGmtEta", "GMT vs. EMTF #eta, best #DeltaR match;GMT #eta;EMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchDREmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchDREmtfVsGmtPhi", "GMT vs. EMTF #phi, best #DeltaR match;GMT #phi;EMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchEtaBmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchEtaBmtfVsGmtPt", "GMT vs. BMTF p_{T}, best #eta match;GMT p_{T};BMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchEtaBmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchEtaBmtfVsGmtEta", "GMT vs. BMTF #eta, best #eta match;GMT #eta;BMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchEtaBmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchEtaBmtfVsGmtPhi", "GMT vs. BMTF #phi, best #eta match;GMT #phi;BMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchEtaOmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchEtaOmtfVsGmtPt", "GMT vs. OMTF p_{T}, best #eta match;GMT p_{T};OMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchEtaOmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchEtaOmtfVsGmtEta", "GMT vs. OMTF #eta, best #eta match;GMT #eta;OMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchEtaOmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchEtaOmtfVsGmtPhi", "GMT vs. OMTF #phi, best #eta match;GMT #phi;OMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchEtaEmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchEtaEmtfVsGmtPt", "GMT vs. EMTF p_{T}, best #eta match;GMT p_{T};EMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchEtaEmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchEtaEmtfVsGmtEta", "GMT vs. EMTF #eta, best #eta match;GMT #eta;EMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchEtaEmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchEtaEmtfVsGmtPhi", "GMT vs. EMTF #phi, best #eta match;GMT #phi;EMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchPhiBmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchPhiBmtfVsGmtPt", "GMT vs. BMTF p_{T}, best #phi match;GMT p_{T};BMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchPhiBmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchPhiBmtfVsGmtEta", "GMT vs. BMTF #eta, best #phi match;GMT #eta;BMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchPhiBmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchPhiBmtfVsGmtPhi", "GMT vs. BMTF #phi, best #phi match;GMT #phi;BMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchPhiOmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchPhiOmtfVsGmtPt", "GMT vs. OMTF p_{T}, best #phi match;GMT p_{T};OMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchPhiOmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchPhiOmtfVsGmtEta", "GMT vs. OMTF #eta, best #phi match;GMT #eta;OMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchPhiOmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchPhiOmtfVsGmtPhi", "GMT vs. OMTF #phi, best #phi match;GMT #phi;OMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2MatchPhiEmtfVsGmtPt_ = fs_->make<TH2F>("h2MatchPhiEmtfVsGmtPt", "GMT vs. EMTF p_{T}, best #phi match;GMT p_{T};EMTF p_{T}", 150, 0., 150, 150, 0., 150);
  h2MatchPhiEmtfVsGmtEta_ = fs_->make<TH2F>("h2MatchPhiEmtfVsGmtEta", "GMT vs. EMTF #eta, best #phi match;GMT #eta;EMTF #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2MatchPhiEmtfVsGmtPhi_ = fs_->make<TH2F>("h2MatchPhiEmtfVsGmtPhi", "GMT vs. EMTF #phi, best #phi match;GMT #phi;EMTF #phi", 128, 0., 6.4, 128, 0., 6.4);
  h2UGmtVsGmtPt_ = fs_->make<TH2F>("h2UGmtVsGmtPt", "GMT vs. uGMT p_{T};GMT p_{T};uGMT p_{T}", 150, 0., 150, 150, 0., 150);
  h2UGmtVsGmtEta_ = fs_->make<TH2F>("h2UGmtVsGmtEta", "GMT vs. uGMT #eta;GMT #eta;uGMT #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  h2UGmtVsGmtPhi_ = fs_->make<TH2F>("h2UGmtVsGmtPhi", "GMT vs. uGMT phi;GMT #phi;uGMT #phi", 128, 0., 6.4, 128, 0., 6.4);

  hGmtBxId_ = fs_->make<TH1I>("hGmtBxId", "GMT event BX", 370, 0, 3700);
  h2GmtEtaPhi_ = fs_->make<TH2F>("h2GmtEtaPhi", "GMT eta phi map;#eta;#phi", 100, -2.5, 2.5, 128, 0., 6.4);

  tree_ = fs_->make<TTree>("L1MuonAnalyzerTree", "L1MuonAnalyzerTree");
  tree_->Branch("bestMatchDRBmtfGmtDR", &bestMatchDRBmtfGmtDR_, 32000, 1);
  tree_->Branch("bestMatchDROmtfGmtDR", &bestMatchDROmtfGmtDR_, 32000, 1);
  tree_->Branch("bestMatchDREmtfGmtDR", &bestMatchDREmtfGmtDR_, 32000, 1);
  tree_->Branch("bestMatchDRTfGmtLink", &bestMatchDRTfGmtLink_, 32000, 1);
  tree_->Branch("diffMatchDRBmtfGmtPt", &diffMatchDRBmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchDRBmtfGmtEta", &diffMatchDRBmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchDRBmtfGmtPhi", &diffMatchDRBmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffMatchDROmtfGmtPt", &diffMatchDROmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchDROmtfGmtEta", &diffMatchDROmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchDROmtfGmtPhi", &diffMatchDROmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffMatchDREmtfGmtPt", &diffMatchDREmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchDREmtfGmtEta", &diffMatchDREmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchDREmtfGmtPhi", &diffMatchDREmtfGmtPhi_, 32000, 1);
  tree_->Branch("bestMatchEtaTfGmtLink", &bestMatchEtaTfGmtLink_, 32000, 1);
  tree_->Branch("diffMatchEtaBmtfGmtPt", &diffMatchEtaBmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchEtaBmtfGmtEta", &diffMatchEtaBmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchEtaBmtfGmtPhi", &diffMatchEtaBmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffMatchEtaOmtfGmtPt", &diffMatchEtaOmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchEtaOmtfGmtEta", &diffMatchEtaOmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchEtaOmtfGmtPhi", &diffMatchEtaOmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffMatchEtaEmtfGmtPt", &diffMatchEtaEmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchEtaEmtfGmtEta", &diffMatchEtaEmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchEtaEmtfGmtPhi", &diffMatchEtaEmtfGmtPhi_, 32000, 1);
  tree_->Branch("bestMatchPhiTfGmtLink", &bestMatchPhiTfGmtLink_, 32000, 1);
  tree_->Branch("diffMatchPhiBmtfGmtPt", &diffMatchPhiBmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchPhiBmtfGmtEta", &diffMatchPhiBmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchPhiBmtfGmtPhi", &diffMatchPhiBmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffMatchPhiOmtfGmtPt", &diffMatchPhiOmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchPhiOmtfGmtEta", &diffMatchPhiOmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchPhiOmtfGmtPhi", &diffMatchPhiOmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffMatchPhiEmtfGmtPt", &diffMatchPhiEmtfGmtPt_, 32000, 1);
  tree_->Branch("diffMatchPhiEmtfGmtEta", &diffMatchPhiEmtfGmtEta_, 32000, 1);
  tree_->Branch("diffMatchPhiEmtfGmtPhi", &diffMatchPhiEmtfGmtPhi_, 32000, 1);
  tree_->Branch("diffUGmtGmtPt", &diffUGmtGmtPt_, 32000, 1);
  tree_->Branch("diffUGmtGmtEta", &diffUGmtGmtEta_, 32000, 1);
  tree_->Branch("diffUGmtGmtPhi", &diffUGmtGmtPhi_, 32000, 1);
}


L1MuonAnalyzer::~L1MuonAnalyzer()
{
}


void
L1MuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bestMatchDRBmtfGmtDR_.clear();
  bestMatchDROmtfGmtDR_.clear();
  bestMatchDREmtfGmtDR_.clear();
  bestMatchDRTfGmtLink_.clear();
  diffMatchDRBmtfGmtPt_.clear();
  diffMatchDRBmtfGmtEta_.clear();
  diffMatchDRBmtfGmtPhi_.clear();
  diffMatchDROmtfGmtPt_.clear();
  diffMatchDROmtfGmtEta_.clear();
  diffMatchDROmtfGmtPhi_.clear();
  diffMatchDREmtfGmtPt_.clear();
  diffMatchDREmtfGmtEta_.clear();
  diffMatchDREmtfGmtPhi_.clear();
  bestMatchEtaTfGmtLink_.clear();
  diffMatchEtaBmtfGmtPt_.clear();
  diffMatchEtaBmtfGmtEta_.clear();
  diffMatchEtaBmtfGmtPhi_.clear();
  diffMatchEtaOmtfGmtPt_.clear();
  diffMatchEtaOmtfGmtEta_.clear();
  diffMatchEtaOmtfGmtPhi_.clear();
  diffMatchEtaEmtfGmtPt_.clear();
  diffMatchEtaEmtfGmtEta_.clear();
  diffMatchEtaEmtfGmtPhi_.clear();
  bestMatchPhiTfGmtLink_.clear();
  diffMatchPhiBmtfGmtPt_.clear();
  diffMatchPhiBmtfGmtEta_.clear();
  diffMatchPhiBmtfGmtPhi_.clear();
  diffMatchPhiOmtfGmtPt_.clear();
  diffMatchPhiOmtfGmtEta_.clear();
  diffMatchPhiOmtfGmtPhi_.clear();
  diffMatchPhiEmtfGmtPt_.clear();
  diffMatchPhiEmtfGmtEta_.clear();
  diffMatchPhiEmtfGmtPhi_.clear();
  diffUGmtGmtPt_.clear();
  diffUGmtGmtEta_.clear();
  diffUGmtGmtPhi_.clear();

  evt_ = iEvent.eventAuxiliary().event();

  edm::Handle<FEDRawDataCollection> rawdata;
  iEvent.getByLabel(fedRawDataTag_, rawdata);
  const FEDRawData& data = rawdata->FEDData(fedId_);
  size_t size = data.size();
  if (size > 0) {
    FEDHeader header(data.data());

    bxId_ = header.bxID();
    hBxId_->Fill(bxId_);
  }

  if (analyzeBmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
    iEvent.getByLabel(bmtfTag_, bmtfMuons);
    if (bmtfMuons.isValid()) {
      analyzeRegionalMuonCand(*bmtfMuons, bmtfHistos_, "BMTF");
      analyzeRegionalMuonCand(*bmtfMuons, bmtfHistosN2plus_, "BMTF_n2plus", 2);
      if (checkIfValidationEvent(*bmtfMuons)) {
        analyzeRegionalMuonCand(*bmtfMuons, bmtfHistosValEvt_, "BMTFValEvt");
      }
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT BMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeOmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
    iEvent.getByLabel(omtfTag_, omtfMuons);
    if (omtfMuons.isValid()) {
      analyzeRegionalMuonCand(*omtfMuons, omtfHistos_, "OMTF");
      analyzeRegionalMuonCand(*omtfMuons, omtfHistosN2plus_, "OMTF_n2plus", 2);
      if (checkIfValidationEvent(*omtfMuons)) {
        analyzeRegionalMuonCand(*omtfMuons, omtfHistosValEvt_, "OMTFValEvt");
      }
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT OMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeEmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
    iEvent.getByLabel(emtfTag_, emtfMuons);
    if (emtfMuons.isValid()) {
      analyzeRegionalMuonCand(*emtfMuons, emtfHistos_, "EMTF");
      analyzeRegionalMuonCand(*emtfMuons, emtfHistosN2plus_, "EMTF_n2plus", 2);
      if (checkIfValidationEvent(*emtfMuons)) {
        analyzeRegionalMuonCand(*emtfMuons, emtfHistosValEvt_, "EMTFValEvt");
      }
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT EMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeUgmtEmulator_) {
    edm::Handle<l1t::MuonBxCollection> ugmtEmulatorMuons;
    iEvent.getByLabel(ugmtEmulatorTag_, ugmtEmulatorMuons);
    if (ugmtEmulatorMuons.isValid()) {
      analyzeMuon(*ugmtEmulatorMuons, emulatorHistos_, "Emulator");
      analyzeMuon(*ugmtEmulatorMuons, emulatorHistosN2plus_, "EmulatorN2plus", 2);
      analyzeMuon(*ugmtEmulatorMuons, emulatorHistosBx0_, "EmulatorBX0", 1, 0, 0);
      // fill validation events only histograms (if one of the TF has filled a validation event)
      if (analyzeBmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
        iEvent.getByLabel(bmtfTag_, bmtfMuons);
        if (bmtfMuons.isValid() && checkIfValidationEvent(*bmtfMuons)) {
          analyzeMuon(*ugmtEmulatorMuons, emulatorHistosValEvt_, "EmulatorValEvt");
        }
      } else if (analyzeOmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
        iEvent.getByLabel(omtfTag_, omtfMuons);
        if (omtfMuons.isValid() && checkIfValidationEvent(*omtfMuons)) {
          analyzeMuon(*ugmtEmulatorMuons, emulatorHistosValEvt_, "EmulatorValEvt");
        }
      } else if (analyzeEmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
        iEvent.getByLabel(emtfTag_, emtfMuons);
        if (emtfMuons.isValid() && checkIfValidationEvent(*emtfMuons)) {
          analyzeMuon(*ugmtEmulatorMuons, emulatorHistosValEvt_, "EmulatorValEvt");
        }
      }
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT emulator collection not found" << std::endl;
      return;
    }
  }
  if (analyzeUgmtUnpacker_) {
    edm::Handle<l1t::MuonBxCollection> ugmtUnpackerMuons;
    iEvent.getByLabel(ugmtUnpackerTag_, ugmtUnpackerMuons);
    if (ugmtUnpackerMuons.isValid()) {
      analyzeMuon(*ugmtUnpackerMuons, unpackerHistos_, "Unpacker");
      analyzeMuon(*ugmtUnpackerMuons, unpackerHistosN2plus_, "UnpackerN2plus", 2);
      analyzeMuon(*ugmtUnpackerMuons, unpackerHistosBx0_, "UnpackerBX0", 1, 0, 0);
      // fill validation events only histograms (if one of the TF has filled a validation event)
      if (analyzeBmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
        iEvent.getByLabel(bmtfTag_, bmtfMuons);
        if (bmtfMuons.isValid() && checkIfValidationEvent(*bmtfMuons)) {
          analyzeMuon(*ugmtUnpackerMuons, unpackerHistosValEvt_, "UnpackerValEvt");
        }
      } else if (analyzeOmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
        iEvent.getByLabel(omtfTag_, omtfMuons);
        if (omtfMuons.isValid() && checkIfValidationEvent(*omtfMuons)) {
          analyzeMuon(*ugmtUnpackerMuons, unpackerHistosValEvt_, "UnpackerValEvt");
        }
      } else if (analyzeEmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
        iEvent.getByLabel(emtfTag_, emtfMuons);
        if (emtfMuons.isValid() && checkIfValidationEvent(*emtfMuons)) {
          analyzeMuon(*ugmtUnpackerMuons, unpackerHistosValEvt_, "UnpackerValEvt");
        }
      }
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT unpacker collection not found" << std::endl;
      return;
    }
  }
  if (analyzeGmt_) {
    edm::Handle<L1MuGMTReadoutCollection> gmtReadoutColl;
    iEvent.getByLabel(gmtTag_, gmtReadoutColl);
    if (gmtReadoutColl.isValid()) {
      L1MuGMTReadoutCollection const* gmtrc = gmtReadoutColl.product();
      std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
      std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
      for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {

        if(igmtrr->getBxInEvent()==0) {
          hGmtBxId_->Fill(igmtrr->getBxNr());
        }

        std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter;
        std::vector<L1MuGMTExtendedCand> exc = igmtrr->getGMTCands();
        unsigned int n = 0;
        for(gmt_iter=exc.begin(); gmt_iter!=exc.end(); gmt_iter++) {
          if (n < 12 && !(*gmt_iter).empty()) {
            h2GmtEtaPhi_->Fill((*gmt_iter).etaValue(), (*gmt_iter).phiValue());
            ++n;
          }
        }
      }
    } else {
      edm::LogError("MissingProduct") << "GMT GMT readout collection not found" << std::endl;
      return;
    }
  }

  if (analyzeGmt_ && analyzeBmtf_) {
    compareTFtoGMT(iEvent, bmtfTag_, bmtfBxShifts_, trackFinder(bmtf));
  }

  if (analyzeGmt_ && analyzeOmtf_) {
    compareTFtoGMT(iEvent, omtfTag_, omtfBxShifts_, trackFinder(omtf));
  }

  if (analyzeGmt_ && analyzeEmtf_) {
    compareTFtoGMT(iEvent, emtfTag_, emtfBxShifts_, trackFinder(emtf));
  }

  if (analyzeGmt_ && analyzeUgmtUnpacker_) {
    compareUGMTtoGMT(iEvent, ugmtUnpackerTag_);
  }

  // emulator - unpacker comparisons
  if (analyzeUgmtEmulator_ && analyzeUgmtUnpacker_) {
    edm::Handle<l1t::MuonBxCollection> ugmtEmulatorMuons;
    edm::Handle<l1t::MuonBxCollection> ugmtUnpackerMuons;
    iEvent.getByLabel(ugmtEmulatorTag_, ugmtEmulatorMuons);
    iEvent.getByLabel(ugmtUnpackerTag_, ugmtUnpackerMuons);
    if (ugmtEmulatorMuons.isValid() && ugmtUnpackerMuons.isValid()) {
      hMuSizeDiff_->Fill(ugmtEmulatorMuons->size(0) - ugmtUnpackerMuons->size(0));
      auto muEmu = ugmtEmulatorMuons->begin(0);
      unsigned int ctrEmu = 0;
      for (; muEmu != ugmtEmulatorMuons->end(0) && ctrEmu < ugmtEmulatorMuons->size(0); ++muEmu, ++ctrEmu) {
        auto muUnp = ugmtUnpackerMuons->begin(0);
        auto muBestUnp = ugmtUnpackerMuons->begin(0);
        unsigned int ctrUnp = 0;
        int bestDPhi = 1e6;
        for (; muUnp != ugmtUnpackerMuons->end(0) && ctrUnp < ugmtUnpackerMuons->size(0); ++muUnp, ++ctrUnp) {
          if (abs(muEmu->hwPhi() - muUnp->hwPhi()) < bestDPhi) {
            bestDPhi = abs(muEmu->hwPhi() - muUnp->hwPhi());
            muBestUnp = muUnp;
          }
        }
        hMuPtDiff_->Fill(muEmu->hwPt() - muBestUnp->hwPt());
        hMuEtaDiff_->Fill(muEmu->hwEta() - muBestUnp->hwEta());
        hMuPhiDiff_->Fill(muEmu->hwPhi() - muBestUnp->hwPhi());
        hMuChargeDiff_->Fill(muEmu->hwCharge() - muBestUnp->hwCharge());
        hMuChargeValidDiff_->Fill(muEmu->hwChargeValid() - muBestUnp->hwChargeValid());
        hMuQualDiff_->Fill(muEmu->hwQual() - muBestUnp->hwQual());
        hMuIsoDiff_->Fill(muEmu->hwIso() - muBestUnp->hwIso());
        break;
      }

      // event by event comparison

      // find out if this is a validation event
      bool valEvt = false;
      if (analyzeBmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
        iEvent.getByLabel(bmtfTag_, bmtfMuons);
        if (bmtfMuons.isValid() && checkIfValidationEvent(*bmtfMuons)) {
          valEvt = true;
        }
      } else if (analyzeOmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
        iEvent.getByLabel(omtfTag_, omtfMuons);
        if (omtfMuons.isValid() && checkIfValidationEvent(*omtfMuons)) {
          valEvt = true;
        }
      } else if (analyzeEmtf_) {
        edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
        iEvent.getByLabel(emtfTag_, emtfMuons);
        if (emtfMuons.isValid() && checkIfValidationEvent(*emtfMuons)) {
          valEvt = true;
        }
      }

      if (debug_) {
        std::cout << "Start HW vs. emulator comparison. ValEvt=" << valEvt << std::endl;
      }

      // check BX ranges of HW and emulator collections
      if (valEvt && (ugmtUnpackerMuons->getFirstBX() != ugmtEmulatorMuons->getFirstBX() || ugmtUnpackerMuons->getLastBX() != ugmtEmulatorMuons->getLastBX())) {
        valEvt = false;
        edm::LogWarning("MuonAnalyzer") << "HW BX range (" << ugmtUnpackerMuons->getFirstBX() << " to " << ugmtUnpackerMuons->getLastBX() << ") and emuator BX range (" << ugmtEmulatorMuons->getFirstBX() << " to " << ugmtEmulatorMuons->getLastBX() << ") do not match for validation event. Override validation event flag and only analyse BX 0. Was an input TF masked or disabled?";
      }

      // analyse the muons
      for (int bx = ugmtUnpackerMuons->getFirstBX(); bx <= ugmtUnpackerMuons->getLastBX(); ++bx) {
        if (!valEvt && bx != 0) continue;

        // fill 2D histogram
        h2MuMuSize_->Fill(ugmtUnpackerMuons->size(bx), ugmtEmulatorMuons->size(bx));

        //if (ugmtUnpackerMuons->size(bx) > 1) {
        //  std::cout << "Number of muons > 1: Found " << ugmtUnpackerMuons->size(bx) << std::endl;
        //}

        if (ugmtUnpackerMuons->size(bx) != ugmtEmulatorMuons->size(bx)) {
          errEvts_.insert(evt_);
          sizeErrEvts_.insert(evt_);
          if (printMismatch_) std::cout << "Muon size difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW size: " << ugmtUnpackerMuons->size(bx) << ", emulator size: " << ugmtEmulatorMuons->size(bx) << std::endl;
        } else {
          unsigned int ctr = 0;
          auto hwMu = ugmtUnpackerMuons->begin(bx);
          auto emulMu = ugmtEmulatorMuons->begin(bx);
          while (hwMu != ugmtUnpackerMuons->end(bx) && emulMu != ugmtEmulatorMuons->end(bx) && ctr < ugmtUnpackerMuons->size(bx)) {
            // fill 2D histograms
            h2MuMuHwPt_->Fill(hwMu->hwPt(), emulMu->hwPt());
            h2MuMuHwEta_->Fill(hwMu->hwEta(), emulMu->hwEta());
            h2MuMuHwPhi_->Fill(hwMu->hwPhi(), emulMu->hwPhi());
            h2MuMuHwQual_->Fill(hwMu->hwQual(), emulMu->hwQual());
            h2MuMuHwCharge_->Fill(hwMu->hwCharge(), emulMu->hwCharge());
            h2MuMuHwChargeValid_->Fill(hwMu->hwChargeValid(), emulMu->hwChargeValid());
            h2MuMuTfMuonIndex_->Fill(hwMu->tfMuonIndex(), emulMu->tfMuonIndex());
            h2MuMuHwIso_->Fill(hwMu->hwIso(), emulMu->hwIso());

            // check for differences
            if (hwMu->hwPt() != emulMu->hwPt()) {
              errEvts_.insert(evt_);
              ptErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon pT difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW pT: " << hwMu->hwPt() << ", emulator pT: " << emulMu->hwPt() << std::endl;
            }
            if (hwMu->hwEta() != emulMu->hwEta()) {
              errEvts_.insert(evt_);
              etaErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon eta difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW eta: " << hwMu->hwEta() << ", emulator eta: " << emulMu->hwEta() << std::endl;
            }
            if (hwMu->hwPhi() != emulMu->hwPhi()) {
              errEvts_.insert(evt_);
              phiErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon phi difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW phi: " << hwMu->hwPhi() << ", emulator phi: " << emulMu->hwPhi() << std::endl;
            }
            if (hwMu->hwQual() != emulMu->hwQual()) {
              errEvts_.insert(evt_);
              qualErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon qual difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW qual: " << hwMu->hwQual() << ", emulator qual: " << emulMu->hwQual() << std::endl;
            }
            if (hwMu->hwCharge() != emulMu->hwCharge()) {
              errEvts_.insert(evt_);
              chargeErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon charge difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW charge: " << hwMu->hwCharge() << ", emulator charge: " << emulMu->hwCharge() << std::endl;
            }
            if (hwMu->hwChargeValid() != emulMu->hwChargeValid()) {
              errEvts_.insert(evt_);
              chargeValidErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon chargeValid difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW chargeValid: " << hwMu->hwChargeValid() << ", emulator chargeValid: " << emulMu->hwChargeValid() << std::endl;
            }
            if (hwMu->tfMuonIndex() != emulMu->tfMuonIndex()) {
              errEvts_.insert(evt_);
              tfMuonIndexErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon tfMuonIndex difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW tfMuonIndex: " << hwMu->tfMuonIndex() << ", emulator tfMuonIndex: " << emulMu->tfMuonIndex() << std::endl;
            }
            if (hwMu->hwIso() != emulMu->hwIso()) {
              errEvts_.insert(evt_);
              isoErrEvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon hwIso difference between HW and emulator for HW BX " << bx << " in event " << evt_ << ". HW hwIso: " << hwMu->hwIso() << ", emulator hwIso: " << emulMu->hwIso() << std::endl;
            }
            ++hwMu;
            ++emulMu;
            ++ctr;
          }
        }
      }
    }
  }

  tree_->Fill();
}


bool
L1MuonAnalyzer::checkIfAnalyseThis(const l1t::RegionalMuonCandBxCollection& muons)
{
  bool analyseThis = false;
  int firstBx = muons.getFirstBX();
  int lastBx = muons.getLastBX();
  if (valEvts_ && lastBx - firstBx != 0) {
    analyseThis = true;
  } else if (nonValEvts_ && lastBx - firstBx < 4) {
    analyseThis = true;
  }
  return analyseThis;
}


bool
L1MuonAnalyzer::checkIfValidationEvent(const l1t::RegionalMuonCandBxCollection& muons)
{
  bool valEvt = false;
  int firstBx = muons.getFirstBX();
  int lastBx = muons.getLastBX();
  if (lastBx - firstBx == 4) { // val events have BX range from -2 to +2
    valEvt = true;
  }
  return valEvt;
}


void
L1MuonAnalyzer::analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, TfHistos& histos, const std::string& label, int nMin)
{
  if (debug_) {
    std::cout << "*** " << label << " muon bx range: " << muons.getFirstBX() << " " << muons.getLastBX() << std::endl;
  }
  if (checkIfAnalyseThis(muons)) {
    for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
      int size = muons.size(bx);
      if (debug_ && size > 0) std::cout << "    BX " << bx << ": " << size << " muons" << std::endl;
      histos.FillSize(size, bx);
      if (size < nMin) continue;

      int ctr = 0;
      for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < size; ++mu, ++ctr) {
        // suppress comma events
        if (suppressCommas_ && ((mu->dataword() & 0xFFFFFFFF) == 0x505050bc || (mu->dataword() >> 32) == 0x505050bc)) {
          if (debug_) {
            std::cout << "       COMMA   ";
          }
        } else {
          histos.FillBx(bx, bxId_);
          histos.FillMuon(*mu, bx);
          
          // range checks
          if (mu->hwPt() < 0 || mu->hwPt() > 511) {
            errEvts_.insert(evt_);
            hwPtAOREvts_.insert(evt_);
            if (printMismatch_) std::cout << "Muon hwPt out of expected range (0 to 511) for BX " << bx << " in event " << evt_ << ". TF type: " << mu->trackFinderType() << ", link: " << mu->link() << ", hwPt: " << mu->hwPt() << std::endl;
          }
          if (mu->hwEta() < -230 || mu->hwEta() > 230) {
            errEvts_.insert(evt_);
            hwEtaAOREvts_.insert(evt_);
            if (printMismatch_) std::cout << "Muon hwEta out of expected range (-230 to 230) for BX " << bx << " in event " << evt_ << ". TF type: " << mu->trackFinderType() << ", link: " << mu->link() << ", hwEta: " << mu->hwEta() << std::endl;
          }
          if (mu->trackFinderType() == 0) {
            if (mu->hwPhi() < -8 || mu->hwPhi() > 56) {
              errEvts_.insert(evt_);
              bmtfHwPhiAOREvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon hwPhi out of expected range (-8 to 56) for BX " << bx << " in event " << evt_ << ". TF type: " << mu->trackFinderType() << ", link: " << mu->link() << ", hwPhi: " << mu->hwPhi() << std::endl;
            }
          } else if (mu->trackFinderType() > 2) {
            if (mu->hwPhi() < -35 || mu->hwPhi() > 100) {
              errEvts_.insert(evt_);
              emtfHwPhiAOREvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon hwPhi out of expected range (-35 to 100) for BX " << bx << " in event " << evt_ << ". TF type: " << mu->trackFinderType() << ", link: " << mu->link() << ", hwPhi: " << mu->hwPhi() << std::endl;
            }
          } else {
            if (mu->hwPhi() < -16 || mu->hwPhi() > 101) {
              errEvts_.insert(evt_);
              omtfHwPhiAOREvts_.insert(evt_);
              if (printMismatch_) std::cout << "Muon hwPhi out of expected range (-16 to 101) for BX " << bx << " in event " << evt_ << ". TF type: " << mu->trackFinderType() << ", link: " << mu->link() << ", hwPhi: " << mu->hwPhi() << std::endl;
            }
          }

          if (debug_) {
            std::cout << "       ";
          }
        }

        if (debug_) {
          std::cout << "hwPt: " << mu->hwPt()
                    << "   hwEta: " << mu->hwEta()
                    << "   hwPhi: " << mu->hwPhi()
                    << "   hwGlobalPhi: " << l1t::MicroGMTConfiguration::calcGlobalPhi(mu->hwPhi(), mu->trackFinderType(), mu->processor())
                    << "   hwSign: " << mu->hwSign()
                    << "   hwSignValid: " << mu->hwSignValid()
                    << "   hwQual: " << mu->hwQual()
                    << "   TF type: " << mu->trackFinderType()
                    << "   Processor: " << mu->processor()
                    << "   Link: " << mu->link()
                    << "   stored dataword: 0x" << hex << setw(16) << setfill('0') << mu->dataword() << dec
                    << "   dataword: 0x" << hex << setw(16) << setfill('0') << l1t::RegionalMuonRawDigiTranslator::generate64bitDataWord(*mu) << dec << std::endl;
        }
      }
    }
  }
}

void
L1MuonAnalyzer::analyzeMuon(const l1t::MuonBxCollection& muons, UGmtHistos& histos, const std::string& label, int nMin, int bxMin, int bxMax)
{
  if (debug_) {
    std::cout << "*** " << label << " muon bx range: " << muons.getFirstBX() << " " << muons.getLastBX() << std::endl;
  }
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    if (bx < bxMin || bx > bxMax) continue;
    int size = muons.size(bx);
    if (debug_ && size > 0) std::cout << "    BX " << bx << ": " << size << " muons" << std::endl;
    histos.FillSize(size, bx);
    if (size < nMin) continue;

    int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < size; ++mu, ++ctr) {
      histos.FillBx(bx, bxId_);
      histos.FillMuon(*mu, bx);

      // range checks
      if (mu->hwPt() < 0 || mu->hwPt() > 511) {
        errEvts_.insert(evt_);
        hwPtAOREvts_.insert(evt_);
        if (printMismatch_) std::cout << label << " muon hwPt out of expected range (0 to 511) for BX " << bx << " in event " << evt_ << ". hwPt: " << mu->hwPt() << std::endl;
      }
      if (mu->hwEta() < -230 || mu->hwEta() > 230) {
        errEvts_.insert(evt_);
        hwEtaAOREvts_.insert(evt_);
        if (printMismatch_) std::cout << label << " muon hwEta out of expected range (-230 to 230) for BX " << bx << " in event " << evt_ << ". hwEta: " << mu->hwEta() << std::endl;
      }
      if (mu->hwPhi() < 0 || mu->hwPhi() > 575) {
        errEvts_.insert(evt_);
        hwPhiAOREvts_.insert(evt_);
        if (printMismatch_) std::cout << label << " muon hwPhi out of expected range (0 to 575) for BX " << bx << " in event " << evt_ << ". hwPhi: " << mu->hwPhi() << std::endl;
      }

      if (debug_) {
        std::cout << "       hwPt: " << mu->hwPt()
                  << "   hwEta: " << mu->hwEta()
                  << "   hwPhi: " << mu->hwPhi()
                  << "   hwCharge: " << mu->hwCharge()
                  << "   hwChargeValid: " << mu->hwChargeValid()
                  << "   hwQual: " << mu->hwQual()
                  << "   hwIso: " << mu->hwIso()
                  << "   dataword: 0x" << hex << setw(16) << setfill('0') << l1t::MuonRawDigiTranslator::generate64bitDataWord(*mu) << dec << std::endl;
      }
    }
  }
}

void
L1MuonAnalyzer::compareTFtoGMT(const edm::Event& iEvent, const edm::InputTag& tfTag, const std::vector<int>& tfBxShifts, trackFinder tf)
{
  edm::Handle<L1MuGMTReadoutCollection> gmtReadoutColl;
  edm::Handle<l1t::RegionalMuonCandBxCollection> tfMuons;
  iEvent.getByLabel(gmtTag_, gmtReadoutColl);
  iEvent.getByLabel(tfTag, tfMuons);

  if (gmtReadoutColl.isValid() && tfMuons.isValid()) {
    L1MuGMTReadoutCollection const* gmtrc = gmtReadoutColl.product();

    // get GMT BX 0 readout record
    L1MuGMTReadoutRecord gmt_record = gmtrc->getRecord(0);
    std::vector<L1MuGMTExtendedCand> exc = gmt_record.getGMTCands();
    std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter;
    unsigned int n = 0;
    // look how many muons are in the GMT and stop if there is more than one
    for(gmt_iter=exc.begin(); gmt_iter!=exc.end(); gmt_iter++) {
      if (n < 12 && !(*gmt_iter).empty()) {
        ++n;
      }
    }
    if (n > 1) {
      return;
    }
    n = 0;
    for(gmt_iter=exc.begin(); gmt_iter!=exc.end(); gmt_iter++) {
      if (n < 12 && !(*gmt_iter).empty()) {
        ++n;
        if ((*gmt_iter).bx() != 0) {
          continue;
        }
        if (tf == bmtf && fabs((*gmt_iter).etaValue()) >= 0.83) {
          continue;
        } else if (tf == omtf && (fabs((*gmt_iter).etaValue()) < 0.83 || fabs((*gmt_iter).etaValue()) >= 1.24)) {
          continue;
        } else if (tf == emtf && fabs((*gmt_iter).etaValue()) < 1.24) {
          continue;
        }
        float bestDR = 1.e10;
        int bestDRMatchLink = -1;
        float bestDRMatchPt = 1.e10;
        float bestDRMatchEta = 1.e10;
        float bestDRMatchPhi = 1.e10;
        int bestEtaMatchLink = -1;
        float bestEtaMatchPt = 1.e10;
        float bestEtaMatchEta = 1.e10;
        float bestEtaMatchPhi = 1.e10;
        int bestPhiMatchLink = -1;
        float bestPhiMatchPt = 1.e10;
        float bestPhiMatchEta = 1.e10;
        float bestPhiMatchPhi = 1.e10;
        for (int bx = tfMuons->getFirstBX(); bx <= tfMuons->getLastBX(); ++bx) {
          if (tfMuons->size(bx) > 1) {
            continue;
          }
          unsigned int ctr = 0;
          for (auto mu = tfMuons->begin(bx); mu != tfMuons->end(bx) && ctr < tfMuons->size(bx); ++mu, ++ctr) {
            int bxShiftIdx = mu->processor();
            l1t::tftype tfType = mu->trackFinderType();
            if (tfType == l1t::tftype::omtf_neg || tfType == l1t::tftype::emtf_neg) {
              bxShiftIdx += 6;
            }
            if (bx != tfBxShifts[bxShiftIdx]) {
              continue;
            }
            int globalPhi = l1t::MicroGMTConfiguration::calcGlobalPhi(mu->hwPhi(), tfType, mu->processor());
            float dR = sqrt((gmt_iter->etaValue() - mu->hwEta() * etaScale_)*(gmt_iter->etaValue() - mu->hwEta() * etaScale_) + (gmt_iter->phiValue() - globalPhi * phiScale_)*(gmt_iter->phiValue() - globalPhi * phiScale_));
            if (dR < bestDR) {
              bestDR = dR;
              bestDRMatchLink = mu->link();
              bestDRMatchPt = mu->hwPt() * ptScale_;
              bestDRMatchEta = mu->hwEta() * etaScale_;
              bestDRMatchPhi = globalPhi * phiScale_;
            }
            if (fabs(gmt_iter->etaValue() - mu->hwEta() * etaScale_) < fabs(gmt_iter->etaValue() - bestEtaMatchEta)) {
              bestEtaMatchLink = mu->link();
              bestEtaMatchPt = mu->hwPt() * ptScale_;
              bestEtaMatchEta = mu->hwEta() * etaScale_;
              bestEtaMatchPhi = globalPhi * phiScale_;
            }
            if (fabs(gmt_iter->phiValue() - globalPhi * phiScale_) < fabs(gmt_iter->phiValue() - bestPhiMatchPhi)) {
              bestPhiMatchLink = mu->link();
              bestPhiMatchPt = mu->hwPt() * ptScale_;
              bestPhiMatchEta = mu->hwEta() * etaScale_;
              bestPhiMatchPhi = globalPhi * phiScale_;
            }
          }
        }
        if (bestDR < 1.e10) {
          bestMatchDRTfGmtLink_.push_back(bestDRMatchLink);
          if (tf == bmtf) {
            h2MatchDRBmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestDRMatchPt);
            h2MatchDRBmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestDRMatchEta);
            h2MatchDRBmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestDRMatchPhi);
            bestMatchDRBmtfGmtDR_.push_back(bestDR);
            diffMatchDRBmtfGmtPt_.push_back(bestDRMatchPt - (*gmt_iter).ptValue());
            diffMatchDRBmtfGmtEta_.push_back(bestDRMatchEta - (*gmt_iter).etaValue());
            diffMatchDRBmtfGmtPhi_.push_back(bestDRMatchPhi - (*gmt_iter).phiValue());
          } else if (tf == omtf) {
            h2MatchDROmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestDRMatchPt);
            h2MatchDROmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestDRMatchEta);
            h2MatchDROmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestDRMatchPhi);
            bestMatchDROmtfGmtDR_.push_back(bestDR);
            diffMatchDROmtfGmtPt_.push_back(bestDRMatchPt - (*gmt_iter).ptValue());
            diffMatchDROmtfGmtEta_.push_back(bestDRMatchEta - (*gmt_iter).etaValue());
            diffMatchDROmtfGmtPhi_.push_back(bestDRMatchPhi - (*gmt_iter).phiValue());
          } else if (tf == emtf) {
            h2MatchDREmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestDRMatchPt);
            h2MatchDREmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestDRMatchEta);
            h2MatchDREmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestDRMatchPhi);
            bestMatchDREmtfGmtDR_.push_back(bestDR);
            diffMatchDREmtfGmtPt_.push_back(bestDRMatchPt - (*gmt_iter).ptValue());
            diffMatchDREmtfGmtEta_.push_back(bestDRMatchEta - (*gmt_iter).etaValue());
            diffMatchDREmtfGmtPhi_.push_back(bestDRMatchPhi - (*gmt_iter).phiValue());
          }
        }
        if (bestEtaMatchEta < 1.e10) {
          bestMatchEtaTfGmtLink_.push_back(bestEtaMatchLink);
          if (tf == bmtf) {
            h2MatchEtaBmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestEtaMatchPt);
            h2MatchEtaBmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestEtaMatchEta);
            h2MatchEtaBmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestEtaMatchPhi);
            diffMatchEtaBmtfGmtPt_.push_back(bestEtaMatchPt - (*gmt_iter).ptValue());
            diffMatchEtaBmtfGmtEta_.push_back(bestEtaMatchEta - (*gmt_iter).etaValue());
            diffMatchEtaBmtfGmtPhi_.push_back(bestEtaMatchPhi - (*gmt_iter).phiValue());
          } else if (tf == omtf) {
            h2MatchEtaOmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestEtaMatchPt);
            h2MatchEtaOmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestEtaMatchEta);
            h2MatchEtaOmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestEtaMatchPhi);
            diffMatchEtaOmtfGmtPt_.push_back(bestEtaMatchPt - (*gmt_iter).ptValue());
            diffMatchEtaOmtfGmtEta_.push_back(bestEtaMatchEta - (*gmt_iter).etaValue());
            diffMatchEtaOmtfGmtPhi_.push_back(bestEtaMatchPhi - (*gmt_iter).phiValue());
          } else if (tf == emtf) {
            h2MatchEtaEmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestEtaMatchPt);
            h2MatchEtaEmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestEtaMatchEta);
            h2MatchEtaEmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestEtaMatchPhi);
            diffMatchEtaEmtfGmtPt_.push_back(bestEtaMatchPt - (*gmt_iter).ptValue());
            diffMatchEtaEmtfGmtEta_.push_back(bestEtaMatchEta - (*gmt_iter).etaValue());
            diffMatchEtaEmtfGmtPhi_.push_back(bestEtaMatchPhi - (*gmt_iter).phiValue());
          }
        }
        if (bestPhiMatchPhi < 1.e10) {
          bestMatchPhiTfGmtLink_.push_back(bestPhiMatchLink);
          if (tf == bmtf) {
            h2MatchPhiBmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestPhiMatchPt);
            h2MatchPhiBmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestPhiMatchEta);
            h2MatchPhiBmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestPhiMatchPhi);
            diffMatchPhiBmtfGmtPt_.push_back(bestPhiMatchPt - (*gmt_iter).ptValue());
            diffMatchPhiBmtfGmtEta_.push_back(bestPhiMatchEta - (*gmt_iter).etaValue());
            diffMatchPhiBmtfGmtPhi_.push_back(bestPhiMatchPhi - (*gmt_iter).phiValue());
          } else if (tf == omtf) {
            h2MatchPhiOmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestPhiMatchPt);
            h2MatchPhiOmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestPhiMatchEta);
            h2MatchPhiOmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestPhiMatchPhi);
            diffMatchPhiOmtfGmtPt_.push_back(bestPhiMatchPt - (*gmt_iter).ptValue());
            diffMatchPhiOmtfGmtEta_.push_back(bestPhiMatchEta - (*gmt_iter).etaValue());
            diffMatchPhiOmtfGmtPhi_.push_back(bestPhiMatchPhi - (*gmt_iter).phiValue());
          } else if (tf == emtf) {
            h2MatchPhiEmtfVsGmtPt_->Fill((*gmt_iter).ptValue(), bestPhiMatchPt);
            h2MatchPhiEmtfVsGmtEta_->Fill((*gmt_iter).etaValue(), bestPhiMatchEta);
            h2MatchPhiEmtfVsGmtPhi_->Fill((*gmt_iter).phiValue(), bestPhiMatchPhi);
            diffMatchPhiEmtfGmtPt_.push_back(bestPhiMatchPt - (*gmt_iter).ptValue());
            diffMatchPhiEmtfGmtEta_.push_back(bestPhiMatchEta - (*gmt_iter).etaValue());
            diffMatchPhiEmtfGmtPhi_.push_back(bestPhiMatchPhi - (*gmt_iter).phiValue());
          }
        }
      }
    }
  } else {
    edm::LogError("MissingProduct") << "GMT or TF input collection not found" << std::endl;
    return;
  }
}

void
L1MuonAnalyzer::compareUGMTtoGMT(const edm::Event& iEvent, const edm::InputTag& ugmtTag)
{
  int bx = 0;

  edm::Handle<L1MuGMTReadoutCollection> gmtReadoutColl;
  edm::Handle<l1t::MuonBxCollection> ugmtMuons;
  iEvent.getByLabel(gmtTag_, gmtReadoutColl);
  iEvent.getByLabel(ugmtTag, ugmtMuons);

  if (gmtReadoutColl.isValid() && ugmtMuons.isValid()) {
    L1MuGMTReadoutCollection const* gmtrc = gmtReadoutColl.product();

    // get GMT BX 0 readout record
    L1MuGMTReadoutRecord gmt_record = gmtrc->getRecord(0);
    std::vector<L1MuGMTExtendedCand> exc = gmt_record.getGMTCands();
    std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter;
    unsigned int n = 0;
    for(gmt_iter=exc.begin(); gmt_iter!=exc.end(); gmt_iter++) {
      if (n < 12 && !(*gmt_iter).empty()) {
        ++n;
        if ((*gmt_iter).bx() != 0) {
          continue;
        }
        if (ugmtMuons->size(bx) == 1) {
          unsigned int ctr = 0;
          for (auto mu = ugmtMuons->begin(bx); mu != ugmtMuons->end(bx) && ctr < ugmtMuons->size(bx); ++mu, ++ctr) {
            h2UGmtVsGmtPt_->Fill((*gmt_iter).ptValue(), mu->hwPt() * ptScale_);
            h2UGmtVsGmtEta_->Fill((*gmt_iter).etaValue(), mu->hwEta() * etaScale_);
            h2UGmtVsGmtPhi_->Fill((*gmt_iter).phiValue(), mu->hwPhi() * phiScale_);
            diffUGmtGmtPt_.push_back(mu->hwPt() * ptScale_ - (*gmt_iter).ptValue());
            diffUGmtGmtEta_.push_back(mu->hwEta() * etaScale_ - (*gmt_iter).etaValue());
            diffUGmtGmtPhi_.push_back(mu->hwPhi() * phiScale_ - (*gmt_iter).phiValue());
          }
        }
      }
    }
  } else {
    edm::LogError("MissingProduct") << "Input collection not found" << std::endl;
    return;
  }
}

void
L1MuonAnalyzer::printErrorEvents(const std::set<edm::EventNumber_t> &evts, const std::string &line)
{
  std::cout << line << evts.size() << std::endl;
  std::cout << "   Events: [";
  unsigned crt = 0;
  for (std::set<edm::EventNumber_t>::const_iterator it = evts.begin(); it != evts.end(); ++it, ++crt) {
    std::cout << *it;
    if (crt < evts.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}

// ------------ method called once each job just before starting event loop  ------------
void
L1MuonAnalyzer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1MuonAnalyzer::endJob() {
  std::cout << "=== Range check summary ======================" << std::endl;
  printErrorEvents(hwPtAOREvts_,        "Number of pt out-of-range events:       ");
  printErrorEvents(hwEtaAOREvts_,       "Number of eta out-of-range events:      ");
  printErrorEvents(hwPhiAOREvts_,       "Number of ugmt phi out-of-range events: ");
  printErrorEvents(bmtfHwPhiAOREvts_,   "Number of bmtf phi out-of-range events: ");
  printErrorEvents(omtfHwPhiAOREvts_,   "Number of omtf phi out-of-range events: ");
  printErrorEvents(emtfHwPhiAOREvts_,   "Number of emtf phi out-of-range events: ");
  std::cout << "=== uGMT HW to emulator comparison summary ===" << std::endl;
  printErrorEvents(sizeErrEvts_,        "Number of size mismatch events:         ");
  printErrorEvents(ptErrEvts_,          "Number of pt mismatch events:           ");
  printErrorEvents(etaErrEvts_,         "Number of eta mismatch events:          ");
  printErrorEvents(phiErrEvts_,         "Number of phi mismatch events:          ");
  printErrorEvents(qualErrEvts_,        "Number of qual mismatch events:         ");
  printErrorEvents(chargeErrEvts_,      "Number of charge mismatch events:       ");
  printErrorEvents(chargeValidErrEvts_, "Number of chargeValid mismatch events:  ");
  printErrorEvents(tfMuonIndexErrEvts_, "Number of tfMuonIndex mismatch events:  ");
  printErrorEvents(isoErrEvts_,         "Number of hwIso mismatch events:        ");
  std::cout << "=== Total ====================================" << std::endl;
  printErrorEvents(errEvts_,            "Number of non matching events:          ");
  std::cout << "==============================================" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1MuonAnalyzer);
