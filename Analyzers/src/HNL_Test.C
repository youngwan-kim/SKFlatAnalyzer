#include "HNL_Test.h"

void HNL_Test::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}

void HNL_Test::executeEvent(){

  FillHist("CutFlow",0,1,2,0.,2.,"");

  if(IsDATA) return; // return if data (will only run for MC)
  
  //AnalyzerParameter param = InitialiseHNLParameter("SignalStudy");

  //double weight(1.);

  //executeEventFromParameter(param);

}

void HNL_Test::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  /*double weight(1.);
  Event ev = GetEvent();

  double this_mc_weight = ev.GetTriggerLumi("Full") * MCweight(true, true) * GetKFactor();
  weight     *=  this_mc_weight;
  FillWeightHist("MCWeight_" ,MCweight(true, true));
  FillWeightHist("MCFullWeight_" , this_mc_weight);
   
  // Leptons 
  std::vector<Electron> ElectronColl = GetElectrons(param.Electron_Tight_ID,10.,2.5);
  std::vector<Muon> MuonColl = GetMuons(param.Muon_Tight_ID,5.,2.4);
  std::vector<Lepton *> LeptonColl = MakeLeptonPointerVector(MuonColl,ElectronColl);
  
  // Veto Leptons
  std::vector<Electron> ElectronCollV = GetElectrons(param.Electron_Veto_ID,10.,2.5);
  std::vector<Muon> MuonCollV = GetMuons(param.Muon_Veto_ID,5.,2.4);
  std::vector<Lepton *> LeptonCollV = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

  // Jets
  vector<FatJet> allfatjets = puppiCorr->Correct(GetAllFatJets());
  std::vector<Jet> jets_tmp = GetJets(param.Jet_ID,15.,5.);
  std::vector<FatJet> fatjets_tmp = SelectFatJets(allfatjets,param.FatJet_ID,200,5.);

  std::vector<FatJet> FatJetColl = GetAK8Jets(fatjets_tmp,200.,2.7,true,1.,true,-999,true,40.,130.,ElectronCollV,MuonCollV);
  std::vector<FatJet> FatJetColl_notag = GetAK8Jets(fatjets_tmp,200.,2.7,true,1.,false,-999,true,40.,130.,ElectronCollV,MuonCollV);
  std::vector<Jet> JetColl = GetAK4Jets(jets_tmp,20.,2.7,true,0.4,0.8,"loose",ElectronCollV,MuonCollV,FatJetColl);

  // B-Tagging
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl = GetBJets(jets_tmp,20.,2.5,false,0.4,0.8,"loose",ElectronCollV,MuonCollV,FatJetColl,param_jets);
  std::vector<Jet> VBFJetColl = GetAK4Jets(jets_tmp,30.,4.7,true,0.4,0.8,"loose",ElectronCollV,MuonCollV,FatJetColl);

  // Gen
  vector<Gen> gens = GetGens();
  TString process = "";
  process = GetProcess();

  TObjArray *tokens = process.Tokenize("_");
  TString chargetype_string = ((TObjString *) tokens -> At(0)) -> GetString();
  TString channel_string = ((TObjString *) tokens -> At(1)) -> GetString();
 
  HNL_LeptonCore::ChargeType q;
  if(chargetype_string=="SS") q=SS;
  if(chargetype_string=="OS") q=OS;

  HNL_LeptonCore::Channel channel;
  if(channel_string=="El+El+"||channel_string=="El-El_"||channel_string=="ElEl") channel=EE;
  if(channel_string=="Mu+Mu+"||channel_string=="Mu-Mu-"||channel_string=="MuMu") channel=MuMu;
  if(channel_string=="ElMu"||channel_string=="El+Mu-"||channel_string=="El-Mu+") channel=EMu;
  if(channel_string=="MuEl"||channel_string=="Mu+El-"||channel_string=="Mu-El+") channel=MuE;

  TString channel_str = GetChannelString(channel,q);

  // METv
  Particle METUnsmearedv = GetvMET("T1xyCorr","");
  Particle METv = UpdateMETSmearedJet(METUnsmearedv,JetColl);
  
  //if(!PassPreselection(channel,q,LeptonColl,LeptonCollV,JetColl,VBFJetColl,FatJetColl,BJetColl,ev,METv,param,channel_str,weight)) return;
  
  FillHist("CutFlow",1,1,2,0.,2.,"");*/

}

HNL_Test::HNL_Test(){

}

HNL_Test::~HNL_Test(){

}


