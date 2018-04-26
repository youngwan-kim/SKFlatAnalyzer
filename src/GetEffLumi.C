#include "GetEffLumi.h"

void GetEffLumi::executeEvent(){

  Event ev = GetEvent();

  FillHist("sumW", 0., ev.MCweight(), 1, 0., 1.);

}
