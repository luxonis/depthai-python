#!/bin/sh
set -ex
cd $(mktemp -d)
unzip $1
DYLD_FALLBACK_LIBRARY_PATH=/usr/local/lib delocate-path -L palace.dylibs .
mkdir -p $2
python -m wheel pack . -d $2
tempdir=$(pwd)
cd -
rm -rf $tempdir