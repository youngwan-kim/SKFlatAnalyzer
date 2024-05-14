from ROOT import *

f = TFile("FitResults.root")
out = TFile("FitResultsNew.root", "RECREATE")

for era in ["2016","2017","2018"] :
    for rg in ["ResolvedSignalRegionMETInvert", "BoostedSignalRegionMETInvert"] :
        for ch in ["ElTau","MuTau"] :
            fr = f.Get(f"{rg}_{ch}_{era}/f").Clone(f"{rg}_{ch}_{era}")
            err = f.Get(f"{rg}_{ch}_{era}/unc").Clone(f"{rg}_{ch}_{era}_err")
            err.SetFillColor(kRed)
            err.SetFillStyle(1)
            fr.Write()
            err.Write()