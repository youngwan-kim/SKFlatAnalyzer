#include "MCCorrection.h"

double MCCorrection::MuonID_SF(TString ID, double eta, double pt, int sys){

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(ID=="NUM_TightID_DEN_genTracks"){

    //==== boundaries
    if(pt<20.) return 1.;
    if(pt>=120.) return 1.;
    if(eta>=2.4) return 1.;

    if(eta>=1.2 && eta<2.1){
      if(pt>=20.0 && pt<25.0){
        value = 0.992425271988;
        error = 0.00778595274024;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.990636422294;
        error = 0.00097132137985;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.989088446128;
        error = 0.0149053560478;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.992046432214;
        error = 0.00213539645672;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.994646906988;
        error = 0.0124242236899;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.992652882516;
        error = 0.0099316781155;
      }
    }
    else if(eta>=2.1 && eta<2.4){
      if(pt>=20.0 && pt<25.0){
        value = 0.975809583953;
        error = 0.00439931512178;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.96735684161;
        error = 0.00370065251696;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.974515359418;
        error = 0.00271110098253;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.976631185673;
        error = 0.00862666466883;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.978741050016;
        error = 0.0010035577872;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.97818912292;
        error = 0.00112306059414;
      }
    }
    else if(eta>=0.9 && eta<1.2){
      if(pt>=20.0 && pt<25.0){
        value = 0.992738927552;
        error = 0.00474373706613;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.983905638476;
        error = 0.00159172336927;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.985063939763;
        error = 0.0218787608424;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.984060403143;
        error = 0.0121278780491;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.986535946418;
        error = 0.000690352635267;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.984913093101;
        error = 0.0201350309149;
      }
    }
    else if(eta>=0.0 && eta<0.9){
      if(pt>=20.0 && pt<25.0){
        value = 0.991077762776;
        error = 0.0034967203087;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.985554516033;
        error = 0.000860273019434;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.987410468262;
        error = 0.00189750829605;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.989805737709;
        error = 0.00162043274196;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.990775327914;
        error = 0.000397750319711;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.989248358895;
        error = 0.000323299413124;
      }
    }
  }

  return value+double(sys)*error;

}

double MCCorrection::MuonISO_SF(TString ID, double eta, double pt, int sys){

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(ID=="NUM_TightRelIso_DEN_TightIDandIPCut"){

    //==== boundaries
    if(pt<20.) return 1.;
    if(pt>=120.) return 1.;
    if(eta>=2.4) return 1.;

    if(eta>=1.2 && eta<2.1){
      if(pt>=20.0 && pt<25.0){
        value = 0.991401054632;
        error = 0.00266822208643;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.998596887338;
        error = 0.000489111385143;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.994066766324;
        error = 0.00146598770113;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.999649117604;
        error = 0.0007066126508;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.995523772937;
        error = 0.000482010496461;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.997467918855;
        error = 0.000223810714069;
      }
    }
    else if(eta>=2.1 && eta<2.4){
      if(pt>=20.0 && pt<25.0){
        value = 0.988829710363;
        error = 0.00365610624011;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.99936392435;
        error = 0.000859753629781;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.992319762458;
        error = 0.00197629461214;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.998346811655;
        error = 0.00136181157567;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.994995867277;
        error = 0.00070835059911;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.997926058557;
        error = 0.000409835920651;
      }
    }
    else if(eta>=0.9 && eta<1.2){
      if(pt>=20.0 && pt<25.0){
        value = 0.995732951929;
        error = 0.00643343277667;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.999346249444;
        error = 0.000904854835854;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.988968679025;
        error = 0.00322373278457;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 1.00001772671;
        error = 0.00117969595782;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.994737975344;
        error = 0.000923140434339;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.997015995715;
        error = 0.00040090331866;
      }
    }
    else if(eta>=0.0 && eta<0.9){
      if(pt>=20.0 && pt<25.0){
        value = 0.992781348803;
        error = 0.00362150958619;
      }
      else if(pt>=50.0 && pt<60.0){
        value = 0.998446695575;
        error = 0.000456205995783;
      }
      else if(pt>=25.0 && pt<30.0){
        value = 0.997332232619;
        error = 0.00177715512476;
      }
      else if(pt>=60.0 && pt<120.0){
        value = 0.999529969744;
        error = 0.000622124621086;
      }
      else if(pt>=30.0 && pt<40.0){
        value = 0.997576733544;
        error = 0.000484942712098;
      }
      else if(pt>=40.0 && pt<50.0){
        value = 0.99821234049;
        error = 0.000251331353176;
      }
    }
  }

  return value+double(sys)*error;

}


