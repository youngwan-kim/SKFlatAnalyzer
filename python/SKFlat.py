import os,sys
import argparse

parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-a', dest='Analyzer', default="")
parser.add_argument('-i', dest='InputSample', default="")
parser.add_argument('-p', dest='DataPeriod', default="ALL")
parser.add_argument('-n', dest='NJobs', default=1, type=int)
parser.add_argument('-o', dest='Outputdir', default="./")
args = parser.parse_args()

## Environment Variables

SKFlatV = os.environ['SKFlatV']
SKFlatAnV = os.environ['SKFlatAnV']
DATA_DIR = os.environ['DATA_DIR']
SKFlatRunlogDir = os.environ['SKFlatRunlogDir']

## Global Varialbes
IsDATA = False
InputSample_Data = ["DoubleMuon", "DoubleEG"]
if args.InputSample in InputSample_Data:
  IsDATA = True

## Get Sample Path
lines_SamplePath = open(DATA_DIR+"/SamplePath.txt").readlines()
Samplepath_Section="Alias"
SampleBaseDirs = []
NtupleFilePath = ""
for line in lines_SamplePath:

  if len(line.split())==0:
    continue

  words = line.split()

  if "Section:" in line:
    Samplepath_Section = words[1]
    continue

  if Samplepath_Section=="Alias":
    SampleBaseDirs.append( (words[0], words[1]) )


  if IsDATA and Samplepath_Section=="DATA":
    if words[0]==args.InputSample:
      NtupleFilePath = words[1]
      break

  #if not IsDATA and Samplepath_Section=="MC":
    
for BaseDir in SampleBaseDirs:
  alias = BaseDir[0]
  path = BaseDir[1]
  if alias in NtupleFilePath:
    NtupleFilePath = NtupleFilePath.replace(alias,path+'/'+SKFlatV+'/')
if IsDATA:
  NtupleFilePath = NtupleFilePath+'period'+args.DataPeriod+'/'

## Get file list ##
os.system('ls -1 '+NtupleFilePath+'/*.root > tmp_filelist.txt')
lines_files = open('tmp_filelist.txt').readlines()
os.system('rm tmp_filelist.txt')
NTotalFiles = len(lines_files)

print "<SKFlat> NTotalFiles = "+str(NTotalFiles)
print "<SKFlat> NJobs = "+str(args.NJobs)
nfilepjob = int(NTotalFiles/args.NJobs)
print "<SKFlat> --> # of files per job = "+str(nfilepjob)
nfilepjob_remainder = NTotalFiles-args.NJobs*nfilepjob

FileRanges = []
temp_end_largerjob = 0
for it_job in range(0,nfilepjob_remainder):
  FileRanges.append(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))
  temp_end_largerjob = (it_job+1)*(nfilepjob+1)
for it_job in range(0,args.NJobs-nfilepjob_remainder):
  FileRanges.append(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))

## Prepair output
base_rundir = SKFlatRunlogDir+'/'+args.InputSample
if IsDATA:
  base_rundir = base_rundir+'/period'+args.DataPeriod+'/'

CheckTotalNFile=0
for it_job in range(0,len(FileRanges)):
  print "["+str(it_job)+"th]",
  print FileRanges[it_job],
  print " --> "+str(len(FileRanges[it_job]))
  CheckTotalNFile = CheckTotalNFile+len(FileRanges[it_job])

  thisjob_dir = base_rundir+'/job_'+str(it_job)+'/'
  os.system('mkdir -p '+thisjob_dir)

  out = open(thisjob_dir+'run.C','w')
  print>>out,'''#include "{0}.C"

void run(){{

  SKFlatValidation m;

  TString outputdir = getenv("OUTPUTDIR");

  m.SetTreeName("recoTree/SKFlat");
'''.format(args.Analyzer)

  out.write('  m.SetOutfilePath("'+thisjob_dir+'/hists.root");\n')

  for it_file in FileRanges[it_job]:
    out.write('  m.AddFile("'+lines_files[it_file].strip('\n')+'");\n')

  print>>out,'''  m.Init();

  m.Loop();

  m.WriteHist();

}'''
  out.close()

  run_commands = open(thisjob_dir+'commands.sh','w')
  print>>run_commands,'''source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /data7/Users/jskim/CMSSW_9_4_4/src/
export SCRAM_ARCH=slc6_amd64_gcc630
cmsenv
cd {0}
root -l -b -q run.C
echo "FINISHED!!"
'''.format(thisjob_dir)
  run_commands.close()

  jobname = 'job_'+str(it_job)+'_'+args.Analyzer
  cmd = 'qsub -V -q fastq -N '+jobname+' -wd '+thisjob_dir+' commands.sh '

  cwd = os.getcwd()
  os.chdir(thisjob_dir)
  os.system(cmd)
  os.chdir(cwd)


#print CheckTotalNFile








