#include "EXO_16_023.h"

void EXO_16_023::initializeAnalyzer(){

}

void EXO_16_023::executeEvent(){


  AnalyzerParameter param;

  //TriggerList = {"HLT_Mu45_v","HLT_Ele45_v"};
  TriggerList = {"HLT_VLooseIsoPFTau140_Trk50_eta2p1_v"};
  
  param.Name = "EXO_16_023_Benchmark";
  param.Muon_Tight_ID = "EXO_16_023";
  param.Muon_Veto_ID = "EXO_16_023_Veto";

  param.Jet_ID = "tight";

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllGens = GetGens();

  executeEventFromParameter(param);

}

void EXO_16_023::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  vector<Jet> this_AllJets = AllJets;

  vector<Tau> this_AllTaus = AllTaus;
  vector<Muon> this_AllMuons = AllMuons;

  double weight = 1.0;
  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Muon> TightMuons = SelectMuons(this_AllMuons, param.Muon_Tight_ID , 53, 2.1);
  vector<Muon> VetoMuons  = SelectMuons(this_AllMuons, param.Muon_Veto_ID , 53, 2.1);
  vector<Jet>  Jets = SelectJets(this_AllJets, param.Jet_ID, 50, 2.4);
  vector<Tau>  Taus = SelectTaus(this_AllTaus, "EXO_16_023", 50, 2.3);

  std::sort(TightMuons.begin(),TightMuons.end(),PtComparing);
  std::sort(Jets.begin(),Jets.end(),PtComparing);
  std::sort(Taus.begin(),Taus.end(),PtComparing);

  FillHist("Cutflow",0.,weight,10,0.,10.);
       
  if(!ev.PassTrigger(TriggerList)) return;
  FillHist("Cutflow",1.,weight,10,0.,10.);

  if(TightMuons.size()!=1) return;
  FillHist("Cutflow",2.,weight,10,0.,10.);

  if(Jets.size()<2) return;
  FillHist("Cutflow",3.,weight,10,0.,10.);

  if(Taus.size()!=1) return;
  FillHist("Cutflow",4.,weight,10,0.,10.);

  if(!(Taus.at(0).DeltaR(TightMuons.at(0))>0.5)) return;
  FillHist("Cutflow",5.,weight,10,0.,10.);

  for(auto const& tau : Taus){
    FillHist("dRTauMu",TightMuons.at(0).DeltaR(tau),weight,60,0.,6.);
  }
  int i = 0;
  vector<Jet> candJets ;
  for(auto const& j : Jets){
    if(j.DeltaR(TightMuons.at(0))>0.5 && j.DeltaR(Taus.at(0))>0.5){
      candJets.push_back(j);
      i++;
    }
  }
  int k = 0;
  for(int j1=0; j1<candJets.size(); ++j1){
    for(int j2=j1+1; j2<candJets.size(); ++j2){
      if(candJets.at(j1).DeltaR(candJets.at(j2))>0.5)  k++;
    }
  }
  if(i<2) return;
  FillHist("Cutflow",6.,weight,10,0.,10.);

  FillHist("nVetoMuon",VetoMuons.size(),weight,10,0.,10.);
  if(!(Taus.at(0).Pt()>150)) return;
  FillHist("Cutflow",7.,weight,10,0.,10.);
  /*for(auto const& j : Jets){
    for(auto const& )
  }*/

  if(VetoMuons.size()==2) return;
  FillHist("Cutflow",8.,weight,10,0.,10.);

  //cout << j << endl;
  FillHist("test",k,weight,10,0.,10.);
  if(k<1) return;
  FillHist("Cutflow",9.,weight,10,0.,10.);


}

EXO_16_023::EXO_16_023(){

}

EXO_16_023::~EXO_16_023(){

}


