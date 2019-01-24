# SKFlatAnalyzer

## Manual

https://jskim.web.cern.ch/jskim/SKFlat/Manual/Manual_SKFlat.pdf

## First time setup
```
cp setup_tmp.sh setup.sh
```

Edit setup.sh :
```
export SKFlatLogEmail='' # <- put your email address here
```

Making libraries
```
make clean
make
```

## Initializing
Everytime when using new shell,
```bash
source setup.sh
# If in KNU, need proxy
# voms-proxy-init --voms cms -valid 24:00
=======
make
voms-proxy-init --voms cms -valid 24:00
```

## Test job
```bash
SKFlat.py -a ExampleRun -i DYJets -n 50 &
```

## SMPValidation job
```bash
source SMPValidationSubmit.sh
```

### SMPValidation plotting
Edit TString filedir in plot.cc  
Draw a plot
```bash
root -l
.L plot.cc
Setup("electron2017")
GetCompareStack("electron2017/OS_Z/dimass",0,0,80,100)
```
Save plots
```bash
root -b
.L plot.cc
Setup("electron2017")
SaveAll()
Setup("muon2017")
SaveAll()
```

## Making a new Ananlyzer
```bash
cd script/MakeCycleSkeleton/
```
Edit MakeCycleSkeleton.py :
```bash
cyclename = "" # <- put new Analyzer name
```
Then, run
```bash
python MakeCycleSkeleton.py
```
It will print below lines :
```bash
mv NewAnalyzer.h $SKFlat_WD/Analyzers/include/
mv NewAnalyzer.C $SKFlat_WD/Analyzers/src/
```
First two lines will move the files to inclue/ and src/ directory

Last line is to run a job with the machine, not batch.

This is good for test your analyzer

## Detailed descriptions

Look Analyzers/src/ExampleRun.C

# How To update your forked repo to follow the original repository

To get your forked repo from this one
```
push "Fork" button on the right-top side of this web page
```

Go to you forked repo webpage and copy the address of it, then clone

How to update your forked repo to follow the original repository
```
git remote add upstream https://github.com/CMSSNU/SKFlatAnalyzer
git fetch upstream
git pull upstream master
```
