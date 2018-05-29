#include "ExampleRun.h"

ExampleRun::ExampleRun(){

  hoe_max = -1.;

}
ExampleRun::~ExampleRun(){

}

void ExampleRun::executeEvent(){

  Event ev = GetEvent();
  FillHist("nPV", ev.nPV(), 1., 50, 0., 50.);

  std::vector<Electron> electrons_all = GetAllElectrons();

  for(unsigned int i=0; i<electrons_all.size(); i++){

    Electron el = electrons_all.at(i);

    if(fabs(el.scEta())>2.5) continue;

    bool POGLoose = el.passLooseID();
    bool MyPOGLoose = el.Pass_CustCBID();

    if( POGLoose && !MyPOGLoose ){
      FillHist("PassPOG__NotMine", 0., 1., 1, 0., 1.);

/*
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
*/

/*
      if(el.HoverE() > hoe_max){
        hoe_max = el.HoverE();
        cout << "#######################" << endl;
        cout << "H/E max = " << hoe_max << endl;
        cout << "#######################" << endl;
      }
*/

    }

    if( !POGLoose && MyPOGLoose ){
      FillHist("PassMyPOG__NotPOG", 0., 1., 1, 0., 1.);

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

  }

}

