import os

with open("/data9/Users/youngwan/work/SKFlatAnalyzer_Sandbox/WRTau/xsec.dat") as f :
    for l in f :
        mwr = l.split(",")[0]
        mn = l.split(",")[1]
        xsec = l.split(",")[2]
        if os.path.isfile(f"./WRtoTauNtoTauTauJets_WR{mwr}_N{mn}.txt") :
            os.system(f"sed -i s/FIXMECROSSSECTION/{xsec}/g WRtoTauNtoTauTauJets_WR{mwr}_N{mn}.txt")