#ifndef WRTau_SingleLepton_h
#define WRTau_SingleLepton_h

#include "AnalyzerCore.h"

class WRTau_SingleLepton : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  WRTau_SingleLepton();
  ~WRTau_SingleLepton();

};



#endif

