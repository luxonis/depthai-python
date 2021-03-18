#!/usr/bin/env python3

# Installs wheels for current develop version 
import sys, os, subprocess
parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(parent_dir)
sys.path.insert(1, parent_dir)
import find_version

# Get package version
version = find_version.get_package_version()
git_commit = subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('UTF-8').strip()
final_version = version + "+" + git_commit

# Install wheel
ARTIFACTORY_URL = 'https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local'
subprocess.call([sys.executable, "-m", "pip", "install", "opencv-python", "--extra-index-url", ARTIFACTORY_URL, "depthai=="+final_version])
