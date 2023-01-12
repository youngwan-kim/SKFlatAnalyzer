from ROOT import TH1D,TFile,TCanvas,TPad,TH1, TLatex, TLegend
from ROOT import kBlue, kRed, kBlack
import math, array

#f = TFile("HNL_ChargeFlip_DYJets.root")
f = TFile("CFval_Prompt.root")
h1 = f.Get("SignalStudyUL/LeadLeptonPt_BinWeight")
h1.SetDirectory(0)
#h1 = TH1D("","",1000,0.,2000.)
h2 = f.Get("SignalStudyUL/LeadLeptonPt_FitWeight")
h2.SetDirectory(0)
h3 = f.Get("SignalStudyUL/LeadLeptonPt_fromSS")
h3.SetDirectory(0)
h = [h1,h2,h3]


#for i in range(1,1001) :
#    if math.isnan(h1.GetBinContent(i)) : h1.Fill(i*2.,0)
#    else : h1.Fill(i*2.,h_temp.GetBinContent(i))

#for i in range(1,1001) : print h1.GetBinContent(i)

#h1 = h_temp
l = TLegend(0.3,0.35,0.825,0.65)
l.SetFillStyle(0)
l.SetBorderSize(0)
#l.SetTextFont(42)
#l.SetTextSize(0.05)

for hist in h :
    print hist
    hist.SetStats(0)
    hist.SetLineWidth(2)
    hist.GetXaxis().SetRangeUser(30.,150.)
    hist.GetXaxis().SetLabelSize(0)
    #print hist

#h1.Rebin(2)
#h2.Rebin(2)
#h3.Rebin(2)

h1.SetLineColor(kRed)
h2.SetLineColor(kBlue)
h3.SetLineColor(kBlack)

l.AddEntry(h1,"( OS2l+m_{ll} #in [75,105] ) #times Bin value CF weight","l")
l.AddEntry(h2,"( OS2l+m_{ll} #in [75,105] ) #times Fit function CF weight","l")
l.AddEntry(h3,"( SS2l+m_{ll} #in [75,105] ) , No Weight","l")

div = h3.Clone("divide")
r1 = h1.Clone("ratio1")
r1.SetMarkerColor(kRed)
r2 = h2.Clone("ratio2")
r2.SetMarkerColor(kBlue)
r1.Divide(div)
r2.Divide(div)
r1_syst = r1.Clone("ratio1_syst")
r2_syst = r2.Clone("ratio2_syst")
r1_syst.SetFillColorAlpha(kRed,0.6)
r2_syst.SetFillColorAlpha(kBlue,0.6)
r1_syst.SetFillStyle(3144)
r2_syst.SetFillStyle(3144)

r = [r1,r2]
for ratio in r :
    ratio.SetStats(0)
    ratio.SetMarkerStyle(8)
    ratio.GetYaxis().SetRangeUser(0,3)
    ratio.GetYaxis().SetLabelSize(0.1)
    ratio.GetYaxis().SetNdivisions(504)
    ratio.GetXaxis().SetLabelSize(0.1)
    ratio.GetXaxis().SetTitle("Leading Lepton p_{T} [GeV]")
    ratio.GetXaxis().SetTitleOffset(1.2)
    ratio.GetXaxis().SetTitleSize(0.1)


c = TCanvas("","",1000,1000)
p1 = TPad("","",0.,0.25,1.,1.)
p1.SetBottomMargin(0.05)
p2 = TPad("","",0.,0.,1.,0.25)
p2.SetTopMargin(0.0025)
p2.SetBottomMargin(0.3)
p2.SetGrid(1)



c.cd()
p1.cd()
h2.Draw("hist")
h1.Draw("hist&same")
h3.Draw("hist&same")
l.Draw()
p2.cd()
r1.Draw("p&hist")
r2.Draw("p&hist&same")
r1_syst.Draw("e2&f&same")
r2_syst.Draw("e2&f&same")
r1.Draw
c.cd()
p1.Draw()
p2.Draw()
c.SaveAs("CFVal_Prompt_CFweight.png")