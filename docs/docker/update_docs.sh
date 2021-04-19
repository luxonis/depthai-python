#!/bin/bash

set -e

cmake --build build --target doxygen sphinx --parallel
