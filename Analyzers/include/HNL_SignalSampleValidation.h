#ifndef HNL_SignalSampleValidation_h
#define HNL_SignalSampleValidation_h

#include "HNL_RegionDefinitions.h"

class HNL_SignalSampleValidation : public HNL_RegionDefinitions {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNL_SignalSampleValidation();
  ~HNL_SignalSampleValidation();

};



#endif

