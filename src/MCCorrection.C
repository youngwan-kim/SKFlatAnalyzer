#include "MCCorrection.h"

double MCCorrection::MuonIDSF(TString ID, double eta, double pt, int sys){

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(ID=="POGTight"){

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



