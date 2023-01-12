#ifndef HNL_Test_h
#define HNL_Test_h

#include "HNL_RegionDefinitions.h"

class HNL_Test : public HNL_RegionDefinitions {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNL_Test();
  ~HNL_Test();

};



#endif

