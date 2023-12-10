#!/bin/bash
# This runs tests for the python part of DepthAI

MYPYPATH=tests-python python -m unittest discover -s tests-python/ "$@"
