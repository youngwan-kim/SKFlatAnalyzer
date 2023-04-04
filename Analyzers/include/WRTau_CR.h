#ifndef WRTau_CR_h
#define WRTau_CR_h

#include "AnalyzerCore.h"

class WRTau_CR : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  WRTau_CR();
  ~WRTau_CR();

};



#endif

