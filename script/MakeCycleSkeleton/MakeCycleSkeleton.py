import os

cyclename = "CalcFakeRate"

out_h = open(cyclename+'.h','w')
print>>out_h,'''#ifndef {0}_h
#define {0}_h

#include "AnalyzerCore.C"

class {0} : public AnalyzerCore {{

public:

  void executeEvent();

}};



#endif
'''.format(cyclename)
out_h.close()

out_c = open(cyclename+'.C','w')
print>>out_c,'''#include "{0}.h"

void {0}::executeEvent(){{


}}
'''.format(cyclename)
out_c.close()

print 'mv '+cyclename+'.h ../../include/'
print 'mv '+cyclename+'.C ../../src'

out_run = open('run_'+cyclename+'.C','w')
print>>out_run,'''#include "{0}.C"

void run_{0}(){{

  {0} m;

  TString outputdir = getenv("OUTPUTDIR");

  m.SetTreeName("recoTree/SKFlat");

  m.MaxEvent = 10;

/*
  //==== For Data Run
  m.IsDATA = true;
  m.DataStream = "SingleMuon";
*/

  //==== For MC Run
/*
  m.MCSample = "DYJets";
  m.IsThisDataFile = false;
  m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/SKFlat_v946p1_2/180511_144610/0000/SKFlatNtuple_1.root");
*/

  m.SetOutfilePath("hists.root");

  m.Init();

  m.Loop();

  m.WriteHist();

}}
'''.format(cyclename)
out_run.close()

print 'mv run_'+cyclename+'.C ../../'
