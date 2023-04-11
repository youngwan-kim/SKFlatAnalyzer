import os

l_qcd = ["QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8" ,
"QCD_Pt_15to30_TuneCP5_13TeV_pythia8"     ,
"QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8",
"QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8",  
"QCD_Pt_600to800_TuneCP5_13TeV_pythia8",
"QCD_Pt_120to170_TuneCP5_13TeV_pythia8"   ,
"QCD_Pt_170to300_TuneCP5_13TeV_pythia8"   ,
"QCD_Pt_300to470_TuneCP5_13TeV_pythia8"   ,
"QCD_Pt_470to600_TuneCP5_13TeV_pythia8"  , 
"QCD_Pt_800to1000_TuneCP5_13TeV_pythia8",
"QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8" ,
"QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8", 
"QCD_Pt_30to50_TuneCP5_13TeV_pythia8" ,    
"QCD_Pt_50to80_TuneCP5_13TeV_pythia8",     
"QCD_Pt_80to120_TuneCP5_13TeV_pythia8"]

for qcd in l_qcd : 
    #print(f"{qcd}_TauHLT")
    propername = qcd.rsplit("_",3)[0]
    with open(f"data/Run2UltraLegacy_v3/2017/Sample/CommonSampleInfo/{propername}.txt") as f :
        lines = f.readlines()
        #print(lines)
        pd = lines[1].split("\t")[1]
        xsec = lines[1].split("\t")[2]
        #print(f"{pd} and {xsec}")
    os.system(f"sed -i s/SKFlat_Run2UltraLegacy_v3/{pd}/g data/Run2UltraLegacy_v3/2017/Sample/CommonSampleInfo/{qcd}_TauHLT.txt")
    os.system(f"sed -i s/FIXMECROSSSECTION/{xsec}/g data/Run2UltraLegacy_v3/2017/Sample/CommonSampleInfo/{qcd}_TauHLT.txt")
    #os.system(f"find /gv0/Users/youngwan/WRTauBkgMC/2017/{qcd}/ -type f |sort -V > data/Run2UltraLegacy_v3/2017/Sample/ForSNU/{qcd}_TauHLT.txt")