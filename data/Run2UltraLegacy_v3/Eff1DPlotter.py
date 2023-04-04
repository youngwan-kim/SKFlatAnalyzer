import os, sys, argparse
from ROOT import TCanvas, TPad, TFile, TPaveLabel, TPaveText, TLatex, TLegend, TH1F, kRed, kGreen,kBlue, THStack, gROOT, TRatioPlot, kMagenta
gROOT.SetBatch(True)

d_eff = {
    "Efficiencies_muon_generalTracks_Z_Run2017_UL_ID" : [["NUM_TightID_DEN_TrackerMuons_abseta_pt"],6,3,0.95,1.02],
    "Efficiencies_muon_generalTracks_Z_Run2017_UL_ISO" : [["NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt"],6,3,0.98,1.04],
    "Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers" : [["NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt_efficiencyData","NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt_efficiencyMC"],5,3,0.65,1.15]
}

pTbinname = ["eta1","0#leq|#eta|<0.9","0.9#leq|#eta|<1.2","1.2#leq|#eta|<2.1","2.1#leq|#eta|<2.4"]
colorbin = [600,629,419,kMagenta,600]

for era in ["2017"] :
    for filename in d_eff :
        savename = filename.rsplit("_",1)[1]
        rootfile = TFile(f"{era}/ID/Muon/{filename}.root")
        for branchname in d_eff[filename][0] :
            c_eta = TCanvas("","",1000,1000)
            c_pt = TCanvas("","",1000,1000)

            legend = TLegend(0.67,0.87-4*0.06,0.87,0.87)
            legend.SetTextSize(0.03)
            legend.SetBorderSize(0)
            #legend.SetTextFont(62)
            legend.SetLineColor(0)
            legend.SetLineStyle(1)
            legend.SetLineWidth(1)

            #histo = rootfile.Get(f"{branchname}")
            for ybin in range(0,d_eff[filename][1]+2) :
                histo = rootfile.Get(f"{branchname}")
                histo_projX = histo.ProjectionX(f"{era}_{branchname}_Eta{ybin}",ybin,ybin,'e')
                histo_projX.SetStats(0)
                histo_projX.GetYaxis().SetRangeUser(d_eff[filename][3],d_eff[filename][4])
                histo_projX.SetLineColor(kBlue+ybin)
                c_eta.cd()
                if ybin == 0 : histo_projX.Draw("e1&hist")
                else : histo_projX.Draw("e1&hist&same")
            for xbin in range(1,d_eff[filename][2]+2) :
                histo = rootfile.Get(f"{branchname}")
                histo_projY = histo.ProjectionY(f"{era}_{branchname}_Pt{xbin}",xbin,xbin,'e')
                histo_projY.SetStats(0)
                histo_projY.SetTitle("")
                histo_projY.GetYaxis().SetRangeUser(d_eff[filename][3],d_eff[filename][4])
                histo_projY.SetLineColor(colorbin[xbin])
                histo_projY.SetMarkerStyle(1)
                histo_projY.SetLineWidth(2)
                histo_projY.GetYaxis().SetTitle("Efficiency")
                histo_projY.GetYaxis().SetLabelSize(0.02)
                histo_projY.GetXaxis().SetLabelSize(0.025)
                histo_projY.GetXaxis().SetTitleSize(0.025)
                legend.AddEntry(histo_projY,pTbinname[xbin])
                c_pt.cd()
                if xbin == 0 : histo_projY.Draw("e1&hist")
                else : histo_projY.Draw("e1&hist&same")
            c_pt.cd(); legend.Draw()
            c_eta.SaveAs(f"{era}Eta_{savename}_{branchname}.png")
            c_eta.Close()
            c_pt.SaveAs(f"{era}Pt_{savename}_{branchname}.png")
            c_pt.Close()



