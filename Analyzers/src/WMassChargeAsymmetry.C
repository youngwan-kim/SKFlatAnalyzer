#include "WMassChargeAsymmetry.h"

void WMassChargeAsymmetry::initializeAnalyzer(){

  if(DataYear==2016){
    Triggers = {"HLT_IsoMu24_v"};
    TriggerSafePtCut = 26.;
  }
  else if(DataYear==2017){
    Triggers = {"HLT_IsoMu27_v"};
    TriggerSafePtCut = 29.;
  }

}

void WMassChargeAsymmetry::executeEvent(){

  AllMuons = GetAllMuons();

  AnalyzerParameter param;
  param.Clear();
  param.Name = "Central";

  weight_Prefire = GetPrefireWeight(0);

  executeEventFromParameter(param);

}

void WMassChargeAsymmetry::executeEventFromParameter(AnalyzerParameter param){

  Event ev = GetEvent();
  double weight = 1.0;
  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= ev.MCweight() * ev.GetTriggerLumi("Full") *  weight_Prefire * GetPileUpWeight(nPileUp,0);
  }

  FillHist(param.Name+"/Cutflow",0.,weight,10,0.,10.);

  if(!PassMETFilter()) return;
  FillHist(param.Name+"/Cutflow",1.,weight,10,0.,10.);


  Particle METv = ev.GetMETVector();

  if(!(ev.PassTrigger(Triggers))) return;
  FillHist(param.Name+"/Cutflow",2.,weight,10,0.,10.);

  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> muons = SelectMuons(this_AllMuons, "POGTight", TriggerSafePtCut, 2.4);
  std::sort(muons.begin(),muons.end(),PtComparing);

  if(!(muons.size()>0)) return;
  FillHist(param.Name+"/Cutflow",2.,weight,10,0.,10.);

  FillHist(param.Name+"/MET",METv.Pt(),weight,600,0.,6000.);
  TString ChargeTag = "Mu";
  for(auto const & mu : muons){
    if(mu.Charge()>0) ChargeTag += "Plus";
    else ChargeTag += "Minus";
    double w_mu = mcCorr->MuonID_SF("NUM_TightID_DEN_genTracks", mu.Eta(), mu.MiniAODPt());
    double m = MT(mu,METv);
    FillHist(param.Name+"/"+ChargeTag+"/MT",m,weight*w_mu,600,0.,6000.);
    FillHist(param.Name+"/"+ChargeTag+"/MuonPt",mu.Pt(),weight*w_mu,500,0.,2500.);
    FillHist(param.Name+"/"+ChargeTag+"/MuonEta",mu.Eta(),weight*w_mu,60,-3.0,3.0);
  }

}

WMassChargeAsymmetry::WMassChargeAsymmetry(){

}

WMassChargeAsymmetry::~WMassChargeAsymmetry(){

}


