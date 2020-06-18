#!/bin/sh
set -ex
cd $(mktemp -d)
unzip $1
DYLD_FALLBACK_LIBRARY_PATH=/usr/local/lib delocate-path -L palace.dylibs .
wheel=$(basename $1)
zip -r $wheel *
mkdir -p $2
mv $wheel $2
tempdir=$(pwd)
cd -
rm -rf $tempdir