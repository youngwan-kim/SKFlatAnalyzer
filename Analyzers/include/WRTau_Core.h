#ifndef WRTau_Core_h
#define WRTau_Core_h

#include "AnalyzerCore.h"
#include "TauIDSFTool.h"
#include <set>
#include <tuple>

class WRTau_Core : public AnalyzerCore {

public:

  WRTau_Core();
  ~WRTau_Core();

  std::string DeepTauVSjet = "DeepTau2017v2p1VSjet";
  std::string DeepTauVSe = "DeepTau2017v2p1VSe";
  std::string DeepTauVSmu = "DeepTau2017v2p1VSmu";

  // Boosted SR object tag
  FatJet fatjet_BoostedSR;
  vector<FatJet> fatjet_BoostedSR_v;

  // Cut Values
  double LSFOptCut = 0.6;
  double METCut = 100;
  double MTCut = 900;
  double MRecoCut = 1200;
  
  double TriggerSafeTauPtCut = 0.0;

  map<int,TString> idname_map = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  map<int,std::string> idname_map_str = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  // cache tauidsftool once during initialization
  map<std::tuple<int,int,int>,TauIDSFTool*> tauidsftool_map;
  map<std::tuple<int,int,int>,TauIDSFTool*> tauidsftool_vEl_map;
  map<std::tuple<int,int,int>,TauIDSFTool*> tauidsftool_vMu_map;

  vector<int> vJet_vec; vector<int> vEl_vec; vector<int> vMu_vec;

  TDirectory *histDir;
  TH1D *TauIDSFHist;

  enum TheRunEra{
    y2016B,y2016C,y2016D,y2016E,y2016F,y2016G,y2016H,
    y2017B,y2017C,y2017D,y2017E,y2017F,
    y2018A,y2018B,y2018C,y2018D,
    y2016MC,
    y2017MC,
    y2018MC,
    yUL2016B,yUL2016C,yUL2016D,yUL2016E,yUL2016F,yUL2016Flate,yUL2016G,yUL2016H,
    yUL2017B,yUL2017C,yUL2017D,yUL2017E,yUL2017F,
    yUL2018A,yUL2018B,yUL2018C,yUL2018D,
    yUL2016MCAPV,
    yUL2016MCnonAPV,
    yUL2017MC,
    yUL2018MC
  };

  // vars
  enum Channel{
    E=0,
    EE=1,
    EEE=2,
    EEEE=3,
    Mu=10,
    MuMu=11,
    MuMuMu=12,
    MuMuMuMu=13,
    EMu=15,
    EMuL=16,
    EMuLL=17,
    LL=18,

    TauE=20,
    TauMu=21
  };

  enum SearchRegion{
    None,
    EXO_16_023_Preselection,
    BaselinePreselection,
    ResolvedPreselection,
    BoostedPreselection,
    ResolvedLowMassControlRegion,
    BoostedLowMassControlRegion,
    ResolvedLowMassControlRegionMass1,
    BoostedLowMassControlRegionMass1,
    ResolvedSignalRegion,
    BoostedSignalRegion,
    BoostedSignalRegionLSFInvert,       // Muon Fake Boosted Measurement Region
    ResolvedSignalRegionMETInvert,      // Tau Fake Resolved Measurement Region
    BoostedSignalRegionMETInvert,       // Tau Fake Boosted Measurement Region
    ResolvedSignalRegionMass1,
    BoostedSignalRegionMass1,
    WJetsControlRegion,
    BoostedMassOptSel,
    ResolvedMassOptSel,
    QCDEnrichedControlRegionAK4,
    QCDEnrichedControlRegionAK8,
    FakeTTControlRegion,
    FakeDYControlRegion,
    BenchmarkResolvedPreselection,
    BenchmarkBoostedPreselection,
    BoostedSignalRegionMETInvertMTSame,
    ResolvedSignalRegionMETInvertMTSame,
    TTEnrichedRegion,
    TTFakeMeasureRegion,

    //Generator level regions ()
    GenDebug                    = 100,
    GenTauHTauH                 = 101,
    GenTauHTauEl                = 102,
    GenTauHTauMu                = 103,
    GenTauElTauH                = 104,
    GenTauMuTauH                = 105,
    GenTauLTauL                 = 106,
    GenResolvedElSignalRegion   = 107,
    GenResolvedMuSignalRegion   = 108,
    GenBoostedElSignalRegion    = 109,
    GenBoostedMuSignalRegion    = 110,
  };


  // RecoSF Syst
  int Syst_MuonRecoSF = 0;
  int Syst_ElectronRecoSF = 0;
  // IDSF Syst
  int Syst_MuonIDSF = 0;
  int Syst_ElectronIDSF = 0;

  int Syst_MuonISOSF = 0;

  std::string Syst_TauIDSF = "";
  int Syst_TauTriggerSF = 0;
  int Syst_LSFSF = 0;
  int Syst_PU = 0;
  int Syst_Prefire = 0;
  int Syst_TauES = 0;

  int TauFRErr = 0;

  vector<WRTau_Core::SearchRegion> BoostedRegions = {WRTau_Core::BoostedLowMassControlRegionMass1,
                                                     WRTau_Core::BoostedLowMassControlRegion,
                                                     WRTau_Core::BoostedMassOptSel,
                                                     WRTau_Core::BoostedSignalRegion,
                                                     WRTau_Core::BoostedSignalRegionMass1,
                                                     WRTau_Core::BoostedSignalRegionMETInvert,
                                                     WRTau_Core::BoostedSignalRegionMETInvertMTSame,
                                                     WRTau_Core::BenchmarkBoostedPreselection};

  vector<WRTau_Core::SearchRegion> ResolvedRegions = {WRTau_Core::ResolvedLowMassControlRegionMass1,
                                                      WRTau_Core::ResolvedLowMassControlRegion,
                                                      WRTau_Core::ResolvedMassOptSel,
                                                      WRTau_Core::ResolvedSignalRegion,
                                                      WRTau_Core::ResolvedSignalRegionMass1,
                                                      WRTau_Core::ResolvedSignalRegionMETInvert,
                                                      WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                                                      WRTau_Core::BenchmarkResolvedPreselection};


  vector<WRTau_Core::SearchRegion> MassOptRegions = {WRTau_Core::BoostedMassOptSel,
                                                     WRTau_Core::ResolvedMassOptSel};

  vector<WRTau_Core::SearchRegion> FakeMeasurementRegion = {WRTau_Core::BoostedSignalRegionMETInvertMTSame,
                                                            WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                                                            WRTau_Core::TTFakeMeasureRegion};

  vector<WRTau_Core::SearchRegion> FakeMeasurementRegion_Lepton = {WRTau_Core::BoostedSignalRegionLSFInvert,
                                                                  };

  


  // misc
  bool isSignalSample();

  // Regions
  const inline bool isPreselection(const std::vector<Tau>& taus){ return (taus.size()>0); };
  const inline bool hasAtLeast2AK4Jets(const std::vector<Jet>& jets){ return (jets.size()>1); };
  const inline bool hasAtLeast1AK8Jets(const std::vector<FatJet>& fatjets){ return (fatjets.size()>0); };
  const inline bool hasAtLeast1Leptons(const std::vector<Lepton *> TightLeptons){ return (TightLeptons.size()>0); };

  bool isBoostedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);

  bool isBoostedPreselectionTest(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);
  
  bool isResolvedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);

  // Objects 
  double GetTauIDSF(TString vsJetWP, TString vsEleWP, int DM, double pt,bool GetFromDM=false);
  std::vector<Jet> VetoTauFromJets(const std::vector<Jet>& jets, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<FatJet> VetoTauFromFatJets(const std::vector<FatJet>& fatjets, const std::vector<Tau>& taus, double dR=0.8);
  std::vector<Lepton*> VetoTauFromLeptons(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<Tau> VetoLeptonsFromTaus(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<Electron> ElectronPromptOnly_Tau(const std::vector<Electron>& electrons, const std::vector<Gen>& gens);
  std::vector<Electron> ElectronNonPromptOnly_Tau(const std::vector<Electron>& electrons, const std::vector<Gen>& gens);
  std::vector<Muon> MuonPromptOnly_Tau(const std::vector<Muon>& muons, const std::vector<Gen>& gens);
  std::vector<Muon> MuonNonPromptOnly_Tau(const std::vector<Muon>& muons, const std::vector<Gen>& gens);

  // Select Objects (syst. incl)
  

  // MET
  Particle GetvMET(TString METType);
  Particle GetvMET(TString METType, AnalyzerParameter param);
  Particle GetvMET(TString METType, AnalyzerParameter param,bool PropSmearing);
  Particle GetvMET(TString METType, AnalyzerParameter param,
                   std::vector<Jet> jets, std::vector<FatJet> fatjets,
                   std::vector<Muon> muons, std::vector<Electron> electrons, bool PropSmearing);
  std::pair<double,double> METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL =false,bool ispuppi=false);
  Particle GetvCorrMET(TString METType, AnalyzerParameter param, Particle METUncorr);
  map<TString, Particle> METMap( AnalyzerParameter param);
  Particle GetMiniAODvMET(TString METType);

  // Truth Matching
  LHE GetClosestTauLHE(const Tau tau, const std::vector<LHE>& LHEs);
  Gen GetClosestTauGen(const Tau tau, const std::vector<Gen>& gens);
  std::map<WRTau_Core::SearchRegion,bool> getGenLevelChannelMap(const std::vector<Gen>& gens);
  void FillClosestTauGen(const Tau tau, const std::vector<Gen>& gens, TString region,double weight);
  void FillClosestTauLHE(const Tau tau, const std::vector<LHE>& LHEs, TString region,double weight);
  int GetTauType(const Tau tau, const std::vector<Gen>& gens);
  int GetTauType_Public(int TruthIdx, const std::vector<Gen>& TruthColl);
  vector<int> GetTauAncestor(const Gen& me, const std::vector<Gen>& gens);
  vector<Tau> TauPromptOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens);
  vector<Tau> TauFakeOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens);
  vector<Tau> TauErrorOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens);
  bool IsNonPromptTau(const Tau tau, const std::vector<Gen>& gens);
  bool IsPromptTau(const Tau tau, const std::vector<Gen>& gens);
  Gen GetClosestGenJet(const std::vector<Gen>& gens, const Jet jet);
  Gen GetClosestGenJet(const std::vector<Gen>& gens, const FatJet jet);
  Gen GetStableTauDaughter(const Gen me,const std::vector<Gen>& gens);
  std::pair<Gen,Gen> GetSignalGenTaus(const std::vector<Gen>& gens);
  int GetDaughterIndex(Gen me,const std::vector<Gen>& gens);
  std::vector<int> GetDaughterIndexVector(Gen me,const std::vector<Gen>& gens);
  //Gen GetSignalGenLepton(const std::vector<Gen>& gens);
  //Gen GetSignalGenJets(const std::vector<Gen>& gens);

  // Variables
  double GetST(std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Tau> taus, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle METv);
  double GetST(std::vector<Lepton *> leptons,  std::vector<Tau> taus, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle METv);

  std::pair<Particle,Particle> GetNeutrinos(const Particle METv,const Particle p1,const Particle p2);

  // Filling basic histograms after preselection (requiring >1 hadronic tau with pT>190 GeV)
  WRTau_Core::Channel GetChannel(const std::vector<Lepton *> leps);
  TString GetChannelString(WRTau_Core::Channel channel);
  std::string GetRegionString(WRTau_Core::SearchRegion region);
  map<WRTau_Core::SearchRegion,bool> GetRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                                               const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons);
  map<WRTau_Core::SearchRegion,bool> GetGenRegion(const std::vector<Gen>& gens);

  double GetResolvedSRMass(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons,bool ignoreMET);
  double GetBoostedSRMass(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons,bool ignoreMET);
  double GetBoostedSRMass(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedSRMass(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  
  double GetBoostedST(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedST(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  
  double GetBoostedLT(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedLT(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  
  double GetBoostedHT(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedHT(const std::vector<Tau>& taus,const std::vector<Jet>& jets,const std::vector<Lepton *> TightLeptons);
  
  double GetBoostedSTwithMET(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedSTwithMET(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);

  double GetBoostedDeltaPhiLepTau(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedDeltaPhiLepTau(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  
  double GetBoostedDeltaPhiLepJets(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedDeltaPhiLepJets(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  
  double GetResolvedSRMass_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  double GetBoostedSRMass_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  double GetResolvedSRMassN_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons);
  double GetBoostedSRMassN_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons);
  
  //map<WRTau_Core::SearchRegion,bool> GetRegion(Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
  //                                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons); // TODO Implement getregion with gen level version
  void FillPassingRegions_XsecVar(map<WRTau_Core::SearchRegion,bool> m_region,WRTau_Core::SearchRegion r,Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight,std::tuple<int,int,int> idtuple, bool highpT);
  void FillPassingRegions_2DScan(map<WRTau_Core::SearchRegion,bool> m_region,WRTau_Core::SearchRegion r,Particle METv,const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight, std::tuple<int,int,int> idtuple, bool highpT);
  void FillPassingRegions(map<WRTau_Core::SearchRegion,bool> m_region,Particle METv,const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight, std::tuple<int,int,int> idtuple, bool highpT);
  void FillPassingRegions(map<pair<WRTau_Core::SearchRegion,double>, bool> m_Cut, TString cutvar ,Particle METv,const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight, std::tuple<int,int,int> idtuple, bool highpT);
  void FillPassingRegions(map<WRTau_Core::SearchRegion,bool> m_region,WRTau_Core::SearchRegion r,Particle METv,const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight, std::tuple<int,int,int> idtuple, bool highpT);
  void FillPreselHists(TString region,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight); 
  void FillMassHists(TString region, const Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight);
  map<pair<WRTau_Core::SearchRegion,double>, bool> LSFCutter(map<WRTau_Core::SearchRegion,bool> m_region,vector<double> LSFCuts,std::vector<FatJet> fatjets);
  map<pair<WRTau_Core::SearchRegion,double>, bool> MassCutter(map<WRTau_Core::SearchRegion,bool> m_region,vector<double> MassCuts,
                                                              Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,
                                                              const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons, bool ignoreMET);
  map<pair<WRTau_Core::SearchRegion,double>, bool> MassCutter(map<WRTau_Core::SearchRegion,bool> m_region,vector<double> MassCuts,
                                                              Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,
                                                              const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons);                                                              
  std::vector<Lepton *> ChooseLeptonColl(WRTau_Core::SearchRegion region, std::pair<std::vector<Lepton *>,std::vector<Lepton *>> LeptonPair);
  bool isBoostedRegion(WRTau_Core::SearchRegion region);
  bool isResolvedRegion(WRTau_Core::SearchRegion region);
  void CopyHist(TString histname0, TString histname1);

  // weights
  double SetupWeight(Event ev, AnalyzerParameter param);
  void GetTauIDSFTools(const std::vector<int> vJet_vec,const std::vector<int> vEl_vec,const std::vector<int> vMu_vec);
  double GetMatchedWeight(const std::vector<Tau>& taus,const std::vector<Gen>& gens,const std::vector<Lepton *> leps, std::tuple<int,int,int> idtuple, bool highpT);
  double GetMatchedWeight(const std::vector<Tau>& taus,const std::vector<Gen>& gens, std::tuple<int,int,int> idtuple, bool highpT);
  double GetTauIDLeptonFakeSF(const std::tuple<int,int,int> idtuple, const std::vector<Lepton*> leps, const std::vector<Gen>& gens);
  double GetTauTriggerSF(const int syst);
  double LSFSF(WRTau_Core::Channel ch,const int syst); 

  // Fake Rates
  double GetTauFR(const Tau tau, WRTau_Core::SearchRegion region);
  double GetTauFRfromBins(const Tau tau, const std::vector<Lepton *> leps, WRTau_Core::SearchRegion region);
  double GetTauFR(const Tau tau, const std::vector<Lepton *> leps, WRTau_Core::SearchRegion region, const int deg);
  double GetTauFRWeight(const Tau tau, const std::vector<Lepton *> leps, const std::vector<Gen>& gens, WRTau_Core::SearchRegion region,const int deg);
  double GetElTauFRWeight(const Tau tau, const Electron el,const std::vector<Gen>& gens, WRTau_Core::SearchRegion region);

  double GetTauFF_QCD(const Tau tau);
  double GetTauFF_TT(const Tau tau);
  double GetTauFF(const Tau tau, WRTau_Core::SearchRegion region);

  double GetTauFR_SingleFit(const Tau tau, const std::vector<Lepton *> leps, WRTau_Core::SearchRegion region);

  // Others
  TDirectory* GetTempDir_WRTauCore();

};



#endif

