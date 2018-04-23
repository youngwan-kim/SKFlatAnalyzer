#!/bin/bash
export PLOTTER_WORKING_DIR=`pwd`
export OUTPUTDIR=$PLOTTER_WORKING_DIR/outputs/
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PLOTTER_WORKING_DIR/include/:$PLOTTER_WORKING_DIR/src/
export SKFlatV="v9-4-4.1"
export SKFlatAnV="v9-4-4.1__1"
export DATA_DIR=$PLOTTER_WORKING_DIR/data/$SKFlatAnV
mkdir -p $DATA_DIR
export SKFlatRunlogDir="/data7/Users/jskim/SKFlatRunlog/"
export MYBIN=$PLOTTER_WORKING_DIR/bin/
export PATH=${MYBIN}:${PATH}
