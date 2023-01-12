#! /usr/bin/env python
# Example of how to read the tau JSON file
# For more information, see the README in
# https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/master/POG/JME
from correctionlib import _core

# Load CorrectionSet
fname = "jmar.json.gz"
if fname.endswith(".json.gz"):
    import gzip
    with gzip.open(fname,'rt') as file:
        data = file.read().strip()
        evaluator = _core.CorrectionSet.from_string(data)
else:
    evaluator = _core.CorrectionSet.from_file(fname)


# Example how to read JSON
    
##### PU JetID
eta, pt, syst, wp = 2.0,20.,"nom","L"
map_name = "PUJetID_eff"
valsf= evaluator[map_name].evaluate(eta, pt, syst, wp)
print("Example for "+map_name)
print("The "+syst+" SF for a Jet with pt="+str(pt) + " GeV and eta="+str(eta) + " for the "+wp+" working point is "+str(valsf))




etabins = ['-3.0', '-2.75', '-2.5', '-2.0', '-1.479', '0.0', '1.479', '2.0', '2.5', '2.75', '3.0', '5.0']
diff=0.1
v_etabins = [-3.0-diff, -2.75-diff, -2.5-diff, -2.0-diff, -1.479-diff, 0.0-diff, 1.479-diff, 2.0-diff, 2.5-diff, 2.75-diff, 3.0-diff, 5.0-diff]           
ptbins = ['25.0', '30.0', '40.0', '50.0']
v_ptbins=[25-diff, 30-diff, 40-diff, 50-diff]


Eras=["2016preVFP","2016postVFP","2017","2018"]

WPs = [["Loose","L"], ["Medium","M"],["Tight","T"]]

for era in Eras:

    # Load CorrectionSet
    
    fname = "/Users/john/Desktop/JMAR/jmar_UL"+era+".json.gz"
    if fname.endswith(".json.gz"):
        import gzip
        with gzip.open(fname,'rt') as file:
            data = file.read().strip()
            evaluator = _core.CorrectionSet.from_string(data)
    else:
        evaluator = _core.CorrectionSet.from_file(fname)


    print( '\t'+'if(DataEra =="'+era+'"){')
    for WP in WPs:
        wp = WP[1]
        print( '\t\t'+'if(WP=="'+WP[0]+'"){')
        for i_etabin in range(0,len(etabins)):
            if i_etabin == 0:
                print( '\t\t\t'+'if( eta < '+etabins[i_etabin]+'  ){')
            else:
                print( '\t\t\t'+'else if( eta < '+etabins[i_etabin]+'  ){')
                
            for i_ptbin in range(0, len(ptbins)) :
                if i_ptbin==0:
                    print( '\t\t\t\t'+'if( pt < '+ptbins[i_ptbin]+'  ){')
                else:
                    print( '\t\t\t\t'+'else if( pt < '+ptbins[i_ptbin]+'  ){')
                print( '\t\t\t\t\t'+'value='+ str(evaluator[map_name].evaluate(v_etabins[i_etabin], v_ptbins[i_ptbin], "nom", wp)))
                print( '\t\t\t\t\t'+'if(sys==1) value='+ str(evaluator[map_name].evaluate(v_etabins[i_etabin], v_ptbins[i_ptbin], "up", wp)))
                print( '\t\t\t\t\t'+'if(sys==-1) value='+ str(evaluator[map_name].evaluate(v_etabins[i_etabin], v_ptbins[i_ptbin], "down", wp)))
                
                print( '\t\t\t\t}')
            print( '\t\t\t}')
        print( '\t\t}')
    print( '\t}')
