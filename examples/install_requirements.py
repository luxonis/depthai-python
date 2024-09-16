#!/usr/bin/env python3
import platform
import sys, os, subprocess
import argparse
import re
import platform

convert_default = "empty"
parser = argparse.ArgumentParser()
parser.add_argument('-sdai', "--skip_depthai", action="store_true", help="Skip installation of depthai library.")
parser.add_argument('-dr', "--dry_run", action="store_true", help="Print commands without executing.")
parser.add_argument("--convert", nargs="?", default=convert_default, help="Convert the NN blobs using BlobConverter. Can be used as --convert 2021.4 to convert using OpenVINO 2021.4 or just --convert to use latest OpenVINO release")

def prettyPrint(command):

    def hasWhitespace(string):
        return (len(string) != len(re.sub('[\s+]', '', string)))

    stringBuilder = str()
    for i, item in enumerate(command):
        if(hasWhitespace(item)):
            stringBuilder += ' "' + item + '"'
        else:
            if i == 0:
                prefix = ''
            else:
                prefix = ' '
            stringBuilder += prefix + item
    print(stringBuilder)


args = parser.parse_args()

examples_dir = os.path.dirname(os.path.abspath(__file__))

parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(parent_dir)
sys.path.insert(1, parent_dir)
import find_version

# 3rdparty dependencies to install
DEPENDENCIES = ['pyyaml', 'requests']
requireOpenCv = True
thisPlatform = platform.machine()
if thisPlatform == "aarch64":
    # try to import opencv, numpy in a subprocess, since it might fail with illegal instruction
    # if it was previously installed w/ pip without setting OPENBLAS_CORE_TYPE=ARMV8 env variable
    try:
        subprocess.check_call([sys.executable, "-c", "import numpy, cv2;"])
        requireOpenCv = False
    except subprocess.CalledProcessError as ex:
        requireOpenCv = True

if requireOpenCv:
    DEPENDENCIES.append('numpy==1.26.4')
    # 4.5.4.58 package is broken for python 3.9
    if sys.version_info[0] == 3 and sys.version_info[1] == 9:
        DEPENDENCIES.append('opencv-python!=4.5.4.58')
    else:
        DEPENDENCIES.append('opencv-python')



# Constants
ARTIFACTORY_URL = 'https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local'

# Check if in virtual environment
in_venv = getattr(sys, "real_prefix", getattr(sys, "base_prefix", sys.prefix)) != sys.prefix
pip_call = [sys.executable, "-m", "pip"]
pip_installed = True
pip_install = pip_call + ["install", "-U"]
pip_package_install = pip_install + ["--prefer-binary"]

try:
    subprocess.check_call(pip_call + ["--version"])
except subprocess.CalledProcessError as ex:
    pip_installed = False

if not pip_installed:
    err_str = "Issues with \"pip\" package detected! Follow the official instructions to install - https://pip.pypa.io/en/stable/installation/"
    raise RuntimeError(err_str)

if sys.version_info[0] != 3:
    raise RuntimeError("Examples require Python 3 to run (detected: Python {})".format(sys.version_info[0]))

is_pi = thisPlatform.startswith("arm")
prebuiltWheelsPythonVersion = [7,9]
if requireOpenCv and is_pi and sys.version_info[1] not in prebuiltWheelsPythonVersion:
    print("[WARNING] There are no prebuilt wheels for Python 3.{} for OpenCV, building process on this device may be long and unstable".format(sys.version_info[1]))

if not in_venv:
    pip_install.append("--user")
    pip_package_install.append("--user")

# Update pip
pip_update_cmd = [*pip_install, "pip"]
if args.dry_run:
    prettyPrint(pip_update_cmd)
else:
    subprocess.check_call(pip_update_cmd)
# Install python dependencies
python_dependencies_cmd = [*pip_package_install, *DEPENDENCIES]
if args.dry_run:
    prettyPrint(python_dependencies_cmd)
else:
    subprocess.check_call(python_dependencies_cmd)

if not args.skip_depthai:
    # Check if in git context and retrieve some information
    git_context = True
    git_commit = ""
    git_branch = ""
    try:
        git_commit = subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('UTF-8').strip()
        git_branch = subprocess.check_output(['git', 'rev-parse', '--abbrev-ref', 'HEAD']).decode('UTF-8').strip()
    except (OSError, subprocess.CalledProcessError) as e:
        git_context = False

    # Install depthai depending on context
    if not git_context or git_branch == 'main':
        # Install latest pypi depthai release
        depthai_install_cmd = [*pip_package_install, '-U', '--force-reinstall', 'depthai']
        if args.dry_run:
            prettyPrint(depthai_install_cmd)
        else:
            subprocess.check_call(depthai_install_cmd)

    elif git_context:
        try:
            subprocess.check_output(['git', 'submodule', 'update', '--init', '--recursive'])
        except (OSError, subprocess.CalledProcessError) as e:
            print("git submodule update failed!")
            raise
        # Get package version if in git context
        final_version = find_version.get_package_dev_version(git_commit)
        # Install latest built wheels from artifactory (0.0.0.0+[hash] or [version]+[hash])
        commands = [[*pip_package_install, "--extra-index-url", ARTIFACTORY_URL, "depthai=="+final_version],
                    [*pip_package_install, "."]]
        success = False
        for command in commands:
            try:
                if args.dry_run:
                    prettyPrint(command)
                    success = True
                else:
                    success = subprocess.call(command) == 0
            except (OSError, subprocess.CalledProcessError) as e:
                success = False
            if success:
                break

        # If all commands failed
        if not success:
            print("Couldn't install dependencies as wheels and trying to compile from sources failed")
            print("Check https://github.com/luxonis/depthai-python#dependencies on retrieving dependencies for compiling from sources")

downloader_cmd = [sys.executable, f"{examples_dir}/downloader/downloader.py", "--all", "--cache_dir", f"{examples_dir}/downloader/", "--num_attempts", "5", "-o", f"{examples_dir}/models"]
if args.dry_run:
    prettyPrint(downloader_cmd)
else:
    subprocess.check_call(downloader_cmd)

if args.convert != convert_default:
    nn_models_shaves = {
        "mobilenet-ssd": [5, 6, 8],
        "person-detection-retail-0013": [7],
        "yolo-v4-tiny-tf": [6],
        "yolo-v3-tiny-tf": [6],
    }
    blobconverter_cmds = [
        [sys.executable, "-m", "blobconverter", "-zn", nn_name, "-sh", str(nn_shave), "-o", f"{examples_dir}/models", *(["-v", args.convert] if args.convert is not None else [])]
        for nn_name in nn_models_shaves
        for nn_shave in nn_models_shaves[nn_name]
    ]
    install_blobconverter_cmd = [*pip_package_install, "blobconverter"]
    for cmd in [install_blobconverter_cmd] + blobconverter_cmds:
        if args.dry_run:
            prettyPrint(cmd)
        else:
            subprocess.check_call(cmd)

if requireOpenCv and thisPlatform == "aarch64":
    from os import environ
    OPENBLAS_CORE_TYPE = environ.get('OPENBLAS_CORE_TYPE')
    if OPENBLAS_CORE_TYPE != 'ARMV8':
        WARNING='\033[1;5;31m'
        RED='\033[91m'
        LINE_CL='\033[0m'
        SUGGESTION='echo "export OPENBLAS_CORETYPE=ARMV8" >> ~/.bashrc && source ~/.bashrc'
        print(f'{WARNING}WARNING:{LINE_CL} Need to set OPENBLAS_CORE_TYPE environment variable, otherwise opencv will fail with illegal instruction.')
        print(f'Run: {RED}{SUGGESTION}{LINE_CL}')
