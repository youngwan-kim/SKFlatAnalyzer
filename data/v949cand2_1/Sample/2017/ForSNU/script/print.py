import os

SKFlatV = os.environ['SKFlatV']
Year = "2017"

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

      outname = line+'_'+period+'.txt'
      os.system('ls -1 /data7/DATA/SKFlat/'+SKFlatV+'/DATA/'+line+'/period'+period+'/*/*/*.root > '+outname)

      ## check number of files
      tmp = open(outname).readlines()
      print line+'_'+period+'\t'+str(len(tmp))

  else:
    PD = line.split()[0]
    alias = line.split()[1]

    outname = alias+'.txt'
    os.system('ls -1 /data7/DATA/SKFlat/'+SKFlatV+'/MC/'+PD+'/*/*/*.root > '+outname)
    ## check number of files
    tmp = open(alias+'.txt').readlines()
    print alias+'\t'+str(len(tmp))
