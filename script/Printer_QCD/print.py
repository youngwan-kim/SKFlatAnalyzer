import os

lines = open('samples.txt').readlines()

tags = [
"MuEnriched",
"EMEnriched",
"Double",
"bEnriched",
]

for tag in tags:
  #print "#### "+tag+" ####"
  for line in lines:
    if tag in line:
      #print line.strip('\n')

      if "#" in line:
        continue

      PD = line.split('/')[1]
      alias = PD.replace('_TuneCP5_13TeV_pythia8','')
      alias = alias.replace('_TuneCP5_13TeV_Pythia8','')
      #print PD+'\t'+alias
      print alias
      #print alias+'\t'+'PATH1/MC/'+PD+'\t1\t1\t1'

