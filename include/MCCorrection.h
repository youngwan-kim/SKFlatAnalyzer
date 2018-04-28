#ifndef MCCorrection_h
#define MCCorrection_h

class MCCorrection{

public:

  double MuonID_SF(TString ID, double eta, double pt, int sys=0);
  double MuonISO_SF(TString ID, double eta, double pt, int sys=0);

};

#endif
