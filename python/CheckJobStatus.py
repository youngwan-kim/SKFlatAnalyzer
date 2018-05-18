import os

#commands.sh  job_0_GetEffLumi.e6409440  job_0_GetEffLumi.o6409440  run.C  submitlog.log

def GetEventDone(l):
  # [SKFlatNtuple::Loop] 1185000/1207307 (98.1523 %) @ 2018-5-16 11:0:9
  w = l.split()[1]
  nums = w.split('/')

  if len(nums)<2:
    print nums

  return str(nums[0])+':'+str(nums[1])

def GetJobID(logfiledir, cycle, jobnumber):
  jobid = open(logfiledir+'/submitlog.log').readlines()[0].split()[2]
  return jobid

def CheckJobStatus(logfiledir, cycle, jobnumber):
  FinishString = "[SKFlat.py] JOB FINISHED"

  jobid = open(logfiledir+'/job_'+str(jobnumber)+'/submitlog.log').readlines()[0].split()[2]

  path_log_e = logfiledir+'job_'+str(jobnumber)+'/job_'+str(jobnumber)+'_'+cycle+'.e'+jobid
  path_log_o = logfiledir+'job_'+str(jobnumber)+'/job_'+str(jobnumber)+'_'+cycle+'.o'+jobid

  if (not os.path.exists(path_log_e)) or (not os.path.exists(path_log_o)):
    return "BATCH JOB NOT STARTED"

  log_e = open(path_log_e).readlines()
  if len(log_e) > 0:
    out = 'ERROR\n'
    out += '--------------------------------------\n'
    out += 'logfile : '+path_log_o+'\n'
    out += '--------------------------------------\n'
    for l in log_e:
      out = out+l
    return out

  log_o = open(path_log_o).readlines()

  ## XX.oXX not created
  if len(log_o) == 0:
    return "BATCH LOG NOT CREATED"

  IsCycleRan = False
  for l in log_o:
    if "Processing run.C" in l:
      IsCycleRan = True
      break

  ## XX.oXX exists

  ## "Processing run.C" not yet done
  if not IsCycleRan:
    return "ANALYZER NOT STARTED"

  LASTLINE = log_o[-1]
  if "Processing run.C" in LASTLINE:
    return "EVENT NOT STARTED"

  if "Event Loop Started" in LASTLINE:
    return "EVENT NOT STARTED"

  line_JobStart = ""
  for l in log_o:
    # [SKFlatNtuple::Loop] Event Loop Started 2018-05-17 19:51:10
    if "Event Loop Started" in l:
      line_JobStart = l.replace("[SKFlatNtuple::Loop] Event Loop Started ","")
      break
  ForTimeEst = LASTLINE

  ## 2) Job Finished
  if FinishString in LASTLINE:
    ForTimeEst = log_o[-2]
    EventDone = GetEventDone(ForTimeEst)
    return "FINISHED"+"\tEVDONE:"+EventDone+"\t"+line_JobStart

  ## 3) Running
  elif "SKFlatNtuple::Loop" in LASTLINE:
    # [SKFlatNtuple::Loop] 2011000/38777460 (5.186 %)
    perct =  LASTLINE.split()[2].strip('(')
    EventDone = GetEventDone(ForTimeEst)
    return "RUNNING\t"+perct+"\tEVDONE:"+EventDone+"\t"+line_JobStart
  else:
    return "WTF"

