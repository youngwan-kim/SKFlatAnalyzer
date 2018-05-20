import os

lines = open('temp.txt').readlines()

for line in lines:
  line = line.strip('\n')

  ## ZToEE_NNPDF31_13TeV-powheg_M_120_200
  alias = line.replace('NNPDF31_13TeV-powheg_','')

  # ZZ_pythia PATH1/MC/ZZ_TuneCP5_13TeV-pythia8 16.523  1949719 1949719
  #print alias+'\t'+'PATH1/MC/'+line+'\t1\t1\t1'

  #print 'python python/SKFlat.py -a GetEffLumi -i '+alias+' -n 100 &'
  #print 'python python/SKFlat.py -a SKFlatValidation -i '+alias+' -n 100 &'
  print line+'\t'+alias
