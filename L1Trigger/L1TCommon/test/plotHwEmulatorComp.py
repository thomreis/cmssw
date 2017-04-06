#!/usr/bin/env python
from ToolBox import parse_options_and_init_log
# have to do this first or ROOT masks the -h messages
opts, parser = parse_options_and_init_log()

from plotting import HistManager
import ROOT as root

def parse_options_plotRates(parser):
    """
    Adds often used options to the OptionParser...
    """
    parsers = parser.add_subparsers()
    sub_parser = parsers.add_parser("plot")
    sub_parser.add_argument("-i", "--interactive", dest="interactive", action='store_false', help="Draw plots on screen.")
    sub_parser.add_argument("-d", "--diff", dest="diff", action='store_true', help="Plot difference instead of ratio.")

    opts, unknown = parser.parse_known_args()
    return opts

def set_root_style():
    root.gStyle.SetTitleFont(font)
    root.gStyle.SetStatFont(font)
    root.gStyle.SetTextFont(font)
    root.gStyle.SetLabelFont(font)
    root.gStyle.SetLegendFont(font)
    root.gStyle.SetMarkerStyle(20)
    root.gStyle.SetOptStat(0)
    root.gStyle.SetOptFit(0)
    root.gStyle.SetOptTitle(0)

def plotComp(hm, var, hwTag='UnpackerBX0', emulTag='EmulatorBX0', data=True, compMode='ratio'):
    ratioPadRelSize = 0.3
    histoPadFontSizeFactor = 1 / (1. - ratioPadRelSize)
    ratioPadFontSizeFactor = 1 / ratioPadRelSize
    textFontSize = 0.04
    axisLabelSize = 0.035
    axisTitleSize = 0.04
    leftMargin = 0.12
    rightMargin = 0.06

    legend = root.TLegend(0.7, 0.75, 0.92, 0.87)
    legend.SetTextFont(font)
    legend.SetTextSize(textFontSize * histoPadFontSizeFactor)
    legend.SetBorderSize(0)
    legend.SetFillColor(19)
    legend.SetFillStyle(0)

    hNameHw = var + hwTag
    hNameEmul = var + emulTag

    hHw = hm.get(hNameHw)
    hEmul = hm.get(hNameEmul)

    hHw.SetLineColor(root.kRed)
    hHw.SetFillColor(root.kRed-7)
    hHw.GetXaxis().SetLabelSize(axisLabelSize * histoPadFontSizeFactor)
    hHw.GetYaxis().SetLabelSize(axisLabelSize * histoPadFontSizeFactor)
    hHw.GetXaxis().SetTitleSize(axisTitleSize * histoPadFontSizeFactor)
    hHw.GetYaxis().SetTitleSize(axisTitleSize * histoPadFontSizeFactor)
    hHw.GetYaxis().SetTitleOffset(0.1)

    hEmul.SetLineColor(root.kBlack)
    hEmul.SetMarkerStyle(root.kFullCircle)

    legends = []

    canvas_name = 'c_{var}_{hwTag}_vs_{emulTag}'.format(var=var, hwTag=hwTag, emulTag=emulTag) 

    # create canvas and draw on it
    canvas_title = 'HW vs. emulator ' + var + ' - ' + hwTag.replace('Unpacker', '')
    c = root.TCanvas(canvas_name, canvas_title, 100, 100, 600, 600)
    p1 = root.TPad(canvas_name + '_p1', canvas_title + '_p1', 0., ratioPadRelSize, 1., 1.)
    p2 = root.TPad(canvas_name + '_p2', canvas_title + '_p2', 0., 0., 1., ratioPadRelSize)
    p1.Draw()
    p2.Draw()

    # Pad 1: histograms
    p1.cd()
    p1.SetBottomMargin(0.)
    p1.SetLeftMargin(leftMargin)
    p1.SetRightMargin(rightMargin)
    p1.SetTicks()

    if var[1:3] == 'Pt' or var[1:4] == 'Eta' or var[1:4] == 'Phi' or var[3:7] == 'Size' or var[3:7] == 'Qual':
        p1.SetLogy(True)

    set_root_style()

    hHw.Draw('hist')
    hEmul.Draw('pe0same')
    hHw.Draw('sameaxis')

    leftInt = hHw.Integral(1, int(hHw.GetNbinsX() / 2.))
    rightInt = hHw.Integral(int(hHw.GetNbinsX() / 2. + 1), hHw.GetNbinsX())
    if rightInt > leftInt:
      legendXMin = 0.15
      legendXMax = 0.37
    else:
      legendXMin = 0.7
      legendXMax = 0.92

    legends.append(root.TLegend(legendXMin, 0.75, legendXMax, 0.87))
    legends[0].SetTextFont(font)
    legends[0].SetTextSize(textFontSize * histoPadFontSizeFactor)
    legends[0].SetBorderSize(0)
    legends[0].SetFillColor(19)
    legends[0].SetFillStyle(0)

    legends[0].AddEntry(hHw, 'HW')
    legends[0].AddEntry(hEmul, 'Emulator', 'lep')
    legends[0].Draw('same')

    tex = root.TLatex()
    tex.SetNDC()
    tex.SetTextFont(font)
    tex.SetTextSize(textFontSize * histoPadFontSizeFactor)
    tex.DrawLatex(0.2, 0.91, var)
    if data:
        tex.DrawLatex(0.73, 0.91, 'CMS internal')
    else:
        tex.DrawLatex(0.59, 0.91, 'CMS simulation, 13 TeV')

    # Pad 2: ratio
    p2.cd()
    p2.SetTopMargin(0.)
    p2.SetBottomMargin(0.3)
    p2.SetLeftMargin(leftMargin)
    p2.SetRightMargin(rightMargin)
    p2.SetTicks()

    legends.append(root.TLegend(0.63, 0.82, 0.85, 0.94))
    legends[1].SetTextFont(font)
    legends[1].SetTextSize(textFontSize * ratioPadFontSizeFactor)
    legends[1].SetBorderSize(0)
    legends[1].SetFillColor(19)
    legends[1].SetFillStyle(0)

    if compMode == 'ratio':
        hComp = hm.get_ratio(hNameEmul, hNameHw)
        hComp.GetYaxis().SetRangeUser(0.91, 1.09)
        hComp.GetYaxis().SetTitle('ratio ')
        hDrawOpt = 'e0'
        legends[1].AddEntry(hComp, 'Emulator / HW')
        eqLineY = 1.
    else:
        hComp = hm.get_diff(hNameHw, hNameEmul)
        #hComp.GetYaxis().SetRangeUser(-1.2, 1.2)
        hComp.GetYaxis().SetTitle('HW - Emul ')
        hDrawOpt = 'hist'
        legends[1].AddEntry(hComp, 'HW - Emulator')
        eqLineY = 0.
    hComp.SetLineColor(root.kRed)
    hComp.GetXaxis().SetLabelSize(axisLabelSize * ratioPadFontSizeFactor)
    hComp.GetYaxis().SetLabelSize(axisLabelSize * ratioPadFontSizeFactor)
    hComp.GetXaxis().SetTitleSize(axisTitleSize * ratioPadFontSizeFactor)
    hComp.GetYaxis().SetTitleSize(axisTitleSize * ratioPadFontSizeFactor)
    hComp.GetYaxis().SetTitleOffset(0.44)
    hComp.GetYaxis().SetNdivisions(205)
    hComp.Draw(hDrawOpt)
    hComp.Draw('sameaxis')

    legends[1].Draw('same')

    line = root.TLine(hComp.GetXaxis().GetBinLowEdge(1), eqLineY, hComp.GetXaxis().GetBinLowEdge(hComp.GetNbinsX()+1), eqLineY)
    line.SetLineStyle(root.kDotted)
    line.Draw('same')

    c.Modified()
    c.Update()

    return [c, p1, p2, hHw, hEmul, hComp, legends, line, tex]

def main():
    opts = parse_options_plotRates(parser)
    batchRun = opts.interactive
    if batchRun:
        root.gROOT.SetBatch(True)

    hm = HistManager(filename=opts.fname)

    compMode = 'ratio'
    if opts.diff:
        compMode = 'diff'

    # holds the canvases, histograms, etc.
    objects = []

    # BX0 events
    objects.append(plotComp(hm, var='hPt', compMode=compMode))
    objects.append(plotComp(hm, var='hEta', compMode=compMode))
    objects.append(plotComp(hm, var='hPhi', compMode=compMode))
    objects.append(plotComp(hm, var='hHwCharge', compMode=compMode))
    objects.append(plotComp(hm, var='hHwChargeValid', compMode=compMode))
    objects.append(plotComp(hm, var='hHwQual', compMode=compMode))
    objects.append(plotComp(hm, var='hHwIso', compMode=compMode))
    objects.append(plotComp(hm, var='hMuSize', compMode=compMode))
    objects.append(plotComp(hm, var='hMuPerBx', compMode=compMode))
    objects.append(plotComp(hm, var='hMuPerBxId', compMode=compMode))

    # Validation events
    objects.append(plotComp(hm, var='hPt', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hEta', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hPhi', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hHwCharge', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hHwChargeValid', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hHwQual', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hHwIso', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hMuSize', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hMuPerBx', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))
    objects.append(plotComp(hm, var='hMuPerBxId', hwTag='UnpackerValEvt', emulTag='EmulatorValEvt', compMode=compMode))

    ##########################################################################
    # save plots to root file
    if savePlots:
        output = root.TFile('./ugmtFwEmulatorComp_plots.root', 'recreate')
        output.cd()
        for obj in objects:
            c = obj[0]
            c.Write(c.GetName())
            c.Print('./plots/'+c.GetName()+'.pdf', '.pdf')
            c.Print('./plots/'+c.GetName()+'.png', '.png')
        output.Close()

    # wait
    if not batchRun:
        raw_input("Press ENTER to quit.")

if __name__ == "__main__":
    savePlots = True
    batchRun = True
    font = 42
    main()

