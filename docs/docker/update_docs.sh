#!/bin/bash

set -e

rm -rf build/docs/sphinx
cmake -S . -B build -D DEPTHAI_BUILD_DOCS=ON -D DEPTHAI_PYTHON_BUILD_DOCS=ON -D ENABLE_PRECOMPILED_HEADERS=OFF
cmake --build build --parallel --target sphinx

# fix missing index.css file
cp /app/docs/source/_static/css/* /app/build/docs/sphinx/_static/css
