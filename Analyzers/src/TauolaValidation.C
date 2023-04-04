#include "TauolaValidation.h"

void TauolaValidation::initializeAnalyzer(){

}

void TauolaValidation::executeEvent(){


  AnalyzerParameter param;
  param.Clear();

  param.Name = "TauolaValidation";
  param.Electron_Tight_ID = "passTightID";
  param.Muon_Tight_ID = "POGTight";
  param.Electron_Veto_ID = "passVetoID";
  param.Muon_Veto_ID = "POGLoose";
  param.Jet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();

  executeEventFromParameter(param);

}

void TauolaValidation::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  
  double weight(1.);
  if(!IsDATA) weight *= MCweight() * ev.GetTriggerLumi("Full") * GetPrefireWeight(0);

  vector<Tau> this_taus = AllTaus;
  vector<int> vJet_vec = {3,4,5};   //L,M,T
  vector<int> vEl_vec = {11,12,13}; //L,M,T 
  vector<int> vMu_vec = {19,21};    //L,T
  map<int,TString> idname_map = {{3,"vJetLoose"},{4,"vJetMedium"},{5,"vJetTight"},{11,"vElLoose"},{12,"vElMedium"},{13,"vElTight"},{19,"vMuLoose"},{21,"vMuTight"}};
  vector<vector <Tau>> v_Tau ;

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = idname_map[vJet_vec.at(i)]+"_"+idname_map[vEl_vec.at(j)]+"_"+idname_map[vMu_vec.at(k)];
        cout << idname << endl;
        vector<Tau> tempTau = SelectTaus_varWP(AllTaus,i,j,k,5,2.1);
        std::sort(tempTau.begin(),tempTau.end(),PtComparing);

        if(tempTau.size()<1) return;
        for(unsigned int l=0; l<tempTau.size(); l++){

          FillHist(idname+"/Tau"+TString::Itoa(l,10)+"_pT",tempTau.at(l).Pt(),weight,100,0,200);
          FillHist(idname+"/Tau"+TString::Itoa(l,10)+"_Eta",tempTau.at(l).Eta(),weight,60,-3,3);
          FillHist(idname+"/Tau"+TString::Itoa(l,10)+"_phi",tempTau.at(l).Phi(),weight,80,-4,4);
          FillHist(idname+"/Tau_pT",tempTau.at(l).Pt(),weight,100,0,200);
          FillHist(idname+"/Tau"+TString::Itoa(l,10)+"_Mass",tempTau.at(l).M(),weight,250,0.,2.5);

        }

      }
    }
  }

}

TauolaValidation::TauolaValidation(){

}

TauolaValidation::~TauolaValidation(){

}


