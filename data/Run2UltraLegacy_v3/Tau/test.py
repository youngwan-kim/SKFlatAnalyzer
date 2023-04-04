from ROOT import TH1D,TFile

f = TFile("TauID_SF_dm_DeepTau2017v2p1VSjet_VSjetLoose_VSeleVVLoose_Mar07.root")
h = f.Get("DM0_2017_hist")

print h.FindBin(199.9)

print h.GetBinContent(1)
print h.GetBinContent(h.FindBin(1992.9))