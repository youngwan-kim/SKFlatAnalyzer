#include "HNL_TriLep.h"

void HNL_TriLep::initializeAnalyzer(){

  trilepton_triggers.clear();
  dilepton_triggers.clear();
  lepton_triggers.clear();

  if(DataYear==2016){

    trilepton_triggers = {
      "HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL",
      "HLT_Mu8_DiEle12_CaloIdL_TrackIdL",
      "HLT_DiMu9_Ele9_CaloIdL_TrackIdL",
      "HLT_TripleMu_12_10_5"
    };

    dilepton_triggers = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",
      "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL",
      "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ"
    };

    lepton_triggers = {
      "HLT_Ele27_WPTight_Gsf",
      "HLT_IsoMu24",
      "HLT_IsoTkMu24"
    };

  }

  else if(DataYear==2017){

    trilepton_triggers = {
      "HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL",
      "HLT_Mu8_DiEle12_CaloIdL_TrackIdL",
      "HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ",
      "HLT_TripleMu_12_10_5",
      "HLT_TripleMu_10_5_5_DZ"
    };
    
    dilepton_triggers = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"
    };

    lepton_triggers = {
      "HLT_Ele32_WPTight_Gsf",
      "HLT_IsoMu24",
      "HLT_IsoMu24_eta2p1"
    };

  }
  else if(DataYear == 2018){

    trilepton_triggers = {
      "HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL",
      "HLT_Mu8_DiEle12_CaloIdL_TrackIdL",
      "HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ",
      "HLT_TripleMu_12_10_5",
      "HLT_TripleMu_10_5_5_DZ"
    };
    
    dilepton_triggers = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8"
    };

    lepton_triggers = {
      "HLT_Ele32_WPTight_Gsf",
      "HLT_IsoMu24",
      "HLT_IsoMu24_eta2p1"
    };

  }

}

void HNL_TriLep::executeEvent(){


  AnalyzerParameter param;

  param.Clear();

  param.Name = "HNL_TriLep";
  param.Electron_Tight_ID = "TriLepEleTight";
  param.Electron_Loose_ID = "TriLepEleLoose";
  param.Muon_Tight_ID = "TriLepMuTight";
  param.Muon_Loose_ID = "TriLepMuLoose";
  param.Jet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();

  executeEventFromParameter(param);

}

void HNL_TriLep::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);
  if(!IsDATA) weight *= MCweight() * ev.GetTriggerLumi("Full");

  // Trigger Cut
  if(!( ev.PassTrigger(trilepton_triggers) || ev.PassTrigger(dilepton_triggers) || ev.PassTrigger(lepton_triggers) )) return;

  // All Leptons
  AllLeptons.clear();
  AllLeptons = CombineLeptonPointerVector(AllElectrons,AllMuons);
  if(HasFlag("debug")) cout << "[DEBUG] AllLeptons" << endl;

  // Muon Selection
  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 10., 2.4) ;
  vector<Muon> muons_FO = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 10., 2.4) ;
  vector<Muon> muons_loose = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 5., 2.4) ;
  if(HasFlag("debug")) cout << "[DEBUG] MuonSelection" << endl;

  // Remove Electrons having Loose Muon within dR<0.05
  for(unsigned int i=0 ; i<AllElectrons.size(); i++){
    for(unsigned int j=0; j<muons_loose.size(); j++){
      if(AllElectrons.at(i).DeltaR(muons_loose.at(j))<0.05){
        AllElectrons.erase(AllElectrons.begin()+i);
        break;
      }
    }
  }
  if(HasFlag("debug")) cout << "[DEBUG] LooseMuonCleaning" << endl;

  // Electron Selection
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 10., 2.5);
  vector<Electron> electrons_FO = SelectElectrons(this_AllElectrons, "TriLepEleFO", 10., 2.5);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 5., 2.5);
  
  // Fakeable Object Selection

  double MVASlope(0); double MVAIntercept(0);
  if(DataYear==2016){
    MVASlope = -.00025; MVAIntercept = .025;
  }
  if(DataYear==2017 || DataYear==2018){
    MVASlope = -.0005; MVAIntercept = .035;
  }

/*vector<Muon> muons_FO;
  for(unsigned int i=0; i<AllMuons.size(); i++){
    if(!(AllMuons.at(i).PassID(param.Muon_Loose_ID))) continue;
    if(!(AllMuons.at(i).Pt()>10. && fabs(AllMuons.at(i).Eta())<2.4)) continue;
    Jet closestJet = GetClosestJet(AllJets,muons_FO.at(i));
    if(AllMuons.at(i).MVA()>0.4) continue;
    double CutFinal = MVASlope*AllMuons.at(i).Pt()+MVAIntercept;
    muons_FO.push_back(AllMuons.at(i));
  }*/

  for(unsigned int i=0; i<muons_FO.size(); i++){
    Jet closestJet = GetClosestJet(AllJets,muons_FO.at(i));
    if(muons_FO.at(i).MVA()>0.4){
      muons_FO.erase(muons_FO.begin()+i);
      continue;
    }
    else{
      double CutFinal = MVASlope*muons_FO.at(i).Pt()+MVAIntercept;
      if(!( muons_FO.at(i).Pt()/closestJet.Pt()>0.45 && closestJet.GetTaggerResult(JetTagging::DeepCSV) < CutFinal)){
        muons_FO.erase(muons_FO.begin()+i);
      }
    }
  }
  if(HasFlag("debug")) cout << "[DEBUG] MuonFOSelection" << endl;

  for(unsigned int i=0; i<electrons_FO.size(); i++){
    Jet closestJet = GetClosestJet(AllJets,electrons_FO.at(i));
    if(electrons_FO.at(i).MVAIso()>0.4){
      electrons_FO.erase(electrons_FO.begin()+i);
      continue;
    }
    else{
      if(!( electrons_FO.at(i).Pt()/closestJet.Pt()>0.45 && closestJet.GetTaggerResult(JetTagging::DeepCSV)>0.5 )) electrons_FO.erase(electrons_FO.begin()+i);
    }
  }
  if(HasFlag("debug")) cout << "[DEBUG] ElectronFOSelection" << endl;

  // Loose Lepton Clean AllTaus/AllJets
  vector<Lepton *> LooseLeps = CombineLeptonPointerVector(electrons_loose,muons_loose);
  for(unsigned int i=0; i<AllTaus.size(); i++){
    for(unsigned int j=0; j<LooseLeps.size(); j++){
      if(AllTaus.at(i).DeltaR(*LooseLeps.at(j))<0.4){
        AllTaus.erase(AllTaus.begin()+i);
        break;
      }
    }
  }
  if(HasFlag("debug")) cout << "[DEBUG] TauLooseLepClean" << endl;

  // Select b-jets before loose lepton cleaning 
  // FO Lepton cleaning + 1a method reweighting
  vector<Lepton *> FOLeps = CombineLeptonPointerVector(electrons_FO,muons_FO);
  vector<Jet> AllJets_tmp = AllJets;
  for(unsigned int i=0; i<AllJets_tmp.size(); i++){
    for(unsigned int j=0; j<FOLeps.size(); j++){
      if(AllJets_tmp.at(i).DeltaR(*FOLeps.at(j))<0.4){
        AllJets_tmp.erase(AllJets_tmp.begin()+i);
        break;
      }
    }
  }
  if(HasFlag("debug")) cout << "[DEBUG] BJetFOLepClean" << endl;

  JetTagging::Parameters jtps = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Loose, JetTagging::incl, JetTagging::mujets);
  vector<Jet> jets_tmp = SelectJets(AllJets_tmp, param.Jet_ID, 25., 2.4) ;
  vector<Jet> BJets = GetBJets(jets_tmp,jtps);
  if(HasFlag("debug")) cout << "[DEBUG] BJetSelection" << endl;

  // Loose Lepton Clean AllJets
  for(unsigned int i=0; i<AllJets.size(); i++){
    for(unsigned int j=0; j<LooseLeps.size(); j++){
      if(AllJets.at(i).DeltaR(*LooseLeps.at(j))<0.4){
        AllJets.erase(AllJets.begin()+i);
        break;
      }
    }
  }
  if(HasFlag("debug")) cout << "[DEBUG] JetsLooseLepClean" << endl;

  // Tau & Jet Collection
  vector<Tau> this_AllTaus = AllTaus;
  vector<Jet> this_AllJets = AllJets;

  vector<Tau> taus = SelectTaus(this_AllTaus, "TriLepTight", 20., 2.3);
  vector<Tau> taus_loose = SelectTaus(this_AllTaus, "TriLepLoose", 20., 2.3);
  vector<Jet> jets = SelectJets(this_AllJets, param.Jet_ID, 25., 2.4) ;

  vector<Lepton *> TightLeptons = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> FOLeptons = CombineLeptonPointerVector(electrons_FO,muons_FO);
  std::sort(TightLeptons.begin(),TightLeptons.end(),PtComparingPtr);
  if(HasFlag("debug")) cout << "[DEBUG] TightLepton" << endl;

  // Baseline Selection
  // 1. 3 tight leptons
  if(!( TightLeptons.size() == 3)) return;
  FillHist("CutFlow",1.,weight,10.,0.,10);
  if(HasFlag("debug")) cout << "[DEBUG] TightTriLepCut" << endl;

  // 2. 4th FO lepton veto
  if(FOLeptons.size() == 4) return;
  FillHist("CutFlow",2.,weight,10.,0.,10);
  if(HasFlag("debug")) cout << "[DEBUG] FourthFOVeto" << endl;

  // 3. same sign veto
  if(TightLeptons.at(0)->Charge() == TightLeptons.at(1)->Charge() && TightLeptons.at(1)->Charge() == TightLeptons.at(2)->Charge()) return;
  FillHist("CutFlow",3.,weight,10.,0.,10);
  if(HasFlag("debug")) cout << "[DEBUG] SameSignVeto" << endl;

  // 4. b-jet veto
  if(BJets.size()>0) return;
  FillHist("CutFlow",4.,weight,10.,0.,10);
  if(HasFlag("debug")) cout << "[DEBUG] BJetVeto" << endl;

  // 5. pT cut
  if(!( TightLeptons.at(0)->Pt() > 15 && TightLeptons.at(1)->Pt() > 10 && TightLeptons.at(2)->Pt() > 10 )) return;
  FillHist("CutFlow",5.,weight,0.,10.,10);
  if(HasFlag("debug")) cout << "[DEBUG] pTCut" << endl;

  // 6. OSSF mZ 15GeV window cut (for all OSSF pairs) + mOSSF > 5
  for(unsigned int i=0; i<TightLeptons.size(); i++){
    for(unsigned int j=0; j<TightLeptons.size(); j++){
      if(i>=j) continue;
      else{
        Particle OSSF = *TightLeptons.at(i) + *TightLeptons.at(j);
        double mOSSF = OSSF.M();
        if((TightLeptons.at(i)->LeptonFlavour() == TightLeptons.at(j)->LeptonFlavour()) 
            && (TightLeptons.at(i)->Charge()*TightLeptons.at(j)->Charge() < 0)
            && !(mOSSF<M_Z-15 || mOSSF>M_Z+15) 
            && !(mOSSF>5)) return;
      }
    }
  }
  FillHist("CutFlow",6.,weight,10.,0.,10);
  if(HasFlag("debug")) cout << "[DEBUG] mOSSF" << endl;

}

HNL_TriLep::HNL_TriLep(){

}

HNL_TriLep::~HNL_TriLep(){

}


