#!/bin/bash
export SKFlat_WD=`pwd`
export OUTPUTDIR=$SKFlat_WD/outputs/
export SKFlat_LIB_PATH=$SKFlat_WD/lib/

export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$SKFlat_WD/include/:$SKFlat_WD/src/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SKFlat_LIB_PATH

export SKFlatV="v946p1_2"
export SKFlatAnV="v946p1_2__Default"
export SAMPLE_DATA_DIR=$SKFlat_WD/data/$SKFlatAnV
mkdir -p $SAMPLE_DATA_DIR

export SKFlatSEDir="/xrootd/store/user/jskim/SKFlatOutput/"

if [[ $HOSTNAME == *"ui10.sdfarm.kr"* ]]; then
  echo "Working on KISTI"
  export SKFlatRunlogDir="/cms/scratch/jskim/SKFlatRunlog/"
  export SKFlatOutputDir="/cms/scratch/jskim/SKFlatOutput/"
  export DATA_DIR=data/$SKFlatAnV

  export CMS_PATH=/cvmfs/cms.cern.ch
  source $CMS_PATH/cmsset_default.sh
  export SCRAM_ARCH=slc6_amd64_gcc630
  cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/src/
  eval `scramv1 runtime -sh`
  cd -
  source /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/external/slc6_amd64_gcc630/bin/thisroot.sh

else
  echo "Working on 42cluster"
  export SKFlatRunlogDir="/data7/Users/jskim/SKFlatRunlog/"
  export SKFlatOutputDir="/data7/Users/jskim/SKFlatOutput/"
  export DATA_DIR=$SKFlat_WD/data/$SKFlatAnV
fi
export MYBIN=$SKFlat_WD/bin/
export PATH=${MYBIN}:${PATH}
