import os,sys
from ROOT import TClass, TCanvas, TPad, TFile, TPaveLabel, TPaveText, TLatex, TLegend, TH1F, kRed, kGreen, kBlue, gROOT, kOrange
gROOT.SetBatch(True)
from math import sqrt

outdir = "/data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalSampleValidation/"
eras = ["2016preVFP","2016postVFP","2017","2018"]
#eras = ["2017","2018"]
hists = ["CutFlow","Gen/lep0_eta","Gen/lep0_pt","Gen/lep1_eta","Gen/lep1_pt",
        "Reco/METv_Pt","Reco/Nbjets","Reco/Njets","Reco/lep0_eta","Reco/lep0_pt",
        "Reco/lep1_eta","Reco/lep1_pt","Reco/jet0_eta","Reco/jet0_pt","Reco/fatjet0_eta","Reco/fatjet0_pt"]
d_hists = {
    "CutFlow" : "CutFlow",
    "Gen/lep0_eta" : "genlep0_eta",
    "Gen/lep0_pt" : "genlep0_pt",
    "Gen/lep1_eta" : "genlep1_eta",
    "Gen/lep1_pt" : "genlep1_pt",
    "Reco/METv_Pt" : "MET",
    "Reco/Nbjets": "nbjets",
    "Reco/Njets" : "njets",
    "Reco/lep0_eta" : "recolep0_eta",
    "Reco/lep0_pt" : "recolep0_pt",
    "Reco/lep1_eta" : "recolep1_eta",
    "Reco/lep1_pt" : "recolep1_pt",
    "Reco/jet0_eta" : "recojet0_eta",
    "Reco/jet0_pt" : "recojet0_pt",
    "Reco/fatjet0_eta" : "recofatjet0_eta",
    "Reco/fatjet0_pt" : "recofatjet0_pt",
}
#centralsamplelist = ["HNL_SignalSampleValidation_DYTypeI_DF_ll_M85","HNL_SignalSampleValidation_DYTypeI_DF_ll_M95"]#,"HNL_SignalSampleValidation_DYTypeI_DF_ll_M85","HNL_SignalSampleValidation_DYTypeI_DF_ll_M85"] #,"HNL_SignalSampleValidation_DYTypeI_DF_ll_M200"]
centralsamplelist = ["HNL_SignalSampleValidation_VBFTypeI_DF_ll_M500",
                     "HNL_SignalSampleValidation_VBFTypeI_DF_ll_M700",
                     "HNL_SignalSampleValidation_VBFTypeI_DF_ll_M1000",
                     "HNL_SignalSampleValidation_VBFTypeI_DF_ll_M1500",
                     "HNL_SignalSampleValidation_VBFTypeI_DF_ll_M2000",
                     "HNL_SignalSampleValidation_VBFTypeI_DF_ll_M3000"]
eff = open("Efficiency.txt",'w')

def GetRebinNum(name) :
    if "pt" in name : return 10
    elif "Pt" in name : return 10
    elif "eta" in name : return 2
    else : return 1

for era in eras :
    print "Running {}".format(era)
#    for rootfile in os.listdir(outdir+era) :
#        rootfilename = rootfile.split(".")[0]
#	print rootfilename
#        if "SSWW" in rootfilename : break
#        if not rootfilename.endswith("private") : centralsamplelist.append(rootfilename)
#    print centralsamplelist
    
    for centralsample in centralsamplelist :
        print "[{}] Sample {}".format(era,centralsample)
        masspoint = centralsample.rsplit("_",1)[1]
        privatesample = centralsample.rsplit("_",2)[0]+"_"+masspoint+"_private"

        centralfile = TFile(outdir+era+"/"+centralsample+".root")
        privatefile = TFile(outdir+era+"/"+privatesample+".root")

        #norm = centralfile.Get("CutFlow").GetBinContent(1)/privatefile.Get("CutFlow").GetBinContent(1)
        centralsamplename = centralsample.split("_",2)[2]
        os.system("mkdir -p SampleValidation/{}/{}".format(centralsamplename,era))

        for hist in hists : 
            print  "[{},{}] Histo {}".format(era,centralsample,hist)
            #if hist == "CutFlow" :
            #    eff.write("[{}] {} \n".format(era,centralsamplename))
		    #    centralratioerr = ( centralfile.Get(hist).GetBinContent(2)/centralfile.Get(hist).GetBinContent(1) )*sqrt(pow(centralfile.Get(hist).GetBinError(2)/centralfile.Get(hist).GetBinContent(2),2)+pow(centralfile.Get(hist).GetBinError(1)/centralfile.Get(hist).GetBinContent(1),2))
		    #    privateratioerr = ( privatefile.Get(hist).GetBinContent(2)/privatefile.Get(hist).GetBinContent(1) )*sqrt(pow(privatefile.Get(hist).GetBinError(2)/privatefile.Get(hist).GetBinContent(2),2)+pow(privatefile.Get(hist).GetBinError(1)/privatefile.Get(hist).GetBinContent(1),2))
            #    eff.write("Central sample efficiency : {}+-{}/{}+-{} = {}+-{} ({}%) \n".format(str(centralfile.Get(hist).GetBinContent(2)),str(centralfile.Get(hist).GetBinError(2)),str(centralfile.Get(hist).GetBinContent(1)),str(centralfile.Get(hist).GetBinError(1)),str(centralfile.Get(hist).GetBinContent(2)/centralfile.Get(hist).GetBinContent(1)),centralratioerr,str(100*centralfile.Get(hist).GetBinContent(2)/centralfile.Get(hist).GetBinContent(1))))
		    #    eff.write("Private sample efficiency : {}+-{}/{}+-{} = {}+-{} ({}%) \n".format(str(privatefile.Get(hist).GetBinContent(2)),str(privatefile.Get(hist).GetBinError(2)),str(privatefile.Get(hist).GetBinContent(1)),str(privatefile.Get(hist).GetBinError(1)),str(privatefile.Get(hist).GetBinContent(2)/privatefile.Get(hist).GetBinContent(1)),privateratioerr,str(100*privatefile.Get(hist).GetBinContent(2)/privatefile.Get(hist).GetBinContent(1))))               
            #    eff.write("========== \n")
            #    #continue 
            centralhist = centralfile.Get(hist)
            privatehist = privatefile.Get(hist)
            if centralhist == None or privatehist == None : break
            centralhist.Rebin(GetRebinNum(hist)) ; privatehist.Rebin(GetRebinNum(hist))
            #norm = privatehist.GetBinContent(1)/centralhist.GetBinContent(1)
            norm = privatehist.Integral()/centralhist.Integral()
            centralhist *= norm
            centralhist.SetStats(0) ; privatehist.SetStats(0) 
            #name = hist.split("/")[1]
            maximum = max(centralhist.GetMaximum(),privatehist.GetMaximum())
            centralhist.GetYaxis().SetRangeUser(0.1,maximum*1.8)
            privatehist.GetYaxis().SetRangeUser(0.1,maximum*1.8)

            c = TCanvas(hist,hist,900,1200)
            p2 = TPad("","",0,0,1,0.3)
            p2.SetBottomMargin(0.2)
            p2.SetGrid(1)
            p1 = TPad("","",0,0.3,1,1)
            p1.SetBottomMargin(0.075)
            centralhist.SetTitle("{}/{}".format(centralsamplename,d_hists[hist]))

            legend = TLegend(0.67,0.87-4*0.06,0.87,0.87)
            legend.SetFillStyle(1001)
            legend.AddEntry(centralhist,"Central")
            legend.AddEntry(privatehist,"Private")

            ratio = centralhist.Clone("central")
            privateclone = privatehist.Clone("private")
            ratio.Divide(privateclone) ; ratio.SetStats(0)
            ratiosyst = ratio.Clone("ratioclone")
            ratiosyst.SetFillColorAlpha(12,0.6)
            ratiosyst.SetFillStyle(3144)
            ratio.SetTitle("")
            ratio.GetYaxis().SetRangeUser(0.,2.5)
            ratio.GetYaxis().SetTitle("Central/Private")
            ratio.SetMarkerStyle(20)
            ratio.GetYaxis().CenterTitle()
            #ratio.GetYaxis().SetTitleSize(0.1)

            centralhist.SetFillColorAlpha(kBlue+1,0.3)
            privatehist.SetMarkerStyle(20)
            p1.cd()
            p1.SetLogy()
            centralhist.Draw("f&hist")
            privatehist.Draw("e1&p&hist&same")
            legend.Draw()
            p2.cd()
            ratio.Draw("p&hist")
            ratiosyst.Draw("e2&f&same")
            c.cd()
            p1.Draw()
            p2.Draw()
            c.SaveAs("SampleValidation/{}/{}/{}.png".format(centralsamplename,era,d_hists[hist]))
            c.Close()



