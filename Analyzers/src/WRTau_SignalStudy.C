#include "WRTau_SignalStudy.h"

void WRTau_SignalStudy::initializeAnalyzer(){

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {3,4,5}; vEl_vec = {9,13}; vMu_vec = {18,21};
  

  for(const auto &vjet : vJet_vec){
    for(const auto &vel : vEl_vec){
      for(const auto &vmu : vMu_vec){
        std::tuple<int,int,int> idtuple = std::make_tuple(vjet,vel,vmu);
        tauidsftool_map[idtuple] = new TauIDSFTool("UL"+std::to_string(DataYear),DeepTauVSjet,idname_map_str[vjet]);
      }
    }
  }

  if(DataYear==2017){

    SingleElectronTriggers = {"HLT_Ele35_WPTight_Gsf_v"};
    SingleMuonTriggers = {"HLT_Mu50_v"};
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    SingleLeptonTriggers = {"HLT_Mu27_v","HLT_Ele35_WPTight_Gsf_v"};

  }

}

void WRTau_SignalStudy::executeEvent(){


  AnalyzerParameter param;
  param.Clear();

  TriggerList.clear();
  TriggerList = SingleTauTriggers;

  param.Name = "WRTauSignalStudy";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";
  param.Electron_ID_SF_Key = "HEEP";

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGLoose";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_TrackerMuons";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";

  param.Jet_ID = "tightLepVeto";
  param.FatJet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();
  AllGens = GetGens();
  AllLHEs = GetLHEs();

  executeEventFromParameter(param);

}

void WRTau_SignalStudy::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  std::pair<Gen,Gen> gentaus = GetSignalGenTaus(AllGens);
  cout << "RunNumber:EventNumber = " << run << ":" << event << endl;
  cout << "(" << gentaus.first.PID() << " , " << gentaus.second.PID() << ")" << endl;

  int test(0);
  if(fabs(gentaus.first.PID())==15 && fabs(gentaus.second.PID())==15) test = 1;
  FillHist(param.Name+"/DiTau",test,1.,2,0.,2.);

  FillHist(param.Name+"/Cutflow",0.,1.,10,0.,10.);
  
  if(!ev.PassTrigger(TriggerList)) return;
  FillHist(param.Name+"/Cutflow",1.,1.,10,0.,10.);
  FillHist(param.Name+"/Ditau_trg",test,1.,2,0.,2.);



  

}

WRTau_SignalStudy::WRTau_SignalStudy(){

}

WRTau_SignalStudy::~WRTau_SignalStudy(){

}


