import os

era = ["2018"]
sample = [
'WRtoNTautoTauTauJJ_WR3000_N2000_TuneCP5_13TeV_madgraph-pythia8',
'WRtoNTautoTauTauJJ_WR3500_N2200_TuneCP5_13TeV_madgraph-pythia8',
'WRtoNTautoTauTauJJ_WR4000_N300_TuneCP5_13TeV_madgraph-pythia8',
'WRtoNTautoTauTauJJ_WR4500_N1000_TuneCP5_13TeV_madgraph-pythia8',
'WRtoNTautoTauTauJJ_WR4500_N4200_TuneCP5_13TeV_madgraph-pythia8',
'WRtoNTautoTauTauJJ_WR5000_N1300_TuneCP5_13TeV_madgraph-pythia8',
]

for e in era :
    for s in sample :
        s_txt = s.rsplit("_",3)[0]
        print(s_txt)
        os.system(f"ls /gv0/DATA/SKFlat/Run2UltraLegacy_v3/{e}/MC/{s}")
        os.system(f"find /gv0/DATA/SKFlat/Run2UltraLegacy_v3/{e}/MC/{s} -type f|sort -V > /data9/Users/youngwan/work/SKFlatAnalyzer_Sandbox/data/Run2UltraLegacy_v3/{e}/Sample/ForSNU/{s_txt}.txt")
        #print(f"{s} @ {e} done")