#!/bin/bash
export SKFlat_WD=`pwd`
export OUTPUTDIR=$SKFlat_WD/outputs/
export SKFlat_LIB_PATH=$SKFlat_WD/lib/

#export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$SKFlat_WD/Analysis/include/:$SKFlat_WD/PhysicsObjects/include/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$SKFlat_WD/include/:$SKFlat_WD/src/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SKFlat_LIB_PATH

export SKFlatV="v944_3"
export SKFlatAnV="v944_3__Default"
export DATA_DIR=$SKFlat_WD/data/$SKFlatAnV
mkdir -p $DATA_DIR
export SKFlatRunlogDir="/data7/Users/jskim/SKFlatRunlog/"
export MYBIN=$SKFlat_WD/bin/
export PATH=${MYBIN}:${PATH}
