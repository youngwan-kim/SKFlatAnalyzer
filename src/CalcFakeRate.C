#include "CalcFakeRate.h"

void CalcFakeRate::executeEvent(){

  AnalyzerParameter param;

  //==== SUS-17-009 ID

  param.Name = "SUSY";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "SUSYLoose";
  param.Muon_Loose_RelIso = 0.4;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);



}

void CalcFakeRate::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Loose_ID, 10., 2.5);
  std::vector<Muon> Loose_muons = GetMuons(param.Muon_Loose_ID, 10., 2.4);
  vector<Gen> gens = GetGens();

  vector<Lepton *> lepptrs_electron = MakeLeptonPointerVector(Loose_electrons);
  vector<Lepton *> lepptrs_muon = MakeLeptonPointerVector(Loose_muons);
  vector<Lepton *> lepptrs;
  for(unsigned int i=0;i<lepptrs_electron.size();i++) lepptrs.push_back( lepptrs_electron.at(i) );
  for(unsigned int i=0;i<lepptrs_muon.size();i++) lepptrs.push_back( lepptrs_muon.at(i) );

  //==== QCD Fake Rate (MC Only)

  const int n_eta = 3;
  float etaarray[n_eta+1] = {0.0, 0.8, 1.479, 2.5};
  const int n_pt = 8;
  float ptarray[n_pt+1] = {5., 10., 15., 20., 30., 40., 50., 60., 70.};

  if(!IsDATA){

    for(unsigned int i=0; i<lepptrs.size(); i++){

      Lepton *lepptr = lepptrs.at(i);
      Lepton lep = *lepptr;
      TString str_lepflv = "Electron";
      bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);

      double this_Tight_RelIso = param.Electron_Tight_RelIso;
      if(!IsElectron){
        this_Tight_RelIso = param.Muon_Tight_RelIso;
        str_lepflv = "Muon";
      }

      double ptcone = lep.CalcPtCone(lep.MiniRelIso(), this_Tight_RelIso);
      lepptr->SetPtCone( ptcone );

      Gen gen_lep = GetGenMatchedLepton(lep, gens);
      if(gen_lep.isPromptFinalState()) continue;
      if(gen_lep.isDirectPromptTauDecayProductFinalState()) continue;

/*
      cout << "------------------------------------" << endl;
      cout << "lep.MiniRelIso() = " << lep.MiniRelIso() << endl;
      cout << "this_Tight_RelIso = " << this_Tight_RelIso << endl;
      cout << "Pt() = " << lep.Pt() << endl;
      cout << "--> PtCone = " << ptcone << endl;
*/
      bool PassTight = false;
      if(IsElectron){
        Electron *el = (Electron *)lepptr;
        PassTight = el->PassID(param.Electron_Tight_ID);
      }
      else{
        Muon *mu = (Muon *)lepptr;
        PassTight = mu->PassID(param.Muon_Tight_ID);
      }

      FillFakeRatePlots(str_lepflv+"_"+param.Name, lepptr, PassTight, 1.);

    }

  }


}

void CalcFakeRate::FillFakeRatePlots(TString name, Lepton *lep, bool IsTight, double weight){

  FillHist(name+"_"+"Den_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
  FillHist(name+"_"+"Den_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
  FillHist(name+"_"+"Den_"+"RelIso", lep->RelIso(), weight, 500, 0., 0.5);
  FillHist(name+"_"+"Den_"+"MiniRelIso", lep->MiniRelIso(), weight, 500, 0., 0.5);

  if(IsTight){

    FillHist(name+"_"+"Num_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
    FillHist(name+"_"+"Num_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
    FillHist(name+"_"+"Num_"+"RelIso", lep->RelIso(), weight, 500, 0., 0.5);
    FillHist(name+"_"+"Num_"+"MiniRelIso", lep->MiniRelIso(), weight, 500, 0., 0.5);

  }

  if(lep->LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)lep;

    FillHist(name+"_"+"Den_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    if(IsTight){
      FillHist(name+"_"+"Num_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    }
  }


}














