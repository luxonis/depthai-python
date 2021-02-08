# readthedocs CI build limitation circumvent
import subprocess
import os

# Build library and generate actual conf.py
current_dir = os.path.dirname(os.path.realpath(__file__))
subprocess.run(["cmake -P ci.cmake"], cwd=current_dir, shell=True, check=True)

# Load generated conf.py which includes all needed information to build with sphinx
with open("../../build/docs/conf.py") as infile:
    exec(infile.read())
