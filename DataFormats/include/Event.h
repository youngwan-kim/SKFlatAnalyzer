#ifndef Event_h
#define Event_h

#include "Particle.h"
#include "TString.h"
#include "TObject.h"

class Event : public TObject {
public:

  Event();
  ~Event();

  void SetMCweight(double genw);
  inline double MCweight() const { return j_MCweight; }

  void SetnPV(double n);
  inline int nPV() const { return j_nPV; }

  void SetTrigger(vector<string> HLT_TriggerName);
  bool PassTrigger(TString trig);
  bool PassTrigger(std::vector<TString> trigs);
  double GetTriggerLumi(TString trig);
  bool IsPDForTrigger(TString trig, TString PD);

  void SetMET(double pt, double phi);
  inline Particle GetMETVector() const {return j_METVector;}

  void SetDataYear(int y);
  inline int DataYear() const {return j_DataYear;}

private:
  double j_MCweight;
  int j_nPV;
  vector<string> j_HLT_TriggerName;
  Particle j_METVector;
  int j_DataYear;

  ClassDef(Event,1)
};

#endif
