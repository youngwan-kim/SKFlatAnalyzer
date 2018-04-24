#ifndef Muon_h
#define Muon_h

#include "Lepton.h"
#include "TString.h"

class Muon: public Lepton{

public:

  Muon();
  ~Muon();

  bool isPOGTight() const {return j_isPOGTight;}
  void SetisPOGTight(bool b);

  void SetIso(double ch04, double nh04, double ph04, double pu04);
  void CalcRelIso();

  //==== ID
  bool PassID(TString ID);
  bool Pass_POGTight();
  bool Pass_TESTID();

private:

  bool j_isPOGTight;
  double j_PFCH04, j_PFNH04, j_PFPH04, j_PU04;

  ClassDef(Muon,1);

};

#endif
