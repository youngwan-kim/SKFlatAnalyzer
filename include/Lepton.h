#ifndef Lepton_h
#define Lepton_h

#include "Particle.h"

class Lepton: public Particle {
public:

  Lepton();
  ~Lepton();

  void SetdXY(double dXY, double dXYerr);
  inline double dXY() const {return j_dXY;}
  inline double dXYerr() const {return j_dXYerr;}

  void SetdZ(double dZ, double dZerr);
  inline double dZ() const {return j_dZ;}
  inline double dZerr() const {return j_dZerr;}

  void SetIP3D(double IP3D, double IP3Derr);
  inline double IP3D() const {return j_IP3D;}
  inline double IP3Derr() const {return j_IP3Derr;}

  //==== AbsIso will be set in Muon/Electron,
  //==== and use SetRelIso to save calculated RelIso
  void SetRelIso(double r);
  inline double RelIso() const {return j_RelIso;}

  //==== SUSY mini Iso has same formula for Muon and Electron
  void SetMiniIso(double ch, double nh, double ph, double pu, double rho, double EA);
  inline double MiniRelIso() const {return j_MiniRelIso;}

  enum Flavour{
    NONE, ELECTRON, MUON
  };
  Flavour LeptonFlavour() const {return j_LeptonFlavour;}
  void SetLeptonFlavour(Flavour f);

  float miniIsoDr(){
    float mindr = 0.05;
    float maxdr = 0.2;
    float kt_scale = 10.0;
    return std::max(mindr, std::min(maxdr, float(kt_scale/this->Pt())));
  }

private:
  double j_dXY, j_dXYerr;
  double j_dZ, j_dZerr;
  double j_IP3D, j_IP3Derr;

  double j_RelIso, j_MiniRelIso;
  Flavour j_LeptonFlavour;

};

Lepton::Lepton() : Particle() {
  j_dXY = -999.;
  j_dXYerr = -999.;
  j_dZ = -999.;
  j_dZerr = -999.;
  j_IP3D = -999.;
  j_IP3Derr = -999.;
  j_RelIso = -999.;
  j_MiniRelIso = -999.;
  j_LeptonFlavour = NONE;
}

Lepton::~Lepton(){

}

void Lepton::SetdXY(double dXY, double dXYerr){
  j_dXY = dXY;
  j_dXYerr = dXYerr;
}

void Lepton::SetdZ(double dZ, double dZerr){
  j_dZ = dZ;
  j_dZerr = dZerr;
}

void Lepton::SetIP3D(double IP3D, double IP3Derr){
  j_IP3D = IP3D;
  j_IP3Derr = IP3Derr;
}

void Lepton::SetRelIso(double r){
  j_RelIso = r;
}

void Lepton::SetMiniIso(double ch, double nh, double ph, double pu, double rho, double EA){

  double r_mini = miniIsoDr();

  double correction = rho * EA * (r_mini/0.3) * (r_mini/0.3);
  double correctedIso = ch + std::max(0.0, nh + ph - correction);
  j_MiniRelIso = correctedIso/this->Pt();

}


void Lepton::SetLeptonFlavour(Flavour f){
  j_LeptonFlavour = f;
}

#endif
