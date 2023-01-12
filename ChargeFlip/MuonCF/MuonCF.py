import os,sys
from ROOT import TClass, TCanvas, TPad, TFile, TPaveLabel, TPaveText, TLatex, TLegend, TH1F, kRed, kGreen, kBlue, gROOT, kOrange
from ROOT.Math import RootFinder, GSLMultiRootFinder, WrappedMultiTF1
from ROOT import TF1, TF3
import array
from math import sqrt
#from ROOT import Math
gROOT.SetBatch(True)

for era in ["2016","2017","2018"] :
    f = TFile(f"HNL_ChargeFlip_{era}.root")
    h_total = f.Get("SignalStudyUL/MuonCFrate/nOS")
    h_nSS = f.Get("SignalStudyUL/MuonCFrate/nSS")

    h_total.Add(h_nSS)
    h_prob = h_nSS.Clone("nSS/(nOS+nSS)")
    h_total_clone = h_total.Clone("nAll")
    h_prob.Divide(h_total)
    h_prob.GetYaxis().SetRangeUser(1e-08,1)


    ratio = []
    error = []
    for binnum in range(1,7) : 
        ratio.append(h_prob.GetBinContent(binnum))
        error.append(h_prob.GetBinError(binnum))

    #print(error)

    #f00 = TF3("f00","x*(1-x)+(1-x)*x-[0]+[1]*y+[2]*z")
    #f00.SetParameter(0,ratio[0])
    #f00.SetParameter(1,0);f00.SetParameter(2,0)
    #f10 = TF3("f10","x*(1-y)+(1-y)*x-[0]+[1]*z")
    #f10.SetParameter(0,ratio[1])
    #f10.SetParameter(1,0)
    #f20 = TF3("f20","x*(1-z)+(1-z)*x-[0]+[1]*y")
    #f20.SetParameter(0,ratio[2])
    #f20.SetParameter(1,0)
    #f11 = TF3("f11","y*(1-y)+(1-y)*y-[0]+[1]*x+[2]*z")
    #f11.SetParameter(0,ratio[3])
    #f11.SetParameter(1,0); f11.SetParameter(2,0)
    #
    #multfinder = GSLMultiRootFinder()
    #g1 = WrappedMultiTF1(f00,3)
    #g2 = WrappedMultiTF1(f10,3)
    #g3 = WrappedMultiTF1(f20,3)
    #g4 = WrappedMultiTF1(f11,3)
    #print(g1)
    #for f in [g2,g3,g4] : multfinder.AddFunction(f)
    #print(multfinder)
    #multfinder.SetPrintLevel(1)
    #multfinder.SetDefaultTolerance(1e-10)
    #l_eff = [2.25256e-06,7.54775e-06,1e-07]
    #eff = array.array('d',l_eff)
    #multfinder.Solve(eff)

    f00 = TF1("f00","-2*x*x + 2*x-[0]",-3,3)
    f00.SetParameter(0,ratio[0])
    finder = RootFinder()
    finder.Solve(f00,0,3)
    #print(f"eff0 = {finder.Root()}")
    eff0 = finder.Root()

    if ratio[3] != 0.0 : f00.SetParameter(0,ratio[3])
    finder = RootFinder(); finder.Solve(f00,0,3)
    #print(f"eff1 = {finder.Root()}")
    eff1 = finder.Root()

    f20 = TF1("f20","[0]*x+[1]-[2]")
    if ratio[2] != 0.0 : 
        f20.SetParameter(0,1-2*eff0)
        f20.SetParameter(1,eff0)
        f20.SetParameter(2,ratio[2])
        #finder = RootFinder(); finder.Solve(f20,0,1)
        #print(f"eff2 = {finder.Root()}") 

    eff2 = (ratio[2]-eff0)/(1-2*eff0)
    #print(f"eff2 = {eff2}")
    #print(f"Test :: obs_p10 = {ratio[1]} / fit_p10 ={eff1*(1-eff0)+(1-eff1)*eff0}")
    
    # literal error propagation
    err0 = error[0]/(2-4*eff0)
    err1 = error[3]/(2-4*eff1)
    err2 = (error[2]-err0+2*eff2*err0)/(1-2*eff0)

    # test analytical solution 
    #eff0 = (-2+sqrt(4+8*ratio[0]))/(4.)
    #eff1 = (-2+sqrt(4+8*ratio[3]))/(4.)
    #err0 = error[0]/4*sqrt(4+8*ratio[0])
    #err1 = error[3]/4*sqrt(4+8*ratio[3])

    #print(f"err0 = {err0} / err1 = {err1}")

    print(f"[{era}] eff0 = {eff0} ± {err0}")
    print(f"[{era}] eff1 = {eff1} ± {err1}")
    print(f"[{era}] eff2 = {eff2} ± {err2}")
    
    #c = TCanvas("","",1200,900)
    #c.cd()
    #c.SetLogy()
    #h_prob.Draw("e1&hist")
    #c.SaveAs(f"MuonCF_{era}.png")
    #c.Close()