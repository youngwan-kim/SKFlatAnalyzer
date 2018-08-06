import os

SKFlatV = os.environ['SKFlatV']

txtfilename = 'sample_DATA.txt'
#txtfilename = 'sample_MC.txt'
#txtfilename = 'sample_PrivateMC.txt'

IsDATA = ("DATA" in txtfilename)
periods = ["B","C","D","E","F"]

lines = open('samples/'+txtfilename).readlines()

for line in lines:

  line = line.strip('\n')

  if "#" in line:
    continue

  if IsDATA:

    for period in periods:

      ## check timestamp
      os.system('ls -1d /xrootd/store/user/jskim/SKFlat/'+line+'/SKFlat_'+SKFlatV+'_period'+period+'/* | wc -l > dircount.txt')
      dircount = open('dircount.txt').readlines()
      os.system('rm dircount.txt')
      ndir = int(dircount[0])

      if ndir != 1:
         print '[Need Only One TimeSampt] /xrootd/store/user/jskim/SKFlat/'+line+'/SKFlat_'+SKFlatV+'_period'+period+'/*'

      os.system('ls -1 /xrootd/store/user/jskim/SKFlat/'+line+'/SKFlat_'+SKFlatV+'_period'+period+'/*/*/*.root &> '+line+'_'+period+'.txt')
      os.system("sed -i 's/\/xrootd/root:\/\/cms-xrdr.sdfarm.kr:1094\/\/\/xrd/g' "+line+'_'+period+'.txt')
      ## check number of files
      tmp = open(line+'_'+period+'.txt').readlines()
      print line+'_'+period+'\t'+str(len(tmp))

  else:
    PD = line.split()[0]
    alias = line.split()[1]

    ## check timestamp
    os.system('ls -1d /xrootd/store/user/jskim/SKFlat/'+PD+'/SKFlat_'+SKFlatV+'/* | wc -l > dircount.txt')
    dircount = open('dircount.txt').readlines()
    os.system('rm dircount.txt')
    ndir = int(dircount[0])

    if ndir != 1:
      print '[Need Only One TimeSampt] /xrootd/store/user/jskim/SKFlat/'+PD+'/SKFlat_'+SKFlatV+'/*'

    os.system('ls -1 /xrootd/store/user/jskim/SKFlat/'+PD+'/SKFlat_'+SKFlatV+'/*/*/*.root &> '+alias+'.txt')
    os.system("sed -i 's/\/xrootd/root:\/\/cms-xrdr.sdfarm.kr:1094\/\/\/xrd/g' "+alias+'.txt')
