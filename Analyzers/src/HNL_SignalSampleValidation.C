#include "HNL_SignalSampleValidation.h"

void HNL_SignalSampleValidation::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}

void HNL_SignalSampleValidation::executeEvent(){

  FillHist("CutFlow",0,1,10,0.,10.,"");

  if(IsDATA) return;
  
  AnalyzerParameter param = InitialiseHNLParameter("SignalStudy","UL");

  executeEventFromParameter(param);

}

void HNL_SignalSampleValidation::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;
  cout << "[DEBUG] PassMETFilter" << endl;

  FillHist("CutFlow",1,1,10,0.,10.,"METFilter");

  double weight(1.);
  Event ev = GetEvent();

  double this_mc_weight = ev.GetTriggerLumi("Full") * MCweight(true, true) * GetKFactor();
  weight *= this_mc_weight;
  FillWeightHist("MCWeight_" ,MCweight(true, true));
  FillWeightHist("MCFullWeight_" , this_mc_weight);
   
  // Leptons 
  std::vector<Electron> ElectronColl = GetElectrons(param.Electron_Tight_ID,10.,2.5);
  std::vector<Muon> MuonColl = GetMuons(param.Muon_Tight_ID,5.,2.4);
  std::vector<Lepton *> LeptonColl = MakeLeptonPointerVector(MuonColl,ElectronColl);
  std::sort(LeptonColl.begin(),LeptonColl.end(),PtComparingPtr);

  // Veto Leptons
  std::vector<Electron> ElectronCollV = GetElectrons(param.Electron_Veto_ID,10.,2.5);
  std::vector<Muon> MuonCollV = GetMuons(param.Muon_Veto_ID,5.,2.4);
  std::vector<Lepton *> LeptonCollV = MakeLeptonPointerVector(MuonCollV,ElectronCollV);
  std::sort(LeptonCollV.begin(),LeptonCollV.end(),PtComparingPtr);

  // Jets
  vector<FatJet> allfatjets = puppiCorr->Correct(GetAllFatJets());
  std::vector<Jet> jets_tmp = GetJets(param.Jet_ID,15.,5.);
  std::vector<FatJet> fatjets_tmp = SelectFatJets(allfatjets,param.FatJet_ID,200,5.);

  std::vector<FatJet> FatJetColl = GetAK8Jets(fatjets_tmp,200.,2.7,true,1.,true,-999,true,40.,130.,ElectronCollV,MuonCollV);
  std::vector<FatJet> FatJetColl_notag = GetAK8Jets(fatjets_tmp,200.,2.7,true,1.,false,-999,true,40.,130.,ElectronCollV,MuonCollV);
  std::vector<Jet> JetColl = GetAK4Jets(jets_tmp,20.,2.7,true,0.4,0.8,"loose",ElectronCollV,MuonCollV,FatJetColl);
  std::sort(JetColl.begin(),JetColl.end(),PtComparing);
  std::sort(FatJetColl.begin(),FatJetColl.end(),PtComparing);
  int njets = JetColl.size(); int nfatjets = FatJetColl.size();

  // B-Jets & VBF Jets
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl = GetBJets(param,jets_tmp,20.,2.4, false,0.4,0.8,"loose",ElectronCollV,MuonCollV, FatJetColl, param_jets);
  std::vector<Jet> VBFJetColl = GetAK4Jets(jets_tmp,30.,4.7,true,0.4,0.8,"loose",ElectronCollV,MuonCollV,FatJetColl);
  std::sort(BJetColl.begin(),BJetColl.end(),PtComparing);
  std::sort(VBFJetColl.begin(),VBFJetColl.end(),PtComparing);
  int nbjets = BJetColl.size(); int nvbfjets = VBFJetColl.size();

  // Gen
  vector<Gen> gens = GetGens();
  vector<Gen> gen_lep = GetGenLepronsSignal();
  std::sort(gen_lep.begin(),gen_lep.end(),PtComparing);
  Gen N;

  for(int i=0;i<2;i++){
    if(gens.at(gen_lep.at(i).MotherIndex()).PID() == 9900012 || gens.at(gen_lep.at(i).MotherIndex()).PID() == 9900014 ){
      N = gens.at(gen_lep.at(i).MotherIndex());
    }
  }

  // Get Process 
  TString process = "";
  process = GetProcess();

  TObjArray *tokens = process.Tokenize("_");
  TString chargetype_string = ((TObjString *) tokens -> At(0)) -> GetString();
  TString channel_string = ((TObjString *) tokens -> At(1)) -> GetString();
 
  /*HNL_LeptonCore::ChargeType q;
  if(chargetype_string=="SS") q=SS;
  if(chargetype_string=="OS") q=OS;*/

  HNL_LeptonCore::Channel channel;
  if(channel_string=="El+El+"||channel_string=="El-El-"||channel_string=="ElEl") channel=EE;
  if(channel_string=="Mu+Mu+"||channel_string=="Mu-Mu-"||channel_string=="MuMu") channel=MuMu;
  if(channel_string=="ElMu"||channel_string=="El+Mu-"||channel_string=="El-Mu+") channel=EMu;
  if(channel_string=="MuEl"||channel_string=="Mu+El-"||channel_string=="Mu-El+") channel=EMu;

  //TString channel_str = GetChannelString(channel,q);

  // METv
  Particle METUnsmearedv = GetvMET("T1xyCorr",param);
  Particle METv = UpdateMETSmearedJet(METUnsmearedv,JetColl);

  // Preselection
  //if(!PassPreselection(channel,q,LeptonColl,LeptonCollV,JetColl,VBFJetColl,FatJetColl,BJetColl,ev,METv,param,channel_str,weight)) return;

  if(chargetype_string != "SS") return;
  FillHist("CutFlow",2,1,10,0.,10.,"SS Cut");

  //FillHist("Reco/lep0_beforetrigger_pt",LeptonColl.at(0)->Pt(),weight, 500, 0., 500., "lepton0 p_{T} GeV");
  //FillHist("Reco/lep1_beforetrigger_pt",LeptonColl.at(1)->Pt(),weight, 500, 0., 500., "lepton1 p_{T} GeV");

  if (!PassTriggerSelection(channel, ev, LeptonColl,"Dilep")) return;
  FillHist("CutFlow",3,1,10,0.,10.,"Trigger Selection");  

  if (LeptonColl.size() != 2) return;
  if (LeptonCollV.size() != 2) return;
  FillHist("CutFlow",4,1,10,0.,10.,"Lep3Veto");
 
  cout << "[DEBUG] Lep3Veto Cut " <<  endl;

  if(DataEra=="2017" || DataEra=="2018"){
      if(LeptonColl.at(1)->Pt() < 15)  return;
  }
  FillHist("CutFlow",5,1,10,0.,10.,"");

  if (channel==EE     && !(LeptonColl.at(0)->LeptonFlavour() == Lepton::ELECTRON && LeptonColl.at(1)->LeptonFlavour() == Lepton::ELECTRON)) return;
  if (channel==MuMu   && !(LeptonColl.at(0)->LeptonFlavour() == Lepton::MUON     && LeptonColl.at(1)->LeptonFlavour() == Lepton::MUON))    return;
  if (channel==EMu  &&
      !( (LeptonColl.at(0)->LeptonFlavour() == Lepton::ELECTRON && LeptonColl.at(1)->LeptonFlavour() == Lepton::MUON) ||
         (LeptonColl.at(0)->LeptonFlavour() == Lepton::MUON && LeptonColl.at(1)->LeptonFlavour() == Lepton::ELECTRON) ))  return;
  FillHist("CutFlow",6,1,10,0.,10.,"LeptonCheck");

  Particle ll =  (*LeptonColl.at(0)) + (*LeptonColl.at(1));
  // VETO Z PEAK IN EE CHANNEL
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return;
  FillHist("CutFlow",7,1,10,0.,10.,"mZ");

  if(ll.M() < 20) return; // TO_CHECK: IS 20 BEST OPTION
  FillHist("CutFlow",8,1,10,0.,10.,"mll");

  int nalljets = JetColl.size() + FatJetColl.size() + VBFJetColl.size();
  if(nalljets == 0) return;
  FillHist("CutFlow",9,1,10,0.,10.,"JetSize");

  FillHist("Gen/lep0_pt",gen_lep.at(0).Pt(),weight, 500, 0., 1000., "lepton0 p_{T} GeV");
  FillHist("Gen/lep1_pt",gen_lep.at(1).Pt(),weight, 500, 0., 1000., "lepton1 p_{T} GeV");
  FillHist("Reco/lep0_pt",LeptonColl.at(0)->Pt(),weight, 500, 0., 1000., "lepton0 p_{T} GeV");
  FillHist("Reco/lep1_pt",LeptonColl.at(1)->Pt(),weight, 500, 0., 1000., "lepton1 p_{T} GeV");


  FillHist("Gen/lep0_eta",gen_lep.at(0).Eta(),weight,60, -3., 3.,"lepton0 #eta"); 
  FillHist("Gen/lep1_eta",gen_lep.at(1).Eta(),weight,60, -3., 3.,"lepton1 #eta");

  FillHist("Gen/mN",N.M(),weight,3000,0.,3000.,"m(N) GeV");

  FillHist("Reco/lep0_eta",LeptonColl.at(0)->Eta(),weight,60, -3., 3.,"lepton0 #eta"); 
  FillHist("Reco/lep1_eta",LeptonColl.at(1)->Eta(),weight,60, -3., 3.,"lepton1 #eta");

  for(int i=0 ; i < njets ; i++){
    FillHist("Reco/jet"+std::to_string(i)+"_pt",JetColl.at(i).Pt(),weight,500, 0., 1000.,"jet"+std::to_string(i)+" p_{T} GeV");
    FillHist("Reco/jet"+std::to_string(i)+"_eta",JetColl.at(i).Eta(),weight,60, -3., 3.,"jet"+std::to_string(i)+" #eta");
  }
  FillHist("Reco/Njets",njets,1.,10,0,10,"NJets");
  for(int i=0 ; i < nbjets ; i++){
    FillHist("Reco/bjet"+std::to_string(i)+"_pt",BJetColl.at(i).Pt(),weight,500, 0., 1000.,"bjet"+std::to_string(i)+" p_{T} GeV");
    FillHist("Reco/bjet"+std::to_string(i)+"_eta",BJetColl.at(i).Eta(),weight,60, -3., 3.,"bjet"+std::to_string(i)+" #eta");
  }
  FillHist("Reco/Nbjets",nbjets,1.,10,0,10,"NBJets");
  for(int i=0 ; i < nfatjets ; i++){
    FillHist("Reco/fatjet"+std::to_string(i)+"_pt",FatJetColl.at(i).Pt(),weight,500, 0., 1000.,"fatjet"+std::to_string(i)+" p_{T} GeV");
    FillHist("Reco/fatjet"+std::to_string(i)+"_eta",FatJetColl.at(i).Eta(),weight,60, -3., 3.,"fatjet"+std::to_string(i)+" #eta");
  }
  FillHist("Reco/NFatjets",nfatjets,1.,10,0,10,"NFatJets");
  FillHist("Reco/METv_Pt",METv.Pt(),weight,500,0.,1000.,"MET");
}

HNL_SignalSampleValidation::HNL_SignalSampleValidation(){

}

HNL_SignalSampleValidation::~HNL_SignalSampleValidation(){

}


