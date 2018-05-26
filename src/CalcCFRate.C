#include "CalcCFRate.h"

void CalcCFRate::executeEvent(){

  AnalyzerParameter param;

  //==== Tight

  param.Name = "SUSYTight";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";

  param.Muon_Tight_ID = "SUSYTight";

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== Loose

  param.Name = "SUSYLoose";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYLoose";

  param.Muon_Tight_ID = "TEST";

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

}

void CalcCFRate::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  std::vector<Electron> electrons = GetElectrons(param.Electron_Tight_ID, 10., 2.5);
  std::vector<Muon> muons = GetMuons(param.Muon_Tight_ID, 10., 2.4);
  vector<Gen> gens = GetGens();

  vector<Lepton *> lepptrs_electron = MakeLeptonPointerVector(electrons);
  vector<Lepton *> lepptrs_muon = MakeLeptonPointerVector(muons);
  vector<Lepton *> lepptrs;
  for(unsigned int i=0;i<lepptrs_electron.size();i++) lepptrs.push_back( lepptrs_electron.at(i) );
  for(unsigned int i=0;i<lepptrs_muon.size();i++) lepptrs.push_back( lepptrs_muon.at(i) );


  if(!IsDATA){

    for(unsigned int i=0; i<lepptrs.size(); i++){

      Lepton *lepptr = lepptrs.at(i);
      Lepton lep = *lepptr;
      TString str_lepflv = "Electron";
      bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);

      TString EtaString = "";

      if(IsElectron){
        Electron *el = (Electron *)lepptr;
        double eta = fabs(el->scEta());
        if( eta < 0.8 ) EtaString = "InnerBarrel";
        else if( eta < 1.444 ) EtaString = "OuterBarrel";
        else if( eta < 1.566 ) EtaString = "EndCap";
        else EtaString = "EndCap";
      }
      else{
        str_lepflv = "Muon";
        double eta = fabs(lep.Eta());
        if( eta < 0.8 ) EtaString = "InnerBarrel";
        else if( eta < 1.444 ) EtaString = "OuterBarrel";
        else if( eta < 1.566 ) EtaString = "EndCap";
        else EtaString = "EndCap";
      }

      Gen genlep = GetGenMatchedLepton(lep, gens);
      int lep_tyep = GetLeptonType(lep, gens);

      //==== Only prompt
      if(lep_tyep<=0) continue;

      bool IsCF = lep.Charge()!=genlep.Charge();

      FillCFRatePlots(str_lepflv+"_"+param.Name, lepptr, genlep, IsCF, 1.);

    }

  }

}

void CalcCFRate::FillCFRatePlots(TString name, Lepton *lep, Gen genlep, bool IsCF, double weight){

  JSFillHist(name, name+"_"+"Den_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+"Den_"+"InvPt", 1./lep->Pt(), weight, 1000, 0., 0.1);

  JSFillHist(name, name+"_"+"Den_"+"GenPt", genlep.Pt(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+"Den_"+"InvGenPt", 1./genlep.Pt(), weight, 1000, 0., 0.1);

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

  if(IsCF){

    JSFillHist(name, name+"_"+"Num_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+"Num_"+"InvPt", 1./lep->Pt(), weight, 1000, 0., 0.1);

    JSFillHist(name, name+"_"+"Num_"+"GenPt", genlep.Pt(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+"Num_"+"InvGenPt", 1./genlep.Pt(), weight, 1000, 0., 0.1);

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

}





