import os

#commands.sh  job_0_GetEffLumi.e6409440  job_0_GetEffLumi.o6409440  run.C  submitlog.log

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
    return "RUNNING 0"

  ## 2) Job Finished
  if FinishString in LASTLINE:
    return "FINISHED"

  ## 3) "Processing run.C" not yet done
  elif "SKFlatNtuple::Loop" in LASTLINE:
    # [SKFlatNtuple::Loop] 2011000/38777460 (5.186 %)
    perct =  LASTLINE.split()[2].strip('(')
    return "RUNNING "+perct
  else:
    return "WTF"

#print CheckJobStatus('/data7/Users/jskim/SKFlatRunlog/2018_04_26_142127__DYJets_10to50_MG/', 'GetEffLumi', 0)
