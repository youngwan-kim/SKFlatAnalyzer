#include "ZTauTau.h"

void ZTauTau::initializeAnalyzer(){

  if(DataYear==2017){
    Triggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
  }

}

void ZTauTau::executeEvent(){

  AnalyzerParameter param;

  param.Name = "ZTauTau";
  param.Electron_Veto_ID = "passVetoID";
  param.Muon_Veto_ID = "POGLoose";
  param.syst_ = AnalyzerParameter::Central;
  AllTaus = GetAllTaus();

  executeEventFromParameter(param);

}

void ZTauTau::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  vector<int> vJet_vec = {5,6};   //vL,L,M,T,vT
  vector<int> vEl_vec = {11,12,13};     //L,M,T 
  vector<int> vMu_vec = {19,20,21};     //L,M,T

  // if(param.Name == "WRTau_BkgSingleLeptonTrg")
  
  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {4,5}; vEl_vec = {9,13}; vMu_vec = {17,20,21};
  

  map<int,TString> idname_map = {{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  map<int,std::string> idname_map_str = {{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                         {9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                         {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Tau> this_AllTaus = AllTaus;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = AllMuons;

  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.4) ;
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.4);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);

  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = "vJet"+idname_map[vJet_vec.at(i)]+"_vEl"+idname_map[vEl_vec.at(j)]+"_vMu"+idname_map[vMu_vec.at(k)];
        TString path = param.Name+"/"+idname;
        vector<Tau> taus = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.4);
        std::sort(taus.begin(),taus.end(),PtComparing);

        if(!ev.PassTrigger(Triggers)) continue;
        if(taus.size()!=2) continue;
        if(taus.at(0).Pt()<190) continue;

        double tau_weight(1.);

        if(!IsDATA){
          for(const auto &tau : taus){
            std::string str_Era = "UL"+std::to_string(DataYear);
            if(!HasFlag("TauSFMethodB")) tau_weight *= GetTauIDSF(idname_map[vJet_vec.at(i)],idname_map[vEl_vec.at(j)],tau.DecayMode(),tau.Pt());
            if(HasFlag("TauSFMethodB")){
              TauIDSFTool *tauSFTool_VSJet = new TauIDSFTool(str_Era,DeepTauVSjet,idname_map_str[vJet_vec.at(i)]);
              tau_weight *= tauSFTool_VSJet->getSFvsPT(tau.Pt());
            }
          }
        }

        Particle Zcand = taus.at(0)+taus.at(1);
        if(!IsOnZ(Zcand.M(),30.)) continue;

        FillHist(path+"/ZCandMass",Zcand.M(),weight*tau_weight,180,0.,180.);
        FillHist(path+"/ZCandMass_noTauWeight",Zcand.M(),weight,180,0.,180.);

      }
    }
  }


}

ZTauTau::ZTauTau(){

}

ZTauTau::~ZTauTau(){

}


