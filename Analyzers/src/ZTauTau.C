#include "ZTauTau.h"
#include <bitset>

void ZTauTau::initializeAnalyzer(){

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {2,4,5}; vEl_vec = {8,13}; vMu_vec = {18,21};

  for(const auto &vjet : vJet_vec){
    for(const auto &vel : vEl_vec){
      std::pair<int,int> idpair = std::make_pair(vjet,vel);
      tauidsftool_map[idpair] = new TauIDSFTool("UL"+std::to_string(DataYear),DeepTauVSjet,idname_map_str[vjet]);
    }
  }

  // HLT_DoubleMediumChargedIsoPFTau40_Trk1_eta2p1_Reg_v
  // HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v8
  if(DataYear==2017){
    Triggers = {"HLT_IsoMu27_v"};
  }

}

void ZTauTau::executeEvent(){

  //FillTimer("START_EV");

  AnalyzerParameter param;

  param.Name = "ZTauTau";
  param.Muon_ID_SF_Key = "NUM_TightID_DEN_TrackerMuons";
  param.Electron_Veto_ID = "passVetoID";
  param.Muon_Veto_ID = "POGLoose";
  param.syst_ = AnalyzerParameter::Central;

  beginEvent();

  //FillTimer("GetAllObjects_beginEventCall");

  executeEventFromParameter(param);

  //FillTimer("END_EV");

}

void ZTauTau::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  //FillTimer("GetEvents");

  Particle METv = ev.GetMETVector();

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Tau> this_AllTaus = All_Taus;
  vector<Muon> this_AllMuons;
  vector<Electron> this_AllElectrons;

  //cout << "nAllMu " << AllMuons.size() << endl;

  if(HasFlag("PromptLepton")){
    this_AllMuons = MuonPromptOnly(All_Muons,All_Gens);
    this_AllElectrons = ElectronPromptOnly(All_Electrons,All_Gens);
  }

  else if(HasFlag("NonpromptLepton")){
    this_AllMuons = MuonNonPromptOnly(All_Muons,All_Gens);
    this_AllElectrons = ElectronNonPromptOnly(All_Electrons,All_Gens);
  }
  
  else{
    this_AllMuons = All_Muons;
    this_AllElectrons = All_Electrons;
  }

  //FillTimer("LeptonTruthMatching");

  //cout << "nThisAllMu " << this_AllMuons.size() << endl;

  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 30., 2.4) ;
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 30., 2.4);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);

  vector<Muon> muons = SelectMuons(this_AllMuons,"POGTight",30,2.4);
  vector<Lepton *> muons_lep = MakeLeptonPointerVector(muons);

  //FillTimer("ObjectSelection");

  //cout << "nMu " << muons.size() << endl;

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = "vJet"+idname_map[vJet_vec.at(i)]+"_vEl"+idname_map[vEl_vec.at(j)]+"_vMu"+idname_map[vMu_vec.at(k)];
        TString path = param.Name+"/"+idname;
        taus_temp = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.4);
        
        taus.clear();

        if(HasFlag("NonpromptTau")) taus = TauFakeOnly(taus_temp,All_Gens);
        else if(HasFlag("PromptTau")) taus = TauPromptOnly(taus_temp,All_Gens);
        else taus = taus_temp;

        //FillTimer("TauTruthMatching_"+idname);

        std::sort(taus.begin(),taus.end(),PtComparing);
        std::sort(muons.begin(),muons.end(),PtComparing);

        //FillTimer("SortObjects_"+idname);

        if(!ev.PassTrigger(Triggers)) continue;

        FillHist(path+"/Cutflow",0,weight,10,0.,10.);
        FillHist(path+"/nMuons",muons.size(),weight,10,0.,10.);
        FillHist(path+"/nTaus",taus.size(),weight,10,0.,10.);
        //cout << "test2 @" << idname  << endl;
        if(!(taus.size()>0 && muons.size()>0)) continue;
        FillHist(path+"/Cutflow",1,weight,10,0.,10.);
        //cout << "test3 @" << idname  << endl;
        if(taus.at(0).Pt()<35) continue;
        FillHist(path+"/Cutflow",2,weight,10,0.,10.);
        //cout << "test4 @" << idname  << endl;

        if(taus.at(0).Charge()*muons.at(0).Charge()>0) continue;
        FillHist(path+"/Cutflow",3,weight,10,0.,10.);

        if(METv.Pt()>50) continue;
        FillHist(path+"/Cutflow",4,weight,10,0.,10.);

        Particle Zcand = taus.at(0)+muons.at(0);
        double mTtaul = MT(Zcand,METv);

        if(!(Zcand.M() > 50 && Zcand.M() < 130 )) continue;
        FillHist(path+"/Cutflow",5,weight,10,0.,10.);

        //FillTimer("EventSelection_"+idname);

        double weight_muon(1.),weight_tau(1.);

        if(!IsDATA){
          if(HasFlag("PromptLepton")){  
            weight_muon *= mcCorr->MuonID_SF(param.Muon_ID_SF_Key,muons.at(0).Eta(),muons.at(0).Pt());
            weight_muon *= mcCorr->MuonTrigger_SF("POGTight","IsoMu27",muons);
          }
          else if(HasFlag("unweighted") || HasFlag("NonpromptLepton")){
            weight_muon = 1.;
          }
          else{
            weight_muon *= mcCorr->MuonID_SF(param.Muon_ID_SF_Key,muons.at(0).Eta(),muons.at(0).Pt());
            weight_muon *= mcCorr->MuonTrigger_SF("POGTight","IsoMu27",muons);
          }
        }

        //FillTimer("LeptonWeight_"+idname);

        if(!IsDATA){
          
          // TAU POG ID SF 
          // https://github.com/cms-tau-pog/TauIDSFs/tree/master#c
          
          if(!HasFlag("unweighted")){

            if(HasFlag("PromptTau")){
              
              //FillTimer("GetTauWeightStart_"+idname);
              std::pair<int,int> idpair = std::make_pair(vJet_vec.at(i),vEl_vec.at(j));
              weight_tau *= tauidsftool_map[idpair]->getSFvsPT(taus.at(0).Pt());
              //FillTimer("GetTauWeightEnd_"+idname);
            }

            if(HasFlag("NonpromptTau")) weight_tau = 1.0;

          }

          else if(HasFlag("unweighted")) weight_tau *= 1.;
          
          if(HasFlag("noTauWeight")) weight_tau = 1.;

        }

        weight *= weight_muon*weight_tau;

        FillLeptonPlots(muons_lep,path,weight);
        FillHist(path+"/dRtm",taus.at(0).DeltaR(muons.at(0)),weight,60,0.,6.);
        FillHist(path+"/ZCandMass",Zcand.M(),weight,80,50.,130.);
        FillHist(path+"/ZCandMT",mTtaul,weight,200,0.,200.);


      }
    }
  }


}

ZTauTau::ZTauTau(){

}

ZTauTau::~ZTauTau(){

}


