#ifndef WRTau_SR_Orthogonal_h
#define WRTau_SR_Orthogonal_h

#include "AnalyzerCore.h"

class WRTau_SR_Orthogonal : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  WRTau_SR_Orthogonal();
  ~WRTau_SR_Orthogonal();

};



#endif

