#ifndef TauHLTDebug_h
#define TauHLTDebug_h

#include "AnalyzerCore.h"

class TauHLTDebug : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  TauHLTDebug();
  ~TauHLTDebug();

};



#endif

