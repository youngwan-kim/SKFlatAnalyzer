import os

txtfilename = 'sample_DATA.txt'
txtfilename = 'sample_MC.txt'

IsDATA = ("DATA" in txtfilename)
periods = ["B","C","D","E","F"]

lines = open('samples/'+txtfilename).readlines()

for line in lines:

  line = line.strip('\n')

  if "#" in line:
    continue

  if IsDATA:

    for period in periods:

      os.system('ls -1 /xrootd/store/user/jskim/SKFlat/'+line+'/SKFlat_v946p1_2_period'+period+'/*/*/*.root &> '+line+'_'+period+'.txt')
      os.system("sed -i 's/\/xrootd/root:\/\/cms-xrdr.sdfarm.kr:1094\/\/\/xrd/g' "+line+'_'+period+'.txt')

  else:
    PD = line.split()[0]
    alias = line.split()[1]

    os.system('ls -1 /xrootd/store/user/jskim/SKFlat/'+PD+'/SKFlat_v946p1_2/*/*/*.root &> '+alias+'.txt')
    os.system("sed -i 's/\/xrootd/root:\/\/cms-xrdr.sdfarm.kr:1094\/\/\/xrd/g' "+alias+'.txt')
