import os 

datapath = '/data9/Users/youngwan/work/SKFlatAnalyzer_Sandbox/WRTauUtilities/Data/xsec_new.csv'

with open(datapath) as f : 
    for l in f :
        mwr = l.split(",")[0]
        mn = l.split(",")[1]
        xsec = l.split(",")[2]
        print(mwr,mn,xsec)
        os.system(f"sed -i s/FIXMECROSSSECTION/{xsec}/g /data9/Users/youngwan/work/SKFlatAnalyzer_Sandbox/data/Run2UltraLegacy_v3/2018/Sample/CommonSampleInfo/WRtoNTautoTauTauJJ_WR{mwr}_N{mn}.txt")