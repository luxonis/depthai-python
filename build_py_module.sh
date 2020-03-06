#!/bin/bash

INITIAL_DIR=`pwd`
SOURCE_DIR=$(dirname "$0")
BUILD_DIR=build
NUM_CORES=`nproc`

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

print_exec make -j$NUM_CORES

DEPTHAI_LIB=(*.so)
print_exec cp ${DEPTHAI_LIB[0]} ../../../../

