#!/bin/bash

set -e

cmake --build build --parallel --target sphinx

# fix missing index.css file
cp /app/docs/source/_static/css/* /app/build/docs/sphinx/_static/css
