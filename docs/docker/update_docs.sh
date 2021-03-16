#!/bin/bash

set -e

rm -rf build/docs/sphinx
cmake -S . -B build -D DEPTHAI_BUILD_DOCS=ON -D DEPTHAI_PYTHON_BUILD_DOCS=ON -D ENABLE_PRECOMPILED_HEADERS=OFF
cmake --build build --parallel --target sphinx
