#include "GetEffLumi.h"

void GetEffLumi::executeEvent(){

  Event ev = GetEvent();

  FillHist("sumW", 0.5, ev.MCweight(), 5, 0., 5.);
  FillHist("sumW", 1.5, ev.MCweight()*PDFWeights_Scale->at(0), 5, 0., 5.);

}
