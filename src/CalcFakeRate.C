#include "CalcFakeRate.h"

void CalcFakeRate::executeEvent(){

  AnalyzerParameter param;

  //==== To Check Denominator distribution, remove all selections for loose lepton

  param.Name = "SUSYTight_VeryLoose";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "NOCUT";
  param.Electron_Loose_RelIso = 999.;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "POGLoose";
  param.Muon_Loose_RelIso = 999.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

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

  //==== Test Loose Muon (0.6)

  param.Name = "SUSYTight_SUDYLooseNoIPMiniIso0p6";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "TEST";
  param.Muon_Loose_RelIso = 0.6;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== Test Loose Muon (0.4)

  param.Name = "SUSYTight_SUDYLooseNoIPMiniIso0p4";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "TEST";
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

  bool LooseMuonRelIso0p4 = param.Name.Contains("MiniIso0p4");

  if(!IsDATA){

    for(unsigned int i=0; i<lepptrs.size(); i++){

      Lepton *lepptr = lepptrs.at(i);
      Lepton lep = *lepptr;
      TString str_lepflv = "Electron";
      bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);

      double this_Tight_RelIso = param.Electron_Tight_RelIso;

      if(IsElectron){

      }
      else{
        this_Tight_RelIso = param.Muon_Tight_RelIso;
        str_lepflv = "Muon";

        if(LooseMuonRelIso0p4 && lep.MiniRelIso()>=0.4) continue;

      }

      double ptcone = lep.CalcPtCone(lep.MiniRelIso(), this_Tight_RelIso);
      lepptr->SetPtCone( ptcone );

      int lep_tyep = GetLeptonType(lep, gens);
      if(lep_tyep>=0) continue;

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

  JSFillHist(name, name+"_"+"Den_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+"Den_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+"Den_"+"Eta", lep->Eta(), weight, 60, -3, 3.);
  JSFillHist(name, name+"_"+"Den_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+"Den_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+"Den_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+"Den_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+"Den_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+"Den_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+"Den_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+"Den_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+"Den_"+"NEvent", 0., weight, 1, 0., 1.);

  if(IsTight){

    JSFillHist(name, name+"_"+"Num_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+"Num_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+"Num_"+"Eta", lep->Eta(), weight, 60, -3, 3.);
    JSFillHist(name, name+"_"+"Num_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+"Num_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+"Num_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+"Num_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+"Num_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+"Num_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+"Num_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+"Num_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+"Num_"+"NEvent", 0., weight, 1, 0., 1.);

  }

  if(lep->LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)lep;

    JSFillHist(name, name+"_"+"Den_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    if(IsTight){
      JSFillHist(name, name+"_"+"Num_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    }
  }
  else{
    Muon *mu = (Muon *)lep;

    JSFillHist(name, name+"_"+"Den_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
    if(IsTight){
      JSFillHist(name, name+"_"+"Num_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
    }

  }


}














