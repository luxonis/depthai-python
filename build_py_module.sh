#!/bin/bash

INITIAL_DIR=`pwd`
SOURCE_DIR=$(dirname "$0")
BUILD_DIR=build
UNIX_NAME=`uname`

# Default 'make' to a number of jobs equal to CPU threads, but lower it
# if the memory available is too low, to avoid a lock up (e.g for RPi)
NUM_JOBS=`nproc`

if [[ $UNIX_NAME = "Darwin" ]]; then
    MEM_AVAIL=`sysctl hw.memsize | awk '{print int($2/1024/1024)}'`
else
    MEM_AVAIL=`grep MemAvailable /proc/meminfo | awk '{print int($2/1024)}'`
fi

echo "MemAvailable: $MEM_AVAIL MB"
if [ -z $MEM_AVAIL ]; then echo "Unable to get MemAvailable"
elif [[ $MEM_AVAIL -lt  900 ]]; then NUM_JOBS=1
elif [[ $MEM_AVAIL -lt 1400 ]]; then NUM_JOBS=2
fi

print_exec() {
    REL_PATH="realpath --relative-to=$INITIAL_DIR `pwd`"
    echo "[BUILD `$REL_PATH`] $@"
    "$@"
}

cd "$SOURCE_DIR/host/py_module"

if  [[ $1 = "--clean" ]]; then
    print_exec rm -r $BUILD_DIR
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR

if [ ! -f Makefile ]; then
    print_exec cmake ..
fi

print_exec make -j$NUM_JOBS

DEPTHAI_LIB=(*.so)
print_exec cp ${DEPTHAI_LIB[0]} ../../../../

