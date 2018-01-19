#include "DQM/L1TMonitor/interface/L1TStage2MuonCompDqm.h"

L1TStage2MuonCompDqm::L1TStage2MuonCompDqm(const edm::ParameterSet& ps)
    : coll1ExcessToken_(consumes<BXVector<edm::Ref<l1t::MuonBxCollection>>>(ps.getParameter<edm::InputTag>("collection1ExcessMuons"))),
      coll2ExcessToken_(consumes<BXVector<edm::Ref<l1t::MuonBxCollection>>>(ps.getParameter<edm::InputTag>("collection2ExcessMuons"))),
      matchToken_(consumes<BXVector<std::pair<edm::Ref<l1t::MuonBxCollection>, edm::Ref<l1t::MuonBxCollection>>>>(ps.getParameter<edm::InputTag>("matchCollection"))),
      mismatchToken_(consumes<BXVector<std::pair<edm::Ref<l1t::MuonBxCollection>, edm::Ref<l1t::MuonBxCollection>>>>(ps.getParameter<edm::InputTag>("mismatchCollection"))),
      monitorDir_(ps.getUntrackedParameter<std::string>("monitorDir")),
      muonColl1Title_(ps.getUntrackedParameter<std::string>("muonCollection1Title")),
      muonColl2Title_(ps.getUntrackedParameter<std::string>("muonCollection2Title")),
      summaryTitle_(ps.getUntrackedParameter<std::string>("summaryTitle")),
      ignoreBin_(ps.getUntrackedParameter<std::vector<int>>("ignoreBin"))
{
  // First include all bins
  for (unsigned int i = 1; i <= RIDX; ++i) {
    incBin_[i] = true;
  }
  // Then check the list of bins to ignore
  for (const auto& i : ignoreBin_) {
    if (i > 0 && i <= RIDX) {
      incBin_[i] = false;
    }
  }
}

void L1TStage2MuonCompDqm::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("collection1ExcessMuons")->setComment("L1T Muons in collection 1 but not in collection 2");
  desc.add<edm::InputTag>("collection2ExcessMuons")->setComment("L1T Muons in collection 2 but not in collection 1");
  desc.add<edm::InputTag>("matchCollection")->setComment("Collection of matching L1T Muon pairs");
  desc.add<edm::InputTag>("mismatchCollection")->setComment("Collection of matching L1T Muon pairs");
  desc.addUntracked<std::string>("monitorDir", "")->setComment("Target directory in the DQM file. Will be created if not existing.");
  desc.addUntracked<std::string>("muonCollection1Title", "Muon collection 1")->setComment("Histogram title for first collection.");
  desc.addUntracked<std::string>("muonCollection2Title", "Muon collection 2")->setComment("Histogram title for second collection.");
  desc.addUntracked<std::string>("summaryTitle", "Summary")->setComment("Title of summary histogram.");
  desc.addUntracked<std::vector<int>>("ignoreBin", std::vector<int>())->setComment("List of bins to ignore");
  descriptions.add("l1tStage2MuonCompDqm", desc);
}

void L1TStage2MuonCompDqm::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c, muoncompdqm::Histograms& histograms) const
{}

void L1TStage2MuonCompDqm::bookHistograms(DQMStore::ConcurrentBooker& booker, const edm::Run&, const edm::EventSetup&, muoncompdqm::Histograms& histograms) const
{

  // Subsystem Monitoring and Muon Output
  booker.setCurrentFolder(monitorDir_);

  histograms.summary = booker.book1D("summary", summaryTitle_, 16, 1, 17); // range to match bin numbering
  histograms.summary.setBinLabel(BXRANGEGOOD, "BX range match", 1);
  histograms.summary.setBinLabel(BXRANGEBAD, "BX range mismatch", 1);
  histograms.summary.setBinLabel(NMUONGOOD, "muon collection size match", 1);
  histograms.summary.setBinLabel(NMUONBAD, "muon collection size mismatch", 1);
  histograms.summary.setBinLabel(MUONALL, "# muons", 1);
  histograms.summary.setBinLabel(MUONGOOD, "# matching muons", 1);
  histograms.summary.setBinLabel(PTBAD, "p_{T} mismatch", 1);
  histograms.summary.setBinLabel(ETABAD, "#eta mismatch", 1);
  histograms.summary.setBinLabel(PHIBAD, "#phi mismatch", 1);
  histograms.summary.setBinLabel(ETAATVTXBAD, "#eta at vertex mismatch", 1);
  histograms.summary.setBinLabel(PHIATVTXBAD, "#phi at vertex mismatch", 1);
  histograms.summary.setBinLabel(CHARGEBAD, "charge mismatch", 1);
  histograms.summary.setBinLabel(CHARGEVALBAD, "charge valid mismatch", 1);
  histograms.summary.setBinLabel(QUALBAD, "quality mismatch", 1);
  histograms.summary.setBinLabel(ISOBAD, "iso mismatch", 1);
  histograms.summary.setBinLabel(IDXBAD, "index mismatch", 1);

  histograms.errorSummaryNum = booker.book1D("errorSummaryNum", summaryTitle_, 13, 1, 14); // range to match bin numbering
  histograms.errorSummaryNum.setBinLabel(RBXRANGE, "BX range mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RNMUON, "muon collection size mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RMUON, "mismatching muons", 1);
  histograms.errorSummaryNum.setBinLabel(RPT, "p_{T} mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RETA, "#eta mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RPHI, "#phi mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RETAATVTX, "#eta at vertex mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RPHIATVTX, "#phi at vertex mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RCHARGE, "charge mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RCHARGEVAL, "charge valid mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RQUAL, "quality mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RISO, "iso mismatch", 1);
  histograms.errorSummaryNum.setBinLabel(RIDX, "index mismatch", 1);

  // Change the label for those bins that will be ignored
  for (unsigned int i = 1; i <= RIDX; i++) {
    if (incBin_[i]==false) {
      histograms.errorSummaryNum.setBinLabel(i, "Ignored", 1);
    }
  }

  histograms.errorSummaryDen = booker.book1D("errorSummaryDen", "denominators", 13, 1, 14); // range to match bin numbering
  histograms.errorSummaryDen.setBinLabel(RBXRANGE, "# events", 1);
  histograms.errorSummaryDen.setBinLabel(RNMUON, "# muon collections", 1);
  for (int i = RMUON; i <= RIDX; ++i) {
    histograms.errorSummaryDen.setBinLabel(i, "# muons", 1);
  }

  histograms.muColl1BxRange = booker.book1D("muBxRangeColl1", muonColl1Title_+" mismatching BX range", 5, -2.5, 2.5);
  histograms.muColl1BxRange.setAxisTitle("BX range", 1);
  histograms.muColl1nMu = booker.book1D("nMuColl1", muonColl1Title_+" mismatching muon multiplicity", 9, -0.5, 8.5);
  histograms.muColl1nMu.setAxisTitle("Muon multiplicity", 1);
  histograms.muColl1hwPt = booker.book1D("muHwPtColl1", muonColl1Title_+" mismatching muon p_{T}", 512, -0.5, 511.5);
  histograms.muColl1hwPt.setAxisTitle("Hardware p_{T}", 1);
  histograms.muColl1hwEta = booker.book1D("muHwEtaColl1", muonColl1Title_+" mismatching muon #eta", 461, -230.5, 230.5);
  histograms.muColl1hwEta.setAxisTitle("Hardware #eta", 1);
  histograms.muColl1hwPhi = booker.book1D("muHwPhiColl1", muonColl1Title_+" mismatching muon #phi", 576, -0.5, 575.5);
  histograms.muColl1hwPhi.setAxisTitle("Hardware #phi", 1);
  histograms.muColl1hwEtaAtVtx = booker.book1D("muHwEtaAtVtxColl1", muonColl1Title_+" mismatching muon #eta at vertex", 461, -230.5, 230.5);
  histograms.muColl1hwEtaAtVtx.setAxisTitle("Hardware #eta at vertex", 1);
  histograms.muColl1hwPhiAtVtx = booker.book1D("muHwPhiAtVtxColl1", muonColl1Title_+" mismatching muon #phi at vertex", 576, -0.5, 575.5);
  histograms.muColl1hwPhiAtVtx.setAxisTitle("Hardware #phi at vertex", 1);
  histograms.muColl1hwCharge = booker.book1D("muHwChargeColl1", muonColl1Title_+" mismatching muon charge", 2, -0.5, 1.5);
  histograms.muColl1hwCharge.setAxisTitle("Hardware charge", 1);
  histograms.muColl1hwChargeValid = booker.book1D("muHwChargeValidColl1", muonColl1Title_+" mismatching muon charge valid", 2, -0.5, 1.5);
  histograms.muColl1hwChargeValid.setAxisTitle("Hardware charge valid", 1);
  histograms.muColl1hwQual = booker.book1D("muHwQualColl1", muonColl1Title_+" mismatching muon quality", 16, -0.5, 15.5);
  histograms.muColl1hwQual.setAxisTitle("Hardware quality", 1);
  histograms.muColl1hwIso = booker.book1D("muHwIsoColl1", muonColl1Title_+" mismatching muon isolation", 4, -0.5, 3.5);
  histograms.muColl1hwIso.setAxisTitle("Hardware isolation", 1);
  histograms.muColl1Index = booker.book1D("muIndexColl1", muonColl1Title_+" mismatching Input muon index", 108, -0.5, 107.5);
  histograms.muColl1Index.setAxisTitle("Index", 1);

  histograms.muColl2BxRange = booker.book1D("muBxRangeColl2", muonColl2Title_+" mismatching BX range", 5, -2.5, 2.5);
  histograms.muColl2BxRange.setAxisTitle("BX range", 1);
  histograms.muColl2nMu = booker.book1D("nMuColl2", muonColl2Title_+" mismatching muon multiplicity", 9, -0.5, 8.5);
  histograms.muColl2nMu.setAxisTitle("Muon multiplicity", 1);
  histograms.muColl2hwPt = booker.book1D("muHwPtColl2", muonColl2Title_+" mismatching muon p_{T}", 512, -0.5, 511.5);
  histograms.muColl2hwPt.setAxisTitle("Hardware p_{T}", 1);
  histograms.muColl2hwEta = booker.book1D("muHwEtaColl2", muonColl2Title_+" mismatching muon #eta", 461, -230.5, 230.5);
  histograms.muColl2hwEta.setAxisTitle("Hardware #eta", 1);
  histograms.muColl2hwPhi = booker.book1D("muHwPhiColl2", muonColl2Title_+" mismatching muon #phi", 576, -0.5, 575.5);
  histograms.muColl2hwPhi.setAxisTitle("Hardware #phi", 1);
  histograms.muColl2hwEtaAtVtx = booker.book1D("muHwEtaAtVtxColl2", muonColl2Title_+" mismatching muon #eta at vertex", 461, -230.5, 230.5);
  histograms.muColl2hwEtaAtVtx.setAxisTitle("Hardware #eta at vertex", 1);
  histograms.muColl2hwPhiAtVtx = booker.book1D("muHwPhiAtVtxColl2", muonColl2Title_+" mismatching muon #phi at vertex", 576, -0.5, 575.5);
  histograms.muColl2hwPhiAtVtx.setAxisTitle("Hardware #phi at vertex", 1);
  histograms.muColl2hwCharge = booker.book1D("muHwChargeColl2", muonColl2Title_+" mismatching muon charge", 2, -0.5, 1.5);
  histograms.muColl2hwCharge.setAxisTitle("Hardware charge", 1);
  histograms.muColl2hwChargeValid = booker.book1D("muHwChargeValidColl2", muonColl2Title_+" mismatching muon charge valid", 2, -0.5, 1.5);
  histograms.muColl2hwChargeValid.setAxisTitle("Hardware charge valid", 1);
  histograms.muColl2hwQual = booker.book1D("muHwQualColl2", muonColl2Title_+" mismatching muon quality", 16, -0.5, 15.5);
  histograms.muColl2hwQual.setAxisTitle("Hardware quality", 1);
  histograms.muColl2hwIso = booker.book1D("muHwIsoColl2", muonColl2Title_+" mismatching muon isolation", 4, -0.5, 3.5);
  histograms.muColl2hwIso.setAxisTitle("Hardware isolation", 1);
  histograms.muColl2Index = booker.book1D("muIndexColl2", muonColl2Title_+" mismatching Input muon index", 108, -0.5, 107.5);
  histograms.muColl2Index.setAxisTitle("Index", 1);
}

void L1TStage2MuonCompDqm::dqmAnalyze(const edm::Event& e, const edm::EventSetup& c, muoncompdqm::Histograms const& histograms) const
{
  edm::Handle<BXVector<edm::Ref<l1t::MuonBxCollection>>> coll1ExcessMuons;
  edm::Handle<BXVector<edm::Ref<l1t::MuonBxCollection>>> coll2ExcessMuons;
  e.getByToken(coll1ExcessToken_, coll1ExcessMuons);
  e.getByToken(coll2ExcessToken_, coll2ExcessMuons);

  edm::Handle<BXVector<std::pair<edm::Ref<l1t::MuonBxCollection>, edm::Ref<l1t::MuonBxCollection>>>> matchMuonBxColl;
  edm::Handle<BXVector<std::pair<edm::Ref<l1t::MuonBxCollection>, edm::Ref<l1t::MuonBxCollection>>>> mismatchMuonBxColl;
  e.getByToken(matchToken_, matchMuonBxColl);
  e.getByToken(mismatchToken_, mismatchMuonBxColl);

  // check BX ranges
  histograms.errorSummaryDen.fill(RBXRANGE);
  int bxRange1 = coll1ExcessMuons->getLastBX() - coll1ExcessMuons->getFirstBX() + 1;
  int bxRange2 = coll2ExcessMuons->getLastBX() - coll2ExcessMuons->getFirstBX() + 1;
  if (bxRange1 != bxRange2) {
    histograms.summary.fill(BXRANGEBAD);
    if (incBin_[RBXRANGE]) histograms.errorSummaryNum.fill(RBXRANGE);
    int bx;
    for (bx = coll1ExcessMuons->getFirstBX(); bx <= coll1ExcessMuons->getLastBX(); ++bx) {
        histograms.muColl1BxRange.fill(bx);
    }
    for (bx = coll2ExcessMuons->getFirstBX(); bx <= coll2ExcessMuons->getLastBX(); ++bx) {
        histograms.muColl2BxRange.fill(bx);
    }
  } else {
    histograms.summary.fill(BXRANGEGOOD);
  }

  for (int iBx = matchMuonBxColl->getFirstBX(); iBx <= matchMuonBxColl->getLastBX(); ++iBx) {
    const auto nMatchingMuons = matchMuonBxColl->size(iBx);
    const auto nMismatchingMuons = mismatchMuonBxColl->size(iBx);
    const auto nMuonTotal = nMatchingMuons + nMismatchingMuons;

    histograms.errorSummaryDen.fill(RNMUON);

    // check if there are excess muons in one collection
    if (coll1ExcessMuons->size(iBx) > 0 || coll2ExcessMuons->size(iBx) > 0) {
      histograms.summary.fill(NMUONBAD);
      if (incBin_[RNMUON]) histograms.errorSummaryNum.fill(RNMUON);
      histograms.muColl1nMu.fill(nMuonTotal + coll1ExcessMuons->size(iBx));
      histograms.muColl2nMu.fill(nMuonTotal + coll2ExcessMuons->size(iBx));

      BXVector<edm::Ref<l1t::MuonBxCollection>>::const_iterator it = coll1ExcessMuons->begin(iBx);
      for (; it != coll1ExcessMuons->end(iBx); ++it) {
        histograms.muColl1hwPt.fill((*it)->hwPt());
        histograms.muColl1hwEta.fill((*it)->hwEta());
        histograms.muColl1hwPhi.fill((*it)->hwPhi());
        histograms.muColl1hwEtaAtVtx.fill((*it)->hwEtaAtVtx());
        histograms.muColl1hwPhiAtVtx.fill((*it)->hwPhiAtVtx());
        histograms.muColl1hwCharge.fill((*it)->hwCharge());
        histograms.muColl1hwChargeValid.fill((*it)->hwChargeValid());
        histograms.muColl1hwQual.fill((*it)->hwQual());
        histograms.muColl1hwIso.fill((*it)->hwIso());
        histograms.muColl1Index.fill((*it)->tfMuonIndex());
      }

      for (it = coll2ExcessMuons->begin(iBx); it != coll2ExcessMuons->end(iBx); ++it) {
        histograms.muColl2hwPt.fill((*it)->hwPt());
        histograms.muColl2hwEta.fill((*it)->hwEta());
        histograms.muColl2hwPhi.fill((*it)->hwPhi());
        histograms.muColl2hwEtaAtVtx.fill((*it)->hwEtaAtVtx());
        histograms.muColl2hwPhiAtVtx.fill((*it)->hwPhiAtVtx());
        histograms.muColl2hwCharge.fill((*it)->hwCharge());
        histograms.muColl2hwChargeValid.fill((*it)->hwChargeValid());
        histograms.muColl2hwQual.fill((*it)->hwQual());
        histograms.muColl2hwIso.fill((*it)->hwIso());
        histograms.muColl2Index.fill((*it)->tfMuonIndex());
      }
    } else {
      histograms.summary.fill(NMUONGOOD);
    }

    // Fill bins for all muon pairs
    for (size_t i = 0; i < nMuonTotal; ++i) {
      histograms.summary.fill(MUONALL);
      for (int j = RMUON; j <= RIDX; ++j) {
        histograms.errorSummaryDen.fill(j);
      }
    }
    // Fill bin for matching muon pairs
    for (size_t i = 0; i < nMatchingMuons; ++i) {
      histograms.summary.fill(MUONGOOD);
    }

    // Check variables of mismatching muons pairs and fill histograms
    BXVector<std::pair<edm::Ref<l1t::MuonBxCollection>, edm::Ref<l1t::MuonBxCollection>>>::const_iterator it = mismatchMuonBxColl->begin(iBx);
    for (; it != mismatchMuonBxColl->end(iBx); ++it) {
      auto& ref1 = it->first;
      auto& ref2 = it->second;

      bool muonSelMismatch = false; // Muon mismatches excluding ignored bins
      if (ref1->hwPt() != ref2->hwPt()) {
        histograms.summary.fill(PTBAD);
        if (incBin_[RPT]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RPT);
        }
      }
      if (ref1->hwEta() != ref2->hwEta()) {
        histograms.summary.fill(ETABAD);
        if (incBin_[RETA]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RETA);
        }
      }
      if (ref1->hwPhi() != ref2->hwPhi()) {
        histograms.summary.fill(PHIBAD);
        if (incBin_[RPHI]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RPHI);
        }
      }
      if (ref1->hwEtaAtVtx() != ref2->hwEtaAtVtx()) {
        histograms.summary.fill(ETAATVTXBAD);
        if (incBin_[RETAATVTX]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RETAATVTX);
        }
      }
      if (ref1->hwPhiAtVtx() != ref2->hwPhiAtVtx()) {
        histograms.summary.fill(PHIATVTXBAD);
        if (incBin_[RPHIATVTX]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RPHIATVTX);
        }
      }
      if (ref1->hwCharge() != ref2->hwCharge()) {
        histograms.summary.fill(CHARGEBAD);
        if (incBin_[RCHARGE]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RCHARGE);
        }
      }
      if (ref1->hwChargeValid() != ref2->hwChargeValid()) {
        histograms.summary.fill(CHARGEVALBAD);
        if (incBin_[RCHARGEVAL]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RCHARGEVAL);
        }
      }
      if (ref1->hwQual() != ref2->hwQual()) {
        histograms.summary.fill(QUALBAD);
        if (incBin_[RQUAL]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RQUAL);
        }
      }
      if (ref1->hwIso() != ref2->hwIso()) {
        histograms.summary.fill(ISOBAD);
        if (incBin_[RISO]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RISO);
        }
      }
      if (ref1->tfMuonIndex() != ref2->tfMuonIndex()) {
        histograms.summary.fill(IDXBAD);
        if (incBin_[RIDX]) {
          muonSelMismatch = true;
          histograms.errorSummaryNum.fill(RIDX);
        }
      }

      if (incBin_[RMUON] && muonSelMismatch) {
        histograms.errorSummaryNum.fill(RMUON);
      }

      histograms.muColl1hwPt.fill(ref1->hwPt());
      histograms.muColl1hwEta.fill(ref1->hwEta());
      histograms.muColl1hwPhi.fill(ref1->hwPhi());
      histograms.muColl1hwEtaAtVtx.fill(ref1->hwEtaAtVtx());
      histograms.muColl1hwPhiAtVtx.fill(ref1->hwPhiAtVtx());
      histograms.muColl1hwCharge.fill(ref1->hwCharge());
      histograms.muColl1hwChargeValid.fill(ref1->hwChargeValid());
      histograms.muColl1hwQual.fill(ref1->hwQual());
      histograms.muColl1hwIso.fill(ref1->hwIso());
      histograms.muColl1Index.fill(ref1->tfMuonIndex());

      histograms.muColl2hwPt.fill(ref2->hwPt());
      histograms.muColl2hwEta.fill(ref2->hwEta());
      histograms.muColl2hwPhi.fill(ref2->hwPhi());
      histograms.muColl2hwEtaAtVtx.fill(ref2->hwEtaAtVtx());
      histograms.muColl2hwPhiAtVtx.fill(ref2->hwPhiAtVtx());
      histograms.muColl2hwCharge.fill(ref2->hwCharge());
      histograms.muColl2hwChargeValid.fill(ref2->hwChargeValid());
      histograms.muColl2hwQual.fill(ref2->hwQual());
      histograms.muColl2hwIso.fill(ref2->hwIso());
      histograms.muColl2Index.fill(ref2->tfMuonIndex());
    }
  }
}

