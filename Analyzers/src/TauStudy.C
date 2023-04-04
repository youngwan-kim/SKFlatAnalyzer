#include "TauStudy.h"

void TauStudy::initializeAnalyzer(){

  lepton_triggers.clear();

  if(DataYear==2016){

    lepton_triggers = {
      "HLT_Ele27_WPTight_Gsf",
      "HLT_IsoMu24",
      "HLT_IsoTkMu24"
    };

  }

  else if(DataYear==2017){

    lepton_triggers = {
      "HLT_Ele32_WPTight_Gsf",
      "HLT_IsoMu24",
      "HLT_IsoMu24_eta2p1"
    };

  }
  else if(DataYear == 2018){

    lepton_triggers = {
      "HLT_Ele32_WPTight_Gsf",
      "HLT_IsoMu24",
      "HLT_IsoMu24_eta2p1"
    };

  }

}

void TauStudy::executeEvent(){

  AnalyzerParameter param;
  param.Clear();

  param.Name = "TauStudy";
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

void TauStudy::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);
  if(!IsDATA) weight *= MCweight() * ev.GetTriggerLumi("Full") * GetPrefireWeight(0);

  if(!( ev.PassTrigger(lepton_triggers) )) return;

  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 10., 2.1) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 5., 2.1) ;

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 10., 2.1);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 5., 2.1);

  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  for(unsigned int i=0; i<AllTaus.size(); i++){
    for(unsigned int j=0; j<VetoLeps.size(); j++){
      if(AllTaus.at(i).DeltaR(*VetoLeps.at(j))<0.4){
        AllTaus.erase(AllTaus.begin()+i);
        break;
      }
    }
  }

  JetTagging::Parameters jtps = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  for(unsigned int i=0; i<AllJets.size(); i++){
    for(unsigned int j=0; j<VetoLeps.size(); j++){
      if(AllJets.at(i).DeltaR(*VetoLeps.at(j))<0.4){
        AllJets.erase(AllJets.begin()+i);
        break;
      }
    }
  }

  vector<Tau> this_AllTaus = AllTaus;
  vector<Jet> this_AllJets = AllJets;

  vector<Tau> taus = SelectTaus(this_AllTaus, "TriLepTight", 20., 2.1);
  vector<Jet> jets = SelectJets(this_AllJets, param.Jet_ID, 25., 2.1) ;
  vector<Jet> BJets = GetBJets(this_AllJets,jtps);

  for(unsigned int i=0; i<taus.size(); i++){
    for(unsigned int j=0; j<jets.size(); j++){
      if( taus.at(i).DeltaR(jets.at(j)) < 0.05 ) jets.erase(jets.begin()+j);
    }
  }

  std::sort(muons.begin(),muons.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing);
  std::sort(electrons.begin(),electrons.end(),PtComparing);

  // Selection
  unsigned int n_region = 4;
  for(unsigned int i=0 ; i<n_region+1 ; i++){
    FillHist("Region"+to_string(i)+"/Cutflow",0.,weight,10,0.,10.);
  }

  if( muons.size()!=2 ) return;
  for(unsigned int i=0 ; i<n_region+1 ; i++){
    FillHist("Region"+to_string(i)+"/Cutflow",1.,weight,10,0.,10.);
  }

  if( !(muons.at(0).Pt() > 35 && muons.at(1).Pt() > 20 ) ) return;
  for(unsigned int i=0 ; i<n_region+1 ; i++){
    FillHist("Region"+to_string(i)+"/Cutflow",2.,weight,10,0.,10.);
  }

  bool mZ_window(false); bool bjet_tag(false);
  Particle ZCand = muons.at(0) + muons.at(1) ;
  if(BJets.size()!=0) bjet_tag = true;
  if( ZCand.M() > M_Z-15 && ZCand.M() < M_Z + 15 ) mZ_window = true;
  FillHist("Region0/Cutflow",3.,weight,10,0.,10.);

  TString region_index = "";
  if(mZ_window){
    if(bjet_tag){
      FillHist("Region1/Cutflow",3.,weight,10,0.,10.);
      FillHist("Region1/nTau",taus.size(),weight,10,0.,10.);
      region_index = "1";
    }
    else{
      FillHist("Region2/Cutflow",3.,weight,10,0.,10.);
      FillHist("Region2/nTau",taus.size(),weight,10,0.,10.);
      region_index = "2";
    }
  }
  else{
    if(bjet_tag){
      FillHist("Region3/Cutflow",3.,weight,10,0.,10.);
      FillHist("Region3/nTau",taus.size(),weight,10,0.,10.);
      region_index = "3";
    }
    else{
      FillHist("Region4/Cutflow",3.,weight,10,0.,10.);
      FillHist("Region4/nTau",taus.size(),weight,10,0.,10.);
      region_index = "4" ;
    }
  }
  
  FillHist("Region"+region_index+"_notaucut/Muon/pT",muons.at(0).Pt(),weight,60,0.,120.);
  FillHist("Region"+region_index+"_notaucut/Muon/Eta",muons.at(0).Eta(),weight,60,-3.,3.);
  FillHist("Region"+region_index+"_notaucut/Muon/phi",muons.at(0).Phi(),weight,60,-3.,3.);
  FillHist("Region"+region_index+"_notaucut/Muon/dRll",muons.at(0).DeltaR(muons.at(1)),weight,60,0.,6.);
  FillHist("Region"+region_index+"_notaucut/Muon/mll",ZCand.M(),weight,250,0.,500.);
 
  double LTnocut(0.),HTnocut(0.);
  LTnocut = muons.at(0).Pt() + muons.at(1).Pt();
  for(unsigned int i=0; i<taus.size() ; i++){
    LTnocut += taus.at(i).Pt();
  }
  for(unsigned int i=0; i<jets.size() ; i++){
    HTnocut += jets.at(i).Pt();
  }
  
  FillHist("Region"+region_index+"_notaucut/nJet",jets.size(),weight,10,0.,10.);
  FillHist("Region"+region_index+"_notaucut/LT",LTnocut,weight,250,0.,500.);
  FillHist("Region"+region_index+"_notaucut/HT",HTnocut,weight,250,0.,500.);
  FillHist("Region"+region_index+"_notaucut/MET",METv.Pt(),weight,500,0.,1000.);
  if(jets.size()!=0){
    FillHist("Region"+region_index+"_notaucut/Jet/pT",jets.at(0).Pt(),weight,150,0.,300.);
    FillHist("Region"+region_index+"_notaucut/Jet/Eta",jets.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"_notaucut/Jet/phi",jets.at(0).Phi(),weight,60,-3.,3.);
  }


  // Region 1 : mll in Z + bjet>0 (DY dominant) / Region 2 : mll in Z + bjet=0 
  // Region 3 : mll out of Z + bjet>0 (TT dominant) / Region 4 : mll out of Z + bjet=0
  if( HasFlag("WZ") == true && electrons.size() != 1)  return;

  if( HasFlag("WZ") == false && taus.size()!=1) return;
  double HT(0.),LT(0.);
  for(unsigned int i=0 ; i<jets.size(); i++) HT += jets.at(i).Pt();

  if( HasFlag("WZ") == false) LT = muons.at(0).Pt() + muons.at(1).Pt() + taus.at(0).Pt();
  else LT = muons.at(0).Pt() + muons.at(1).Pt() + electrons.at(0).Pt();
  
  FillHist("Region"+region_index+"/Cutflow",4.,weight,10,0.,10.);

  if( HasFlag("WZ") == true ){ 
    FillHist("Region"+region_index+"/Electron/pT",electrons.at(0).Pt(),weight,60,0.,120.);
    FillHist("Region"+region_index+"/Electron/Eta",electrons.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/Electron/phi",electrons.at(0).Phi(),weight,60,-3.,3.);
  }

  if( HasFlag("WZ") == false ){ 
    FillHist("Region"+region_index+"/Tau/pT",taus.at(0).Pt(),weight,60,0.,120.);
    FillHist("Region"+region_index+"/Tau/Eta",taus.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/Tau/phi",taus.at(0).Phi(),weight,60,-3.,3.);
  }

  FillHist("Region"+region_index+"/Muon/pT",muons.at(0).Pt(),weight,60,0.,120.);
  FillHist("Region"+region_index+"/Muon/Eta",muons.at(0).Eta(),weight,60,-3.,3.);
  FillHist("Region"+region_index+"/Muon/phi",muons.at(0).Phi(),weight,60,-3.,3.);
  FillHist("Region"+region_index+"/Muon/dRll",muons.at(0).DeltaR(muons.at(1)),weight,60,0.,6.);
  FillHist("Region"+region_index+"/Muon/mll",ZCand.M(),weight,250,0.,500.);


  FillHist("Region"+region_index+"/nJet",jets.size(),weight,10,0.,10.);
  FillHist("Region"+region_index+"/LT",LT,weight,250,0.,500.);
  FillHist("Region"+region_index+"/HT",HT,weight,250,0.,500.);
  FillHist("Region"+region_index+"/MET",METv.Pt(),weight,500,0.,1000.);
  if(jets.size()!=0){
    FillHist("Region"+region_index+"/Jet/pT",jets.at(0).Pt(),weight,150,0.,300.);
    FillHist("Region"+region_index+"/Jet/Eta",jets.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/Jet/phi",jets.at(0).Phi(),weight,60,-3.,3.);
    /*for(unsigned int i=0; i<jets.size(); i++){
      FillHist("Region"+region_index+"/Jet/dRtau"+to_string(i),taus.at(0).DeltaR(jets.at(i)),weight,60,0.,6.); 
    }*/
  }
  if(BJets.size()!=0){
    FillHist("Region"+region_index+"/BJet/pT",BJets.at(0).Pt(),weight,150,0.,300.);
    FillHist("Region"+region_index+"/BJet/Eta",BJets.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/BJet/phi",BJets.at(0).Phi(),weight,60,-3.,3.);
    /*for(unsigned int i=0; i<BJets.size(); i++){
      FillHist("Region"+region_index+"/BJet/dRtau"+to_string(i),taus.at(0).DeltaR(BJets.at(i)),weight,60,0.,6.); 
    }*/
  }

  // all of the regions summed up

  region_index = '0';

  if( HasFlag("WZ") == true ){ 
    FillHist("Region"+region_index+"/Electron/pT",electrons.at(0).Pt(),weight,60,0.,120.);
    FillHist("Region"+region_index+"/Electron/Eta",electrons.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/Electron/phi",electrons.at(0).Phi(),weight,60,-3.,3.);
  }

  if( HasFlag("WZ") == false ){ 
    FillHist("Region"+region_index+"/Tau/pT",taus.at(0).Pt(),weight,60,0.,120.);
    FillHist("Region"+region_index+"/Tau/Eta",taus.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/Tau/phi",taus.at(0).Phi(),weight,60,-3.,3.);
  }

  FillHist("Region"+region_index+"/Muon/pT",muons.at(0).Pt(),weight,60,0.,120.);
  FillHist("Region"+region_index+"/Muon/Eta",muons.at(0).Eta(),weight,60,-3.,3.);
  FillHist("Region"+region_index+"/Muon/phi",muons.at(0).Phi(),weight,60,-3.,3.);
  FillHist("Region"+region_index+"/Muon/dRll",muons.at(0).DeltaR(muons.at(1)),weight,60,0.,6.);
  FillHist("Region"+region_index+"/Muon/mll",ZCand.M(),weight,250,0.,500.);

  FillHist("Region"+region_index+"/nJet",jets.size(),weight,10,0.,10.);
  FillHist("Region"+region_index+"/LT",LT,weight,250,0.,500.);
  FillHist("Region"+region_index+"/HT",HT,weight,250,0.,500.);
  FillHist("Region"+region_index+"/MET",METv.Pt(),weight,500,0.,1000.);
  if(jets.size()!=0){
    FillHist("Region"+region_index+"/Jet/pT",jets.at(0).Pt(),weight,150,0.,300.);
    FillHist("Region"+region_index+"/Jet/Eta",jets.at(0).Eta(),weight,60,-3.,3.);
    FillHist("Region"+region_index+"/Jet/phi",jets.at(0).Phi(),weight,60,-3.,3.);
  }

}

TauStudy::TauStudy(){

}

TauStudy::~TauStudy(){

}


