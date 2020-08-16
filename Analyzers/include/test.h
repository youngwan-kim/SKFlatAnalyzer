#ifndef test_h
#define test_h

#include "AnalyzerCore.h"

class test : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  test();
  ~test();

};



#endif

