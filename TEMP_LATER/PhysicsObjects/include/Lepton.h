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

private:
  double j_dXY, j_dZ;
  double j_RelIso;

  ClassDef(Lepton,1);

};

#endif
