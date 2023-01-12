import os,json


etabinsLM=[ 'abseta:[0.00,0.90]',
            'abseta:[0.90,1.20]',
            'abseta:[1.20,2.10]',
            'abseta:[2.10,2.40]']

etabinsMM=['abseta:[0.0,0.9]',
           'abseta:[0.9,1.2]',
           'abseta:[1.2,2.1]',
           'abseta:[2.1,2.4]']
           

ptbinsLM=['pt:[2.00,2.50]',
        'pt:[2.50,2.75]',
        'pt:[2.75,3.00]',
        'pt:[3.00,3.25]',
        'pt:[3.25,3.50]',
        'pt:[3.50,3.75]',
        'pt:[3.75,4.00]',
        'pt:[4.00,4.50]',
        'pt:[4.50,5.00]',
        'pt:[5.00,6.00]',
        'pt:[6.00,8.00]',
        'pt:[8.00,10.00]',
        'pt:[10.00,15.00]',
        'pt:[15.00,20.00]',
        'pt:[20.00,30.00]',
        'pt:[30.00,40.00]']

ptbinsMM=['pt:[40,60]']

reco_corr =  [ 
    
    ["MM","2016preVFP",etabinsMM, ptbinsMM,"/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2016preVFP/ID/Muon/NUM_TrackerMuons_DEN_genTracks_Z_abseta_pt_2016preVFP.json"],
    ["MM","2016postVFP",etabinsMM, ptbinsMM,"/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2016postVFP/ID/Muon/NUM_TrackerMuons_DEN_genTracks_Z_abseta_pt_2016postVFP.json"],
    ["MM","2017",etabinsMM, ptbinsMM,"/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2017/ID/Muon/NUM_TrackerMuons_DEN_genTracks_Z_abseta_2017_pt.json"],
    ["MM","2018",etabinsMM, ptbinsMM,"/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2018/ID/Muon/NUM_TrackerMuons_DEN_genTracks_Z_abseta_pt.json"],

    #["LM", "2016preVFP", etabinsLM, ptbinsLM, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2016preVFP/ID/Muon/Efficiency_muon_generalTracks_Run2016preVFP_UL_trackerMuon.json"],       
    #["LM", "2016postVFP", etabinsLM, ptbinsLM, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2016postVFP/ID/Muon/Efficiency_muon_generalTracks_Run2016postVFP_UL_trackerMuon.json"],       
    #["LM", "2017", etabinsLM, ptbinsLM, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2017/ID/Muon/Efficiency_muon_generalTracks_Run2017_UL_trackerMuon.json"],   
    #["LM", "2018", etabinsLM, ptbinsLM, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/data/Run2UltraLegacy_v3/2018/ID/Muon/Efficiency_muon_generalTracks_Run2018_UL_trackerMuon.json"]
]
    

for JSON in reco_corr:
    
    muon_json = JSON[4]
    ptbins =  JSON[3]
    
    Year=JSON[1]

    etabins=JSON[2]
    
    Flag=JSON[0]
    
    fjson = open(muon_json)

    data = json.load(fjson)
    
    Output=[]
    for i in  data['NUM_TrackerMuons_DEN_genTracks']['abseta_pt']:
        if not "abseta" in i:
            continue
        if not i in etabins:
            print i

        for j in  data['NUM_TrackerMuons_DEN_genTracks']['abseta_pt'][i]:
            if not j in ptbins:
                print j
            for k in data['NUM_TrackerMuons_DEN_genTracks']['abseta_pt'][i][j]:
                Output.append( i + ' ' +j+ ' ' + k + " : " +  str(data['NUM_TrackerMuons_DEN_genTracks']['abseta_pt'][i][j][k]))

    fjson.close()


    print ("\t" + 'if(DataEra=="'+Year+'"){')

    for etabin in etabins:
        etastring= etabin
        etastring=etastring.replace(',',' ')
        etastring=etastring.replace(']',' ')
        
        if "[0.0" in etabin:
            print ("\t\t" + "if( eta < " + str(etastring.split()[1])+"  ){") 
        else:
            print ("\t\t" + "else if( eta < " + str(etastring.split()[1])+"  ){") 
        
        for ptbin in ptbins:
            ptstring= ptbin
            ptstring=ptstring.replace(',',' ')
            ptstring=ptstring.replace(']',' ')
            
            if Flag == "LM":
                if ptbin == "pt:[2.00,2.50]":
                    print ("\t\t\t"+ "if( pt < "+str(ptstring.split()[1])+"){") 
                else:
                    print ("\t\t\t"+ "else if( pt < "+str(ptstring.split()[1])+"){")

            if Flag == "MM":
                print ("\t\t\t"+ "if( pt < "+str(ptstring.split()[1])+"){")

                
            for line in Output:
                if  etabin in line and ptbin in line:

                    
                    if Flag == "LM":
                        value = line.split()[4]
                        print ("\t\t\t\t " +  line.split()[2] + '='+ str(value) + ";")

                    if Flag == "MM" and line.split()[2] == "value":
                        value = line.split()[4]
                        print ("\t\t\t\t " +  line.split()[2] + '='+ str(value) + ";")

                    if Flag == "MM" and line.split()[2] == "syst":
                        value = line.split()[4]
                        print ("\t\t\t\t error" + '='+ str(value) + ";")



            print ("\t\t\t}")

        print ("\t\t}")
    print ("\t}")

                






