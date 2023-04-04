#include "WRStudy.h"

void WRStudy::initializeAnalyzer(){

  triggers.clear();

  if(DataYear==2017){

    etau_triggers = {
      "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v8"
    };

    mutau_triggers = {
      "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v8"
    };

    singleEle_triggers = {"HLT_Ele32_WPTight_Gsf"};
    singleMu_triggers = {"HLT_Mu27_v5"};

    triggers = {
      "HLT_Ele32_WPTight_Gsf",
      "HLT_Mu27_v5",
    };

  }

}

void WRStudy::executeEvent(){


  AnalyzerParameter param;
  param.Clear();

  param.Name = "TauStudy";
  param.Electron_Tight_ID = "passTightID";
  param.Muon_Tight_ID = "POGTight";
  param.Electron_Veto_ID = "passVetoID";
  param.Muon_Veto_ID = "POGLoose";
  param.Jet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllGens = GetGens();

  executeEventFromParameter(param);

}

void WRStudy::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);
  if(!IsDATA) weight *= MCweight() * ev.GetTriggerLumi("Full") * GetPrefireWeight(0);
  std::map<TString,double> map_weight = {{"Unweighted",1.},{"Weighted",weight}};
  for(const auto &WeightPair : map_weight){
    FillHist("vJetVLooseTau/"+WeightPair.first+"/Cutflow",0.,WeightPair.second,10,0.,10.);
    FillHist("vJetLooseTau/"+WeightPair.first+"/Cutflow",0.,WeightPair.second,10,0.,10.);
    FillHist("vJetMediumTau/"+WeightPair.first+"/Cutflow",0.,WeightPair.second,10,0.,10.);
    FillHist("vJetTightTau/"+WeightPair.first+"/Cutflow",0.,WeightPair.second,10,0.,10.);
    FillHist("vJetVTightTau/"+WeightPair.first+"/Cutflow",0.,WeightPair.second,10,0.,10.);
  }

  // Seperate cases by trigger firing
  // TODO make a code to deal with firing both : just split it be if elif
  TString channel = "";
  if(ev.PassTrigger(singleEle_triggers)){
    channel = "ele";
    double safeptcut = 60;
  } 
  else if(ev.PassTrigger(singleMu_triggers)){
    channel = "mu";
    double safeptcut = 60;
  }

  //if(!(ev.PassTrigger(triggers))) return;
  for(const auto &WeightPair : map_weight){
    FillHist("vJetVLooseTau/"+WeightPair.first+"/Cutflow",1.,WeightPair.second,10,0.,10.);
    FillHist("vJetLooseTau/"+WeightPair.first+"/Cutflow",1.,WeightPair.second,10,0.,10.);
    FillHist("vJetMediumTau/"+WeightPair.first+"/Cutflow",1.,WeightPair.second,10,0.,10.);
    FillHist("vJetTightTau/"+WeightPair.first+"/Cutflow",1.,WeightPair.second,10,0.,10.);
    FillHist("vJetVTightTau/"+WeightPair.first+"/Cutflow",1.,WeightPair.second,10,0.,10.);
  }

  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.1) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 24., 2.1) ;

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.1);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 32., 2.1);

  vector<Lepton *> leptons = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);


  for(unsigned int i=0; i<AllTaus.size(); i++){
    for(unsigned int j=0; j<VetoLeps.size(); j++){
      if(AllTaus.at(i).DeltaR(*VetoLeps.at(j))<0.4){
        AllTaus.erase(AllTaus.begin()+i);
        break;
      }
    }
  }

  JetTagging::Parameters jtps = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  for(unsigned int i=0; i<AllJets.size(); i++){
    for(unsigned int j=0; j<VetoLeps.size(); j++){
      if(AllJets.at(i).DeltaR(*VetoLeps.at(j))<0.4){
        AllJets.erase(AllJets.begin()+i);
        break;
      }
    }
  }

  // TODO add fatjets 
  vector<Tau> this_AllTaus = AllTaus;
  vector<Jet> this_AllJets = AllJets;

  vector<Tau> taus_VLoose = SelectWRTaus(this_AllTaus, 2, 20., 2.1);
  vector<Tau> taus_Loose = SelectWRTaus(this_AllTaus, 3, 20., 2.1);
  vector<Tau> taus_Medium = SelectWRTaus(this_AllTaus, 4, 20., 2.1);
  vector<Tau> taus_Tight = SelectWRTaus(this_AllTaus, 5, 20., 2.1);
  vector<Tau> taus_VTight = SelectWRTaus(this_AllTaus, 6, 20., 2.1);
  vector<Jet> jets = SelectJets(this_AllJets, param.Jet_ID, 25., 2.1) ;
  vector<Jet> BJets = GetBJets(this_AllJets,jtps);

  vector<vector <Tau>> v_taus = {taus_VLoose, taus_Loose, taus_Medium, taus_Tight, taus_VTight};
  std::map<TString,vector <Tau>> map_taus = {{"vJetVLooseTau",taus_VLoose},{"vJetLooseTau",taus_Loose},{"vJetMediumTau",taus_Medium},{"vJetTightTau",taus_Tight},{"vJetVTightTau",taus_VTight}};


  for(unsigned int i=0; i<v_taus.size(); i++){
    std::sort(v_taus.at(i).begin(),v_taus.at(i).end(),PtComparing);
  }

  std::sort(muons.begin(),muons.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing);
  std::sort(electrons.begin(),electrons.end(),PtComparing);

  for(const auto &Pair : map_taus){
    for(const auto &WeightPair : map_weight){

      TString vJet_ID = Pair.first;
      TString weighted = "/"+WeightPair.first;
      double w = WeightPair.second;

      vector<Lepton *> tau_leptons = MakeLeptonPointerVector(Pair.second);
      for(unsigned i=0;i<tau_leptons.size();i++){
        FillHist(vJet_ID+weighted+"/TauLeptonType",GetLeptonType(*tau_leptons.at(i),AllGens),w,20,-10.,10.);
      }

      for(unsigned i=0;i<leptons.size();i++){
        FillHist(vJet_ID+weighted+"/LightLeptonType_BeforeSelection",GetLeptonType(*leptons.at(i),AllGens),w,20,-10.,10.);
      }

      FillHist(vJet_ID+weighted+"/nTau",Pair.second.size(),w,5,0.,5.);
      FillHist(vJet_ID+weighted+"/nLep",leptons.size(),w,5,0.,5.);

      if(leptons.size()<1) continue;
      FillHist(vJet_ID+weighted+"/Cutflow",2.,w,10,0.,10.);

      if(Pair.second.size()!=1) continue;
      FillHist(vJet_ID+weighted+"/Cutflow",3.,w,10,0.,10.);

      vector<Jet> jets_cleaned;
      for(unsigned int i=0; i<jets.size(); i++){
        if(jets.at(i).DeltaR(Pair.second.at(0))>0.05){
          jets_cleaned.push_back(jets.at(i));
        }
      }

      double dRltau = Pair.second.at(0).DeltaR(*leptons.at(0));

      if(dRltau<2.5) continue;
      FillHist(vJet_ID+weighted+"/Cutflow",4.,w,10,0.,10.);

      if(jets_cleaned.size()<2) continue;
      FillHist(vJet_ID+weighted+"/Cutflow",5.,w,10,0.,10.);

      Particle eff = Pair.second.at(0)+ *leptons.at(0)+METv+jets.at(0)+jets_cleaned.at(1);
      Particle eff_noMET = Pair.second.at(0)+ *leptons.at(0)+jets_cleaned.at(0)+jets_cleaned.at(1);

      //if(eff.M()<800) return
      //FillHist(vJet_ID+"/Cutflow",6.,weight,10,0.,10.);

      double dRtauj0 = Pair.second.at(0).DeltaR(jets_cleaned.at(0));
      double dRtauj1 = Pair.second.at(0).DeltaR(jets_cleaned.at(1));
      double dRlj0 = jets_cleaned.at(0).DeltaR(*leptons.at(0));
      double dRlj1 = jets_cleaned.at(1).DeltaR(*leptons.at(0));

      // Lepton
      FillHist(vJet_ID+weighted+"/Lepton/pT",leptons.at(0)->Pt(),w,500,0.,1000);
      FillHist(vJet_ID+weighted+"/Lepton/Eta",leptons.at(0)->Eta(),w,50,-2.5,2.5);
      FillHist(vJet_ID+weighted+"/Lepton/dRlj0",dRlj0,w,60,0.,6.);
      FillHist(vJet_ID+weighted+"/Lepton/dRlj1",dRlj1,w,60,0.,6.);
      FillHist(vJet_ID+weighted+"/Lepton/dRltau",dRltau,w,60,0.,6.);

      // Tau
      FillHist(vJet_ID+weighted+"/Tau/pT",Pair.second.at(0).Pt(),w,500,0.,1000);
      FillHist(vJet_ID+weighted+"/Tau/Eta",Pair.second.at(0).Eta(),w,50,-2.5,2.5);
      FillHist(vJet_ID+weighted+"/Tau/dRtauj0",dRtauj0,w,60,0.,6.);
      FillHist(vJet_ID+weighted+"/Tau/dRtauj1",dRtauj1,w,60,0.,6.);

      // MET
      FillHist(vJet_ID+weighted+"/METv/MET",METv.Pt(),w,500,0.,1000);
      FillHist(vJet_ID+weighted+"/METv/Phi",METv.Phi(),w,70,-3.5,3.5);

      // Jet
      double HT(0.); 
      for(unsigned int i=0; i<jets_cleaned.size(); i++){
        double dRjtau(0.);
        HT += jets_cleaned.at(i).Pt();
        dRjtau = jets_cleaned.at(i).DeltaR(Pair.second.at(0));
        FillHist(vJet_ID+weighted+"/Jet/Jet"+TString::Itoa(i,10)+"_Pt",jets_cleaned.at(i).Pt(),w,500,0.,1000);
        FillHist(vJet_ID+weighted+"/Jet/Jet"+TString::Itoa(i,10)+"_Eta",jets_cleaned.at(i).Eta(),w,50,-2.5,2.5);
        FillHist(vJet_ID+weighted+"/Jet/dRj"+TString::Itoa(i,10)+"tau",dRjtau,w,60,0,6.0);
      }
      FillHist(vJet_ID+weighted+"/Jet/dRj0j1",jets_cleaned.at(0).DeltaR(jets_cleaned.at(1)),w,60,0.,6.);


      // Inclusive Variables
      FillHist(vJet_ID+weighted+"/HT",HT,w,1000,0.,1000.);
      FillHist(vJet_ID+weighted+"/mEff",eff.M(),w,5000,0.,5000);
      FillHist(vJet_ID+weighted+"/mEff_noMET",eff_noMET.M(),w,5000,0.,5000);
      FillHist(vJet_ID+weighted+"/ST",eff.Pt(),w,1000,0.,1000.);
      FillHist(vJet_ID+weighted+"/nJets",jets_cleaned.size(),w,10,0.,10.);
      FillHist(vJet_ID+weighted+"/nBJets",BJets.size(),w,10,0.,10.);
      FillHist(vJet_ID+weighted+"/LightLeptonType",GetLeptonType(*leptons.at(0),AllGens),w,20,-10.,10.);
    }
  }


}

WRStudy::WRStudy(){

}

WRStudy::~WRStudy(){

}


