#include "ZTauTau.h"
#include <bitset>

void ZTauTau::initializeAnalyzer(){


  // HLT_DoubleMediumChargedIsoPFTau40_Trk1_eta2p1_Reg_v
  // HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v8
  if(DataYear==2017){
    Triggers = {"HLT_DoubleTightChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v"};
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
  vJet_vec = {0,2,4,5}; vEl_vec = {8,10,12,13}; vMu_vec = {18,20,21};
  

  map<int,TString> idname_map = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  map<int,std::string> idname_map_str = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Tau> this_AllTaus = AllTaus;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = AllMuons;

  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 40., 2.4) ;
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.4);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);

  /*cout << "==All==" << endl;
  cout << this_AllTaus.size() << endl;
  for(const auto &th : this_AllTaus){
    cout << std::bitset<27>(th.IDBit()).to_string() << " " << th.DecayMode() << " " << th.Pt() << " " << th.Eta() << endl;
  }*/


  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);

  /*cout << "==LepVeto==" << endl;
  cout << taus_lepVeto.size() << endl;
  for(const auto &th : taus_lepVeto){
    cout << std::bitset<27>(th.IDBit()).to_string() << " " << th.DecayMode() << " " << th.Pt() << " " << th.Eta() << endl;
  }*/


  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = "vJet"+idname_map[vJet_vec.at(i)]+"_vEl"+idname_map[vEl_vec.at(j)]+"_vMu"+idname_map[vMu_vec.at(k)];
        TString path = param.Name+"/"+idname;
        vector<Tau> taus = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.4);
        //vector<Tau> taus = SelectTaus_varWP(taus_lepVeto,i,j,k,50,2.4);
        std::sort(taus.begin(),taus.end(),PtComparing);

        /*cout << "---"<< idname <<"---" << endl;
        cout << taus.size() << endl;
        for(const auto &th : taus){
          cout << std::bitset<27>(th.IDBit()).to_string() << " " << th.DecayMode() << " " << th.Pt() << " " << th.Eta() << endl;
        }*/


        if(!ev.PassTrigger(Triggers)) continue;
        FillHist(path+"/Cutflow",0,weight,10,0.,10.);
        if(taus.size()!=2) continue;
        FillHist(path+"/Cutflow",1,weight,10,0.,10.);
        if(taus.at(1).Pt()<45) continue;
        FillHist(path+"/Cutflow",2,weight,10,0.,10.);

        double tau_weight(1.);

        if(!IsDATA){
          //cout << "===" << endl;
          for(const auto &tau : taus){
            std::string str_Era = "UL"+std::to_string(DataYear);
            if(!HasFlag("TauSFMethodB")) tau_weight *= GetTauIDSF(idname_map[vJet_vec.at(i)],idname_map[vEl_vec.at(j)],tau.DecayMode(),tau.Pt());
            if(HasFlag("TauSFMethodB")){
              TauIDSFTool *tauSFTool_VSJet = new TauIDSFTool(str_Era,DeepTauVSjet,idname_map_str[vJet_vec.at(i)]);
              tau_weight *= tauSFTool_VSJet->getSFvsPT(tau.Pt());
            }
            //cout << idname_map[vJet_vec.at(i)] << " " << idname_map[vEl_vec.at(j)] << " " << idname_map[vMu_vec.at(j)] << " " << tau.IDBit() << " " << tau.DecayMode() << " "  << tau.Pt() << " " << tau_weight << endl;
          }
        }

        Particle Zcand = taus.at(0)+taus.at(1);

        FillHist(path+"/dRtt",taus.at(0).DeltaR(taus.at(1)),weight,60,0.,6.);
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


