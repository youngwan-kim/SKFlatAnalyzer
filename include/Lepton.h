#ifndef Lepton_h
#define Lepton_h

#include "Particle.h"

class Lepton: public Particle {
public:

  Lepton();
  ~Lepton();

  void SetdXY(double dXY);
  inline double dXY() const {return j_dXY;}

  void SetdZ(double dZ);
  inline double dZ() const {return j_dZ;}

  void SetRelIso(double r);
  inline double RelIso() const {return j_RelIso;}

  enum Flavour{
    NONE, MUON, ELECTRON
  };
  Flavour LeptonFlavour() const {return j_LeptonFlavour;}
  void SetLeptonFlavour(Flavour f);

private:
  double j_dXY, j_dZ;
  double j_RelIso;
  Flavour j_LeptonFlavour;

};

Lepton::Lepton() : Particle() {
  j_dXY = -999.;
  j_dZ = -999.;
  j_RelIso = -999.;
  j_LeptonFlavour = NONE;
}

Lepton::~Lepton(){

}

void Lepton::SetdXY(double dXY){
  j_dXY = dXY;
}

void Lepton::SetdZ(double dZ){
  j_dZ = dZ;
}

void Lepton::SetRelIso(double r){
  j_RelIso = r;
}

void Lepton::SetLeptonFlavour(Flavour f){
  j_LeptonFlavour = f;
}

#endif
