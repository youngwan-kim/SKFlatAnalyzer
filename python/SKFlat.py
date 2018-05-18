import os,sys
import argparse
import datetime

print ''

## TimeStamp

JobStartTime = datetime.datetime.now()
timestamp =  JobStartTime.strftime('%Y_%m_%d_%H%M%S')

parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-a', dest='Analyzer', default="")
parser.add_argument('-i', dest='InputSample', default="")
parser.add_argument('-p', dest='DataPeriod', default="ALL")
parser.add_argument('-n', dest='NJobs', default=1, type=int)
parser.add_argument('-o', dest='Outputdir', default="")
parser.add_argument('-q', dest='Queue', default="fastq")
parser.add_argument('--no_exec', action='store_true')
args = parser.parse_args()

if args.Analyzer=="":
  print "No Analyzer"
  sys.exit(1)
if args.InputSample=="":
  print "No InputSample"
  sys.exit(1)

## Environment Variables

SKFlatV = os.environ['SKFlatV']
SKFlatAnV = os.environ['SKFlatAnV']
DATA_DIR = os.environ['DATA_DIR']
SKFlatRunlogDir = os.environ['SKFlatRunlogDir']
SKFlatOutputDir = os.environ['SKFlatOutputDir']
SKFlat_LIB_PATH = os.environ['SKFlat_LIB_PATH']

## Global Varialbes

IsDATA = False
InputSample_Data = ["DoubleMuon", "DoubleEG", "SingleMuon", "SingleElectron"]
if args.InputSample in InputSample_Data:
  IsDATA = True

## Prepare output

base_rundir = SKFlatRunlogDir+'/'+args.Analyzer+'__'+timestamp+'__'+args.InputSample
if IsDATA:
  base_rundir = base_rundir+'__period'+args.DataPeriod+'/'
os.system('mkdir -p '+base_rundir)

## Copy shared library file

os.system('mkdir -p '+base_rundir+'/libs/')
os.system('cp '+SKFlat_LIB_PATH+'/* '+base_rundir+'/libs')

## Create webdir

webdir = SKFlatRunlogDir+'/www/SKFlatAnalyzerJobLogs/'+base_rundir.replace(SKFlatRunlogDir,'')
os.system('mkdir -p '+webdir)
os.system('scp -r '+webdir+' jskim@147.47.242.71:/var/www/html/SKFlatAnalyzerJobLogs/')
os.system('ssh -Y jskim@147.47.242.71 chmod -R 777 /var/www/html/SKFlatAnalyzerJobLogs/'+base_rundir.replace(SKFlatRunlogDir,''))

## Get Sample Path

lines_SamplePath = open(DATA_DIR+"/Sample/SamplePath.txt").readlines()
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

  if not IsDATA and Samplepath_Section=="MC":
    if words[0]==args.InputSample:
      NtupleFilePath = words[1]
      break
    
for BaseDir in SampleBaseDirs:
  alias = BaseDir[0]
  path = BaseDir[1]
  if alias in NtupleFilePath:
    NtupleFilePath = NtupleFilePath.replace(alias,path+'/'+SKFlatV+'/')
if IsDATA:
  NtupleFilePath = NtupleFilePath+'period'+args.DataPeriod+'/'

## Get input file list ##

os.system('ls -1 '+NtupleFilePath+'/*.root > '+base_rundir+'/input_filelist.txt')
lines_files = open(base_rundir+'/input_filelist.txt').readlines()
NTotalFiles = len(lines_files)

if args.NJobs>NTotalFiles:
  args.NJobs = NTotalFiles

print "<SKFlat> NTotalFiles = "+str(NTotalFiles)
print "<SKFlat> NJobs = "+str(args.NJobs)
nfilepjob = int(NTotalFiles/args.NJobs)
print "<SKFlat> --> # of files per job = "+str(nfilepjob)
nfilepjob_remainder = NTotalFiles-(args.NJobs)*(nfilepjob)
if nfilepjob_remainder>=(args.NJobs):
  print 'nfilepjob_remainder = '+str(nfilepjob_remainder)
  print 'while, (args.NJobs) = '+str((args.NJobs))
  print '--> exit'
  sys.exit()

FileRanges = []
temp_end_largerjob = 0
nfile_checksum = 0
## First nfilepjob_remainder jobs will have (nfilepjob+1) files per job
for it_job in range(0,nfilepjob_remainder):
  FileRanges.append(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))
  temp_end_largerjob = (it_job+1)*(nfilepjob+1)
  nfile_checksum += len(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))
## Remaining (args.NJobs)-nfilepjob_remainder jobs will have (nfilepjob) files per job
for it_job in range(0,(args.NJobs)-nfilepjob_remainder):
  FileRanges.append(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))
  nfile_checksum += len(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))
print 'nfile_checksum = '+str(nfile_checksum)
print 'NTotalFiles = '+str(NTotalFiles)


## Write run script

CheckTotalNFile=0
for it_job in range(0,len(FileRanges)):

  #print "["+str(it_job)+"th]",
  #print FileRanges[it_job],
  #print " --> "+str(len(FileRanges[it_job]))

  CheckTotalNFile = CheckTotalNFile+len(FileRanges[it_job])

  thisjob_dir = base_rundir+'/job_'+str(it_job)+'/'
  os.system('mkdir -p '+thisjob_dir)

  out = open(thisjob_dir+'run.C','w')
  print>>out,'''R__LOAD_LIBRARY({1}/{0}_C.so)

void run(){{

  {0} m;

  TString outputdir = getenv("OUTPUTDIR");

  m.SetTreeName("recoTree/SKFlat");
'''.format(args.Analyzer, (base_rundir+'/libs').replace('///','/').replace('//','/'))

  if IsDATA:
    out.write('  m.IsThisDataFile = true;\n')
    out.write('  m.DataStream = "'+args.InputSample+'";\n')
  else:
    out.write('  m.MCSample = "'+args.InputSample+'";\n');
    out.write('  m.IsThisDataFile = false;\n')

  out.write('  m.SetOutfilePath("'+thisjob_dir+'/hists.root");\n')

  for it_file in FileRanges[it_job]:
    out.write('  m.AddFile("'+lines_files[it_file].strip('\n')+'");\n')

  print>>out,'''  m.Init();

  m.Loop();

  m.WriteHist();

}'''
  out.close()

  run_commands = open(thisjob_dir+'commands.sh','w')
  print>>run_commands,'''echo "[SKFlat.py] cmsset_default.sh"
source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "[SKFlat.py] Going to $CMSSW_BASE/src"
cd /data7/Users/jskim/CMSSW_9_4_4/src/
export SCRAM_ARCH=slc6_amd64_gcc630
echo "[SKFlat.py] Trying cmsenv"
cmsenv
cd {0}
echo "[SKFlat.py] Okay, let's run the analysis"
root -l -b -q run.C
echo "[SKFlat.py] JOB FINISHED!!"
'''.format(thisjob_dir)
  run_commands.close()

  jobname = 'job_'+str(it_job)+'_'+args.Analyzer
  cmd = 'qsub -V -q '+args.Queue+' -N '+jobname+' -wd '+thisjob_dir+' commands.sh '

  if not args.no_exec:
    cwd = os.getcwd()
    os.chdir(thisjob_dir)
    os.system(cmd+' > submitlog.log')
    os.chdir(cwd)
  sublog = open(thisjob_dir+'/submitlog.log','a')
  sublog.write('\nSubmission command was : '+cmd+'\n')
  sublog.close()

## Submission all done. Now check job status

import time
time.sleep(1)

from CheckJobStatus import *

## Write Kill Command

KillCommand = open(base_rundir+'/Script_JobKill.sh','w')
for it_job in range(0,len(FileRanges)):
  thisjob_dir = base_rundir+'/job_'+str(it_job)+'/'
  jobid = GetJobID(thisjob_dir, args.Analyzer, it_job)
  KillCommand.write('qdel '+jobid+' ## job_'+str(it_job)+' ##\n')
KillCommand.close()

AllDone = False
GotError = False

JobStartTime = datetime.datetime.now()
string_JobStartTime =  JobStartTime.strftime('%Y-%m-%d %H:%M:%S')

## Write Job status until it's done

from TimeTools import *

while not AllDone:
  AllDone = True
  statuslog = open(base_rundir+'/JobStatus.log','w')

  statuslog.write('Job submitted at '+string_JobStartTime+'\n')

  statuslog.write('JobNumber\t| Status\n')

  ToStatuslog = []
  n_eventran = 0
  finished = []
  EventDone = 0
  EventTotal = 0

  TotalEventRunTime = 0
  MaxTimeLeft = 0
  MaxEventRunTime = 0

  for it_job in range(0,len(FileRanges)):
    thisjob_dir = base_rundir+'/'
    this_status = ""
    this_status = CheckJobStatus(thisjob_dir, args.Analyzer, it_job)

    if "ERROR" in this_status:
      GotError = True
      statuslog.write("#### ERROR OCCURED ####\n")
      statuslog.write(this_status+'\n')
      break

    if "FINISHED" not in this_status:
      AllDone = False

    outlog = ""
    if "FINISHED" in this_status:
      finished.append("Finished")

      EventInfo = this_status.split()[1].split(':')

      this_EventDone = int(EventInfo[2])
      this_EventTotal = int(EventInfo[2])

      EventDone += this_EventDone
      EventTotal += this_EventTotal

      line_EventRunTime = this_status.split()[2]+' '+this_status.split()[3]
      this_jobstarttime = GetDatetimeFromMyFormat(line_EventRunTime)
      this_diff = datetime.datetime.now()-this_jobstarttime
      this_EventRunTime = 86400*this_diff.days+this_diff.seconds

      this_TimePerEvent = float(this_EventRunTime)/float(this_EventDone)
      this_TimeLeft = (this_EventTotal-this_EventDone)*this_TimePerEvent

      TotalEventRunTime += this_EventRunTime
      MaxTimeLeft = max(MaxTimeLeft,this_TimeLeft)
      MaxEventRunTime = max(MaxEventRunTime,this_EventRunTime)

    elif "RUNNING" in this_status:
      outlog = str(it_job)+'\t| '+this_status.split()[1]+' %'

      if len(this_status.split())<3 :
        print 'len(this_status.split())<3;; Priting this_status.split()'
        print this_status.split()

      EventInfo = this_status.split()[2].split(':')

      this_EventDone = int(EventInfo[1])
      this_EventTotal = int(EventInfo[2])

      EventDone += this_EventDone
      EventTotal += this_EventTotal

      line_EventRunTime = this_status.split()[3]+' '+this_status.split()[4]
      this_jobstarttime = GetDatetimeFromMyFormat(line_EventRunTime)
      this_diff = datetime.datetime.now()-this_jobstarttime
      this_EventRunTime = 86400*this_diff.days+this_diff.seconds

      if this_EventDone==0:
        this_EventDone = 1

      this_TimePerEvent = float(this_EventRunTime)/float(this_EventDone)
      this_TimeLeft = (this_EventTotal-this_EventDone)*this_TimePerEvent

      TotalEventRunTime += this_EventRunTime
      MaxTimeLeft = max(MaxTimeLeft,this_TimeLeft)
      MaxEventRunTime = max(MaxEventRunTime,this_EventRunTime)

      round_this_TimeLeft = round(this_TimeLeft,1)
      round_this_EventRunTime = round(this_EventRunTime,1)

      outlog += ' ('+str(round_this_EventRunTime)+' s ran, and '+str(round_this_TimeLeft)+' s left)'
      ToStatuslog.append(outlog)
      n_eventran += 1

    else:
      outlog = str(it_job)+'\t| '+this_status
      ToStatuslog.append(outlog)

  if GotError:
    break

  for l in ToStatuslog:
    statuslog.write(l+'\n')
  statuslog.write('\n==============================================================\n')
  statuslog.write('queue = '+args.Queue+'\n')
  statuslog.write(str(len(FileRanges))+' jobs submitted\n')
  statuslog.write(str(n_eventran)+' jobs are running\n')
  statuslog.write(str(len(finished))+' jobs are finished\n')

  ThisTime = datetime.datetime.now()
  string_ThisTime =  ThisTime.strftime('%Y-%m-%d %H:%M:%S')

  statuslog.write('EventDone = '+str(EventDone)+'\n')
  statuslog.write('EventTotal = '+str(EventTotal)+'\n')
  statuslog.write('EventLeft = '+str(EventTotal-EventDone)+'\n')
  statuslog.write('TotalEventRunTime = '+str(TotalEventRunTime)+'\n')
  statuslog.write('MaxTimeLeft = '+str(MaxTimeLeft)+'\n')
  statuslog.write('MaxEventRunTime = '+str(MaxEventRunTime)+'\n')

  t_per_event = 1
  if EventDone is not 0:
    t_per_event = float(TotalEventRunTime)/float(EventDone)
  statuslog.write('t_per_event = '+str(t_per_event)+'\n')

  EstTime = ThisTime+datetime.timedelta(0, MaxTimeLeft)

  statuslog.write('Estimated Finishing Time : '+EstTime.strftime('%Y-%m-%d %H:%M:%S')+'\n')
  statuslog.write('Last checked at '+string_ThisTime+'\n')
  statuslog.close()

  ## copy statuslog to webdir
  os.system('cp '+base_rundir+'/JobStatus.log '+webdir)
  os.system('scp -r '+webdir+' jskim@147.47.242.71:/var/www/html/SKFlatAnalyzerJobLogs/')
  os.system('ssh -Y jskim@147.47.242.71 chmod -R 777 /var/www/html/SKFlatAnalyzerJobLogs/'+base_rundir.replace(SKFlatRunlogDir,''))
  #os.system("ssh -Y jskim@147.47.242.71 source /var/www/html/SKFlatAnalyzerJobLogs/run.sh")

  time.sleep(5)

## Job is finished
## copy output

outputname = args.Analyzer+'_'+args.InputSample
if IsDATA:
  outputname += '_'+args.DataPeriod

if not GotError:
  cwd = os.getcwd()
  os.chdir(thisjob_dir)
  os.system('hadd -f '+outputname+'.root job_*/*.root >> JobStatus.log')

  os.system('rm job_*/*.root')

  ## Final Outputpath

  FinalOutputPath = args.Outputdir
  if args.Outputdir=="":
    FinalOutputPath = SKFlatOutputDir+'/'+args.Analyzer+'/'
    if IsDATA:
      FinalOutputPath += '/DATA/'

  os.system('mkdir -p '+FinalOutputPath)
  os.system('cp '+outputname+'.root '+FinalOutputPath)
  os.chdir(cwd)

## Send Email now

from SendEmail import SendEmail
JobFinishEmail = '''#### Job Info ####
Analyzer = {0}
InputSample = {1}
OutputDir = {2}
'''.format(args.Analyzer,args.InputSample,FinalOutputPath)
if IsDATA:
  JobFinishEmail += "DataPeriod = "+args.DataPeriod+"\n"
JobFinishEmail += '''##################
Job started at {0}
Job finished at {1}
'''.format(string_JobStartTime,string_ThisTime)
SendEmail('jskim','jae.sung.kim@cern.ch','Job Summary',JobFinishEmail)









