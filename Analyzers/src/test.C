#include "test.h"

void test::initializeAnalyzer(){

}

void test::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void test::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  if(HasFlag("test")) cout<< "flag ok \n" ;

}

test::test(){

}

test::~test(){

}


