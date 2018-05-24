#include "CalcCFRate.h"

void CalcCFRate::executeEvent(){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  //bool PassDiElectron = ev.PassTrigger("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");

  std::vector<Electron> electrons = GetElectrons("passMVAID_iso_WP80", 10., 2.5);
  vector<Gen> gens = GetGens();

  for(unsigned int i=0; i<electrons.size(); i++){

    Electron el = electrons.at(i);
    Gen genel = GetGenMatchedLepton(el, gens);
    if(genel.IsEmpty()) continue;
    if(!genel.isPromptFinalState()) continue;

    //==== Den : All Prompt Electrons

    //PrintGen(gens);

    TString EtaString = "";
    Electron::EtaRegion etr = el.etaRegion();
    if(etr==Electron::IB) EtaString = "InnerBarrel";
    else if(etr==Electron::OB) EtaString = "OuterBarrel";
    else if(etr==Electron::EC) EtaString = "EndCap";

    FillHist("Den_AllEta_Reco_Pt", el.Pt(), 1., 2000, 0., 2000.);
    FillHist("Den_AllEta_Reco_InvPt", 1./el.Pt(), 1., 1000, 0., 0.1);
    FillHist("Den_"+EtaString+"_Reco_Pt", el.Pt(), 1., 2000, 0., 2000.);
    FillHist("Den_"+EtaString+"_Reco_InvPt", 1./el.Pt(), 1., 1000, 0., 0.1);

    FillHist("Den_AllEta_Gen_Pt", genel.Pt(), 1., 2000, 0., 2000.);
    FillHist("Den_AllEta_Gen_InvPt", 1./genel.Pt(), 1., 1000, 0., 0.1);
    FillHist("Den_"+EtaString+"_Gen_Pt", genel.Pt(), 1., 2000, 0., 2000.);
    FillHist("Den_"+EtaString+"_Gen_InvPt", 1./genel.Pt(), 1., 1000, 0., 0.1);

    if( el.Charge() != genel.Charge() ){

			FillHist("Num_AllEta_Reco_Pt", el.Pt(), 1., 2000, 0., 2000.);
			FillHist("Num_AllEta_Reco_InvPt", 1./el.Pt(), 1., 1000, 0., 0.1);
			FillHist("Num_"+EtaString+"_Reco_Pt", el.Pt(), 1., 2000, 0., 2000.);
			FillHist("Num_"+EtaString+"_Reco_InvPt", 1./el.Pt(), 1., 1000, 0., 0.1);

			FillHist("Num_AllEta_Gen_Pt", genel.Pt(), 1., 2000, 0., 2000.);
			FillHist("Num_AllEta_Gen_InvPt", 1./genel.Pt(), 1., 1000, 0., 0.1);
			FillHist("Num_"+EtaString+"_Gen_Pt", genel.Pt(), 1., 2000, 0., 2000.);
			FillHist("Num_"+EtaString+"_Gen_InvPt", 1./genel.Pt(), 1., 1000, 0., 0.1);

      //PrintGen(gens);

    }

  }


}

