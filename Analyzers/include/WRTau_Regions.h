#ifndef WRTau_Regions_h
#define WRTau_Regions_h

#include "WRTau_Core.h"

class WRTau_Regions : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  WRTau_Regions();
  ~WRTau_Regions();

};



#endif

