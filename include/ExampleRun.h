#ifndef ExampleRun_h
#define ExampleRun_h

#include "AnalyzerCore.C"

class ExampleRun : public AnalyzerCore {

public:

  void executeEvent();
  double hoe_max;

  ExampleRun();
  ~ExampleRun();

};



#endif

