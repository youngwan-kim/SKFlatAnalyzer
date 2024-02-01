#include "WRTau_LeptonFake.h"

void WRTau_LeptonFake::initializeAnalyzer(){

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {5}; vEl_vec = {13}; vMu_vec = {21};

  GetTauIDSFTools(vJet_vec,vEl_vec,vMu_vec);

  if(DataYear==2017){

    SingleElectronTriggers = {"HLT_Ele35_WPTight_Gsf_v"};
    SingleMuonTriggers = {"HLT_Mu50_v"};
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    SingleLeptonTriggers = {"HLT_Mu27_v","HLT_Ele35_WPTight_Gsf_v"};

  }

}

void WRTau_LeptonFake::executeEvent(){

 AnalyzerParameter param;

  param.Clear();
  TriggerList.clear();
  
  TriggerList = SingleLeptonTriggers;

  param.Name = "WRTauFake";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "LRSMLooseNoIso";
  param.Electron_Veto_ID = "LRSMVetoNoIso";
  param.Electron_ID_SF_Key = "HEEP";

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPtWithTrkIso";
  param.Muon_Veto_ID = "POGHighPt";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_TrackerMuons";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";

  param.Jet_ID = "tightLepVeto";
  param.FatJet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllGens = GetGens();
  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();

  executeEventFromParameter(param);

}

void WRTau_LeptonFake::executeEventFromParameter(AnalyzerParameter param){

  TriggerSafeTauPtCut = 190.;

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  tauid_LTT = make_tuple(3,13,21);
  tauid_TTT = make_tuple(5,13,21);

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else if(HasFlag("genWeight")) weight *= MCweight(true,false);
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Gen> this_AllGens = AllGens;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;
  vector<Muon> this_AllMuons_tmp = AllMuons;
  vector<Muon> this_AllMuons = UseTunePMuon(this_AllMuons_tmp);
  vector<Electron> this_AllElectrons = AllElectrons;

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::comb);

  vector<Muon> muons_raw           = SelectMuons(this_AllMuons, "NOCUT", 50., 2.4) ;
  vector<Muon> muons               = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.4) ;
  vector<Muon> muons_veto          = SelectMuons(this_AllMuons, param.Muon_Veto_ID,  50., 2.4) ;
  vector<Muon> muons_loose         = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.4) ;

  vector<Electron> electrons_raw   = SelectElectrons(this_AllElectrons, "NOCUT", 50. , 2.4);
  vector<Electron> electrons       = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.4);
  vector<Electron> electrons_veto  = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID,  50., 2.4);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 50., 2.4);
  
  vector<Lepton *> VetoLeps        = CombineLeptonPointerVector(electrons_veto, muons_veto);
  //vector<Lepton *> leptons_raw     = CombineLeptonPointerVector(electrons_raw, muons_raw);

  vector<Lepton *> LooseLeptons    = CombineLeptonPointerVector(electrons_loose,muons_loose);
  vector<Lepton *> TightLeptons    = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeptons     = CombineLeptonPointerVector(electrons_veto,muons_veto);

  vector<Tau> this_AllTaus = AllTaus;
  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);
  vector<Tau> taus         =  SelectTaus_varWP(taus_lepVeto,vJet_vec[0],vEl_vec[0],vMu_vec[0],50,2.4);

  vector<Jet>    jets_tauVeto         = VetoTauFromJets(this_AllJets,taus); 
  vector<FatJet> fatjets_tmp          = VetoTauFromFatJets(this_AllFatJets,taus);
  vector<Jet>    jets_lepVeto_tauVeto = JetsVetoLeptonInside(jets_tauVeto,electrons_veto,muons_veto,0.4);

  vector<Jet> jets = SelectJets(jets_lepVeto_tauVeto, param.Jet_ID, 40., 2.4);
  vector<Jet> bjets = SelectBJets(jets,param_jetsM);
  vector<FatJet> fatjets = SelectFatJets(fatjets_tmp,param.FatJet_ID,200.,2.4);

  std::sort(taus.begin(),taus.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing);
  std::sort(fatjets.begin(),fatjets.end(),PtComparing);
  std::sort(LooseLeptons.begin(),LooseLeptons.end(),PtComparingPtr);
  std::sort(TightLeptons.begin(),TightLeptons.end(),PtComparingPtr);
  std::sort(electrons.begin(),electrons.end(),PtComparing);
  std::sort(muons.begin(),muons.end(),PtComparing);
  // loose leptons
  std::sort(electrons_loose.begin(),electrons_loose.end(),PtComparing);
  std::sort(muons_loose.begin(),muons_loose.end(),PtComparing);
  // veto leptons
  std::sort(electrons_veto.begin(),electrons_veto.end(),PtComparing);
  std::sort(muons_veto.begin(),muons_veto.end(),PtComparing);


  map<WRTau_Core::SearchRegion,bool> map_regions = GetRegion(METv,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons);
  FillLeptonKinematics(map_regions,param,METv,AllGens,taus,LooseLeptons,TightLeptons,electrons,electrons_loose,electrons_veto,muons,muons_loose,muons_veto,jets,bjets,fatjets,param.Name,weight);

}

void WRTau_LeptonFake::FillLeptonKinematics(map<WRTau_Core::SearchRegion,bool> map_regions, AnalyzerParameter param,
                                            Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, 
                                            const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                                            const std::vector<Electron>& electrons, const std::vector<Electron>& electrons_loose, const std::vector<Electron>& electrons_veto,
                                            const std::vector<Muon>& muons, const std::vector<Muon>& muons_loose, const std::vector<Muon>& muons_veto,
                                            const std::vector<Jet>& jets, const std::vector<Jet>& bjets, const std::vector<FatJet>& fatjets, TString fillpath, double weight){

  //double ptbins[23] = {0,53,60,70,80,90,100,125,150,175,200,250,300,350,400,450,500,600,700,800,900,1000};
  //double etabins[12] = {0.0,0.25,0.5,0.75,1.00,1.25,1.479,1.75,2.0,2.25,2.4};

  for(auto const& r : FakeMeasurementRegion_Lepton){ // loop over boosted/resolved

    if(map_regions[r] == true){

      std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
      vector<Lepton *> leptons = ChooseLeptonColl(r,PairVecLeps);

      WRTau_Core::Channel ch = GetChannel(leptons);
      
      bool isMu = ( ch == WRTau_Core::TauMu );
      bool isEl = ( ch == WRTau_Core::TauE  );

      TString genmatchTag = "";

      if(!IsDATA){
        if(IsPromptLepton(*leptons.at(0),gens))           genmatchTag = "Prompt";
        else if(IsNonPromptLepton(*leptons.at(0),gens))   genmatchTag = "Fake";
        else                                              genmatchTag = "Error";
      }
      else                                                genmatchTag = "Data"; 

      TString label_channel = fillpath+"/"+GetRegionString(r) + "_"+GetChannelString(ch) + "/"+ genmatchTag;
      
      std::vector<TString> fillstr = {label_channel};

      // lepton type discrimination
      if(isMu){ 

        cout << param.Muon_Veto_ID  << " veto muon " << muons_veto.size() << endl;
        cout << param.Muon_Loose_ID << " loose muon " << muons_loose.size() << endl;
        cout << param.Muon_Tight_ID << " tight muon " << muons.size() << endl;

        for(auto const& mu : muons_veto){

          double muPt = mu.Pt(); 
          double muAbsEta = fabs(mu.Eta());
          if(mu.Pt() > 1000.) muPt = 999.9;
          if(muAbsEta > 2.4)  muAbsEta = 2.399;

          bool isVeto  = mu.PassID(param.Muon_Veto_ID);            // Veto-to-Veto (debug)
          bool isLoose = isVeto && mu.PassID(param.Muon_Loose_ID); // Veto-to-Loose
          bool isTight = isVeto && mu.PassID(param.Muon_Tight_ID); // Veto-to-Tight

          if(isVeto)   FillHist(label_channel+"/LeptonPtAbsEta_Veto",muPt,muAbsEta,weight,100,0.,1000.,24,0.0,2.4);
          if(isLoose)  FillHist(label_channel+"/LeptonPtAbsEta_Loose",muPt,muAbsEta,weight,100,0.,1000.,24,0.0,2.4);
          if(isTight)  FillHist(label_channel+"/LeptonPtAbsEta_Tight",muPt,muAbsEta,weight,100,0.,1000.,24,0.0,2.4);
        
          cout << "muIDBit: " << mu.GetIDBit() << " muPt: " << muPt << " muAbsEta: " << muAbsEta << " isVeto: " << isVeto << " isLoose: " << isLoose << " isTight: " << isTight << endl;

        }

      }
      else if(isEl){

        for(auto const& el : electrons_veto){

          double elPt = el.Pt(); 
          double elAbsEta = fabs(el.Eta());
          if(elPt > 1000.)    elPt = 999.9;
          if(elAbsEta > 2.4)  elAbsEta = 2.399;

          bool isVeto  = el.PassID(param.Electron_Veto_ID);            // Veto-to-Veto (debug)
          bool isLoose = isVeto && el.PassID(param.Electron_Loose_ID); // Veto-to-Loose
          bool isTight = isVeto && el.PassID(param.Electron_Tight_ID); // Veto-to-Tight

          if(isVeto)   FillHist(label_channel+"/LeptonPtAbsEta_Veto", elPt,elAbsEta,weight,100,0.,1000.,24,0.0,2.4);
          if(isLoose)  FillHist(label_channel+"/LeptonPtAbsEta_Loose",elPt,elAbsEta,weight,100,0.,1000.,24,0.0,2.4);
          if(isTight)  FillHist(label_channel+"/LeptonPtAbsEta_Tight",elPt,elAbsEta,weight,100,0.,1000.,24,0.0,2.4);
        }

      }
      
    }

  }
  
  return;

}

WRTau_LeptonFake::WRTau_LeptonFake(){

}

WRTau_LeptonFake::~WRTau_LeptonFake(){

}


