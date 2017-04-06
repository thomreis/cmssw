import ROOT as root
from array import array
from math import sqrt


class HistManager(object):
    """Class that manages and holds histograms"""
    def __init__(self, varnames=[], binning_dict={}, ytitle="# Muons", prefix="", filename=None):
        super(HistManager, self).__init__()
        self.varnames = varnames
        self.binnings = binning_dict
        self.prefix = prefix
        root.TGaxis().SetMaxDigits(3)

        self.hists = {}

        self._stackcache = {}
        self._effcache = {}
        self._ratiocache = {}
        self._thresholdcache = {}

        if filename is None:
            for vname in varnames:
                have_unit = type(self.binnings[vname][-2]) is str
                # variable binning when nBins == -1
                if self.binnings[vname][0] < 0:
                    if have_unit:
                        self.hists[vname] = root.TH1D(prefix+vname, "", len(self.binnings[vname])-4, array('d', self.binnings[vname][1:-2]))
                    else:
                        self.hists[vname] = root.TH1D(prefix+vname, "", len(self.binnings[vname])-3, array('d', self.binnings[vname][1:-1]))
                # fixed binning
                else:
                    self.hists[vname] = root.TH1D(prefix+vname, "", self.binnings[vname][0], self.binnings[vname][1], self.binnings[vname][2])
                self.hists[vname].Sumw2()
                if not have_unit:
                    xtitle = self.binnings[vname][-1]
                elif self.binnings[vname][-1] is None:
                    xtitle = self.binnings[vname][-2]
                else:
                    xtitle = "{title} ({unit})".format(title=self.binnings[vname][-2], unit=self.binnings[vname][-1])

                self.hists[vname].GetXaxis().SetTitle(xtitle)
                self.hists[vname].GetYaxis().SetTitle(ytitle)
        else:
            self.varnames = []
            input = root.TFile(filename)
            directory = input.GetDirectory('muonAnalyzer')
            keyList = directory.GetListOfKeys()
            for key in keyList:
                hName = key.GetName()
                self.varnames.append(hName)
                self.hists[hName] = directory.Get(hName)
                self.hists[hName].SetDirectory(0)
            input.Close()

    def fill(self, varname, val):
        self.hists[varname].Fill(val)

    def get(self, varname):
        return self.hists[varname]

    def get_varnames(self):
        return self.varnames

    def get_stack(self, varnames):
        keyname = "_".join(varnames)
        if keyname in self._stackcache.keys():
            return self._stackcache[keyname]
        stack = root.THStack()
        hs = []
        for vname in varnames:
            h = self.get(vname)
            stack.Add(h)
            hs.append(h)
        self._stackcache[keyname] = [hs, stack]
        return [hs, stack]

    def get_binning(self, varname):
        return self.binnings[varname]

    def get_threshold_hist(self, varname):
        if varname in self._thresholdcache.keys():
            return self._thresholdcache[varname]

        h_thr = self.hists[varname].Clone()
        h = self.hists[varname]
        bins = range(h_thr.GetNbinsX()+2)
        bins.reverse()
        bmax = h_thr.GetNbinsX()
        binsum = 0
        binerr2 = 0
        for ib in bins:
            binsum += h.GetBinContent(ib)
            binerr2 += h.GetBinError(ib)**2
            if ib > 0 and ib < bmax:
                h_thr.SetBinContent(ib, binsum)
                h_thr.SetBinError(ib, sqrt(binerr2))
        h_thr.GetYaxis().SetTitle("Integrated "+h_thr.GetYaxis().GetTitle())
        self._thresholdcache[varname] = h_thr
        return h_thr

    def get_threshold_stack(self, varnames):
        keyname = "thr_".join(varnames)
        if keyname in self._stackcache.keys():
            return self._stackcache[keyname]
        stack = root.THStack()
        hs = []
        for vname in varnames:
            h = self.get_threshold_hist(vname)
            stack.Add(h)
            hs.append(h)
        self._stackcache[keyname] = [hs, stack]
        return [hs, stack]

    def get_diff(self, varname_h1, varname_h2):
        name = "{h1}_-_{h2}".format(h1=varname_h1, h2=varname_h2)
        if name in self._ratiocache.keys():
            return self._ratiocache[name]

        h2 = self.hists[varname_h2]
        h_diff = self.hists[varname_h1].Clone(name)
        h_diff.Sumw2()
        h_diff.Add(h2, -1.)

        self._ratiocache[name] = h_diff
        return h_diff

    def get_ratio(self, varname_nom, varname_denom):
        name = "{nom}_o_{denom}".format(nom=varname_nom, denom=varname_denom)
        if name in self._ratiocache.keys():
            return self._ratiocache[name]

        h_denom = self.hists[varname_denom]
        h_denom.Sumw2()
        h_ratio = self.hists[varname_nom].Clone()
        h_ratio.Sumw2()
        h_ratio.Divide(h_ratio, h_denom, 1, 1, "b")

        self._ratiocache[name] = h_ratio
        return h_ratio

    def get_ratio_stack(self, varnames_nom, varname_denom):
        name = "{nom}_o_{denom}".format(nom="_".join(varnames_nom), denom=varname_denom)
        if name in self._stackcache.keys():
            return self._stackcache[name]

        stack = root.THStack()
        hs = []
        for vname_nom in varnames_nom:
            h_ratio = self.get_ratio(vname_nom, varname_denom)
            hs.append(h_ratio)
            stack.Add(h_ratio)

        self._stackcache[name] = [hs, stack]
        return [hs, stack]

    def get_efficiency(self, varname_nom, varname_denom):
        name = "{nom}_o_{denom}".format(nom=varname_nom, denom=varname_denom)
        if name in self._effcache.keys():
            return self._effcache[name]

        h_denom = self.hists[varname_denom]
        h_nom = self.hists[varname_nom]

        eff = root.TEfficiency(h_nom, h_denom)
        self._effcache[name] = eff
        return eff


class L1AnalysisHistManager(HistManager):
    """Class that manages and holds histograms"""
    def __init__(self, varnames, binning_dict, prefix=""):
        super(L1AnalysisHistManager, self).__init__(varnames, binning_dict, prefix=prefix)

    def fill(self, varname, val):
        self.hists[varname].Fill(val)


class VarExp(object):
    def __init__(self, name, varexp, cutexp):
        super(VarExp, self).__init__()
        self.name = name
        self.varexp = varexp
        self.cutexp = cutexp


class FlatHistManager(HistManager):
    def __init__(self, varnames, binning_dict, varexps, cutexp, prefix=""):
        super(FlatHistManager, self).__init__(varnames, binning_dict, prefix=prefix)
        self.varexps = varexps
        self.cutexp = cutexp

    def project(self, ntuple):
        for vname in self.varnames:
            ntuple.Project(self.prefix+vname, self.varexps[vname].varexp, "({cut1})*({cut2})".format(cut1=self.varexps[vname].cutexp, cut2=self.cutexp))
