#ifndef SKFlatValidation_h
#define SKFlatValidation_h

#include "AnalyzerCore.C"

class SKFlatValidation : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);

  void executeEvent();

};



#endif

