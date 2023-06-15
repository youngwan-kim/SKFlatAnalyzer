#include "TauQCDFakeStudy.h"

void TauQCDFakeStudy::initializeAnalyzer(){

  if(DataYear == 2017){
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
  }

}

void TauQCDFakeStudy::executeEvent(){


  AnalyzerParameter param;

  TriggerList.clear();
  TriggerList = SingleTauTriggers;

  param.Name = "TauQCDFakeStudy_SingleTauTrigger_TightLeptons";
  param.Electron_Tight_ID = "passHEEPID";
  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllGens = GetGens();

  executeEventFromParameter(param);

}

void TauQCDFakeStudy::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);
  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Tau> this_AllTaus = AllTaus;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = UseTunePMuon(AllMuons);

  vector<Tau> TightTaus = SelectTaus(AllTaus, "TightFakeStudyID", 50, 2.4);
  vector<Tau> LooseTaus = SelectTaus(AllTaus, "LooseFakeStudyID", 50, 2.4);
  
  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.4);
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.4);
  vector<Lepton *> leptons = CombineLeptonPointerVector(electrons,muons);

  std::sort(TightTaus.begin(),TightTaus.end(),PtComparing);
  std::sort(LooseTaus.begin(),LooseTaus.end(),PtComparing);
  std::sort(leptons.begin(),leptons.end(),PtComparingPtr);

  TString path = param.Name;

  FillHist(path+"/Cutflow",0.,weight,10,0.,10.);

  if(!ev.PassTrigger(TriggerList)) return;
  FillHist(path+"/Cutflow",1.,weight,10,0.,10.);

  bool hasLooseTau = (LooseTaus.size() == 1 && LooseTaus.at(0).Pt()>190 );
  bool hasTightTau = (TightTaus.size() == 1 && TightTaus.at(0).Pt()>190 );

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else{
      std::string str_Era = "UL"+std::to_string(DataYear);
      TauIDSFTool *tauSFTool_VSJet_T = new TauIDSFTool(str_Era,DeepTauVSjet,"Tight");
      TauIDSFTool *tauSFTool_VSJet_VVVL = new TauIDSFTool(str_Era,DeepTauVSjet,"VVVLoose");
      if(hasLooseTau) weight *= tauSFTool_VSJet_VVVL->getSFvsPT(LooseTaus.at(0).Pt());
      else if(hasTightTau) weight *= tauSFTool_VSJet_T->getSFvsPT(TightTaus.at(0).Pt());
    } 
  }

  if(!(hasLooseTau || hasTightTau)) return;
  FillHist(path+"/Cutflow",2.,weight,10,0.,10.); 

  bool isCR(false);
  bool isSR(false);
  if(leptons.size()==1 && leptons.at(0)->Pt()>53) isSR = true;
  else if(leptons.size()==0) isCR = true;

  if(isCR){
    if(hasLooseTau) FillHist(path+"/CRLoose/TauPt_DM"+TString::Itoa(LooseTaus.at(0).DecayMode(),10),LooseTaus.at(0).Pt(),weight,5000,0.,5000.);
    else if(hasTightTau) FillHist(path+"/CRTight/TauPt_DM"+TString::Itoa(TightTaus.at(0).DecayMode(),10),TightTaus.at(0).Pt(),weight,5000,0.,5000.);
  }

}

TauQCDFakeStudy::TauQCDFakeStudy(){

}

TauQCDFakeStudy::~TauQCDFakeStudy(){

}


