#include "ExampleRun.h"

ExampleRun::ExampleRun(){

  hoe_max = -1.;

}
ExampleRun::~ExampleRun(){

}

void ExampleRun::executeEvent(){


  AnalyzerParameter param;

  param.Name = "HNPair";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "HNPairLoose";
  param.Electron_Loose_RelIso = 0.6;
  param.Electron_Veto_ID = "HNPairVeto";
  param.Electron_Veto_RelIso = 0.6;
  param.Electron_FR_ID = "HNPair";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_UseMini = true;

  param.Muon_Tight_ID = "HNPairTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "HNPairLoose";
  param.Muon_Loose_RelIso = 0.6;
  param.Muon_Veto_ID = "HNPairVeto";
  param.Muon_Veto_RelIso = 0.6;
  param.Muon_FR_ID = "HNPair";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_UseMini = true;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);

}

void ExampleRun::executeEventFromParameter(AnalyzerParameter param){

  Event ev = GetEvent();

/*
  FillHist("nPV", ev.nPV(), 1., 50, 0., 50.);

  std::vector<Electron> electrons_all = GetAllElectrons();
  std::vector<Muon>     muons_all     = GetAllMuons();

  //==== Comparing POG CutBased and Mine

  for(unsigned int i=0; i<electrons_all.size(); i++){

    Electron el = electrons_all.at(i);

    if(fabs(el.scEta())>2.5) continue;

    bool POGLoose = el.passLooseID();
    bool MyPOGLoose = el.Pass_CutBasedLoose();

    bool POGVeto = el.passVetoID();
    bool MyPOGVeto = el.Pass_CutBasedVeto();

    if(POGLoose)   FillHist("POGLoose", 0., 1., 2, 0., 2.);
    if(MyPOGLoose) FillHist("POGLoose", 1., 1., 2, 0., 2.);
    if(POGVeto)   FillHist("POGVeto", 0., 1., 2, 0., 2.);
    if(MyPOGVeto) FillHist("POGVeto", 1., 1., 2, 0., 2.);


    if( POGLoose && !MyPOGLoose ){
      FillHist("PassPOGLoose__NotMyPOGLoose", 0., 1., 1, 0., 1.);


      cout << "----------------------------------------------------" << endl;
      cout << "|scEta| = " << el.scEta() << endl;
      cout << "Energy_PostCorr = " << el.E() << endl;
      cout << "Energy_PreCorr = " << el.UncorrE() << endl;
      cout << "full5x5 = " << el.full5x5_sigmaIetaIeta() << endl;
      cout << "|dEtaSeed| = " << fabs(el.dEtaSeed()) << endl;
      cout << "|dPhiIn| = " << fabs(el.dPhiIn()) << endl;
      cout << "H/E = " << el.HoverE() << endl;
      cout << "--> cut = " << 0.05 + 1.12/el.UncorrE() + 0.0368*el.Rho()/el.UncorrE() << endl;
      cout << "RelIso = " << el.RelIso() << endl;
      cout << "|1/E-1/p| = " << fabs(el.InvEminusInvP()) << endl;
      cout << "NMissHist = " << el.NMissingHits() << endl;
      cout << "PassConv = " << el.PassConversionVeto() << endl;



      if(el.HoverE() > hoe_max){
        hoe_max = el.HoverE();
        cout << "#######################" << endl;
        cout << "H/E max = " << hoe_max << endl;
        cout << "#######################" << endl;
      }


    }

    if( !POGLoose && MyPOGLoose ){
      FillHist("PassMyPOGLoose__NotPOGLoose", 0., 1., 1, 0., 1.);

      cout << "### |scEta| = " << el.scEta() << " ###" << endl;
      cout << "full5x5 = " << el.full5x5_sigmaIetaIeta() << endl;
      cout << "|dEtaSeed| = " << fabs(el.dEtaSeed()) << endl;
      cout << "|dPhiIn| = " << fabs(el.dPhiIn()) << endl;
      cout << "H/E = " << el.HoverE() << endl;
      cout << "--> cut = " << 0.05 + 1.12/el.UncorrE() + 0.0368*el.Rho()/el.UncorrE() << endl;
      cout << "RelIso = " << el.RelIso() << endl;
      cout << "|1/E-1/p| = " << fabs(el.InvEminusInvP()) << endl;
      cout << "NMissHist = " << el.NMissingHits() << endl;
      cout << "PassConv = " << el.PassConversionVeto() << endl;

    }


    if( POGVeto && !MyPOGVeto ){
      FillHist("PassPOGVeto__NotMyPOGVeto", 0., 1., 1, 0., 1.);
    }
    if( !POGVeto && MyPOGVeto ){
      FillHist("PassMyPOGVeto__NotPOGVeto", 0., 1., 1, 0., 1.);
    }

  } // END Electron Loop
*/

  //==== Fake Rate

  double MinPt = 40;

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Veto_ID, MinPt, 2.4);
  std::vector<Muon>     Veto_muons      = MuonWithoutGap(GetMuons(param.Muon_Veto_ID, MinPt, 2.4));
  int n_Veto_leptons = Veto_electrons.size()+Veto_muons.size();

  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Loose_ID, MinPt, 2.4);
  std::vector<Muon>     Loose_muons     = MuonWithoutGap(GetMuons(param.Muon_Loose_ID, MinPt, 2.4));

  std::vector<Electron> Tight_electrons = GetElectrons(param.Electron_Tight_ID, MinPt, 2.4);
  std::vector<Muon>     Tight_muons     = MuonWithoutGap(GetMuons(param.Muon_Tight_ID, MinPt, 2.4));

  std::vector<Lepton *> leps_electron = MakeLeptonPointerVector(Loose_electrons, param.Electron_Tight_RelIso, param.Electron_UseMini);
  std::vector<Lepton *> leps_muon     = MakeLeptonPointerVector(Loose_muons, param.Muon_Tight_RelIso, param.Muon_UseMini);
  std::vector<Lepton *> leps;
  for(unsigned int i=0;i<leps_electron.size(); i++) leps.push_back( leps_electron.at(i) );
  for(unsigned int i=0;i<leps_muon.size(); i++) leps.push_back( leps_muon.at(i) );

  bool UseEvent = false;
  if(Loose_electrons.size()==2 && Loose_muons.size()==0){
    if(Loose_electrons.at(0).Charge()==Loose_electrons.at(1).Charge()){
      UseEvent = true;
    }
  }
  if(Loose_electrons.size()==0 && Loose_muons.size()==2){
    if(Loose_muons.at(0).Charge()==Loose_muons.at(1).Charge()){
      UseEvent = true;
    }
  }

  if(UseEvent){

    double mll = ( *(leps.at(0))+*(leps.at(1)) ).M();
    if(mll>120){
      double this_fr_weight = fakeEst.GetWeight(leps, param);
      if(fakeEst.HasLooseLepton) FillHist("hist_exp", 0., this_fr_weight, 1, 0., 1.);
      else                       FillHist("hist_obs", 0., 1., 1, 0., 1.);
    }
  }

}



