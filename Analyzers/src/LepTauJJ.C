#include "LepTauJJ.h"

void LepTauJJ::initializeAnalyzer(){

}

void LepTauJJ::executeEvent(){


  AnalyzerParameter param;

  param.Clear();

  param.Name = "Test";
  param.Electron_Tight_ID = "passTightID";
  param.Muon_Tight_ID = "POGTight";
  param.Jet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  
  executeEventFromParameter(param);

}

void LepTauJJ::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  double weight(1.);
  FillHist("CutFlow", 1., weight, 10, 0., 10.);

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  if(!IsDATA){
    // L1 Prefire weight not yet
    weight *= MCweight() * ev.GetTriggerLumi("Full");
  }

  vector<Muon> this_AllMuons = AllMuons;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Tau> this_AllTaus = AllTaus;
  vector<Jet> this_AllJets = AllJets;
  
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 35., 2.5) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, "POGLoose", 15., 2.5) ;
  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 25., 2.4);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, "passVetoID", 15., 2.4);

  vector<Tau> taus = SelectTaus(this_AllTaus, "HNVeto", 20., 2.4);
  vector<Jet> jets = SelectJets(this_AllJets, param.Jet_ID, 30., 2.4) ;

  vector<Lepton *> ElectronsLep = MakeLeptonPointerVector(electrons);
  vector<Lepton *> MuonsLep = MakeLeptonPointerVector(muons);
  vector<Lepton *> ElectronsVetoLep = MakeLeptonPointerVector(electrons_veto);
  vector<Lepton *> MuonsVetoLep = MakeLeptonPointerVector(muons_veto);

  Leptons.clear();
  Leptons.reserve(MuonsLep.size()+ElectronsLep.size());
  Leptons.insert(Leptons.end(),MuonsLep.begin(),MuonsLep.end());
  Leptons.insert(Leptons.end(),ElectronsLep.begin(),ElectronsLep.end());

  LeptonsVeto.clear();
  LeptonsVeto.reserve(MuonsVetoLep.size()+ElectronsVetoLep.size());
  LeptonsVeto.insert(LeptonsVeto.end(),MuonsVetoLep.begin(),MuonsVetoLep.end());
  LeptonsVeto.insert(LeptonsVeto.end(),ElectronsVetoLep.begin(),ElectronsVetoLep.end());

  if(HasFlag("debug")) cout << "[DEBUG] Object Defined" << endl;

  // B Tagging
  JetTagging::Parameters jtps = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  BJets = GetBJets(jets,jtps);

  std::sort(Leptons.begin(), Leptons.end(), PtComparingPtr);
  std::sort(jets.begin(), jets.end(), PtComparing);
  FillHist("CutFlow", 2., weight, 10, 0., 10.);
  

  // Targeting final state : lep,tau,j,j
  // Exact 1 lepton (e,mu) + additional lepton veto
  if(!(Leptons.size() == 1 && LeptonsVeto.size() == 1)) return;
  FillHist("CutFlow", 3., weight, 10, 0., 10.);
  if(HasFlag("debug")) cout << "[DEBUG] Cut 1" << endl;

  // 1 tau (will be matched with jets later)
  if(taus.size() != 1) return;
  FillHist("CutFlow", 4., weight, 10, 0., 10.);
  if(HasFlag("debug")) cout << "[DEBUG] Cut 2" << endl;

  if(jets.size() < 2) return;

  std::vector<double> dRjTau;
  for (unsigned int i = 0; i < jets.size(); i++){
    dRjTau.push_back(jets.at(i).DeltaR(taus.at(0)));
  }
  if(HasFlag("debug")) cout << "[DEBUG] getdRjTau" << endl;

  int minIndex = std::min_element(dRjTau.begin(),dRjTau.end()) - dRjTau.begin();
  if(HasFlag("debug")) cout << "[DEBUG] dRjTau " << dRjTau.size() << " " << minIndex << endl;

  // Check if there's tau matching jet
  if(dRjTau.at(minIndex) > 0.05) return;
  FillHist("CutFlow", 5., weight, 10, 0., 10.);
  if(HasFlag("debug")) cout << "[DEBUG] Cut 4" << endl;

  // OS Lepton, Tau pair
  if((Leptons.at(0)->Charge()*taus.at(0).Charge())>0) return;
  FillHist("CutFlow", 6., weight, 10, 0., 10.);
  if(HasFlag("debug")) cout << "[DEBUG] Cut 5" << endl;

  // remove TauJet, discard events with < 2 jets
  vector<Jet> jets_final = jets;
  jets_final.erase(jets_final.begin()+minIndex);
  if(jets_final.size() < 2) return;
  FillHist("CutFlow", 7., weight, 10, 0., 10.);

  for (unsigned int i = 0; i < jets_final.size(); i++)
  {
    FillHist("Jet/Jet"+to_string(i)+"/pT", jets_final.at(i).Pt(), weight, 500, 0., 500.);
    FillHist("Jet/Jet"+to_string(i)+"/Eta", jets_final.at(i).Eta(), weight, 60, -3., 3.);
    FillHist("Jet/Jet"+to_string(i)+"/Phi", jets_final.at(i).Phi(), weight, 60, -3., 3.);
    FillHist("Jet/Jet"+to_string(i)+"/dRjTau", jets_final.at(i).DeltaR(taus.at(0)), weight, 40, 0., 4.);
  }

  Jet TauJetCand = jets.at(minIndex);
  FillHist("Jet/dRjj", jets_final.at(0).DeltaR(jets_final.at(1)), weight, 40, 0., 4.);
  FillHist("Jet/TauJetCand/pT", TauJetCand.Pt(), weight, 500, 0., 500.);
  FillHist("Jet/TauJetCand/Eta", TauJetCand.Eta(), weight, 60, -3., 3.);
  FillHist("Jet/TauJetCand/Phi", TauJetCand.Phi(), weight, 60, -3., 3.);
  FillHist("Jet/TauJetCand/dRLepTauCand", TauJetCand.DeltaR(*Leptons.at(0)), weight, 40, 0., 4.);
  if(HasFlag("debug")) cout << "[DEBUG] FillTauJetCand" << endl;

  FillHist("Jet/nJets", jets_final.size(), 1, 10, 0., 10.);
  FillHist("Jet/nBJets", BJets.size(), 1, 10, 0., 10.);

  FillHist("Lepton/pT", Leptons.at(0)->Pt(), weight, 500, 0., 500.);
  FillHist("Lepton/eta", Leptons.at(0)->Eta(), weight, 60, -3., 3.);
  FillHist("Lepton/phi", Leptons.at(0)->Phi(), weight, 60, -3., 3.);

  FillHist("Lepton/dRltau", taus.at(0).DeltaR(*Leptons.at(0)), weight,40,0.,4.);

  FillHist("Tau/pT", taus.at(0).Pt(), weight, 500, 0., 500.);
  FillHist("Tau/eta", taus.at(0).Eta(), weight, 60, -3., 3.);
  FillHist("Tau/phi", taus.at(0).Phi(), weight, 60, -3., 3.);

  FillHist("MET",METv.Pt(), weight, 500, 0., 500.);  

}

LepTauJJ::LepTauJJ(){

}

LepTauJJ::~LepTauJJ(){

}


