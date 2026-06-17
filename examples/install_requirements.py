#!/usr/bin/env python3
import platform
import sys, os, subprocess
import argparse
import re
import platform
from subprocess import CalledProcessError, DEVNULL
import textwrap


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
        subprocess.check_call([sys.executable, "-c", "import numpy, cv2;"], stderr=DEVNULL)
        requireOpenCv = False
    except subprocess.CalledProcessError as ex:
        requireOpenCv = True

if requireOpenCv:
    DEPENDENCIES.append('numpy<3.0')
    # 4.5.4.58 package is broken for python 3.9
    if sys.version_info[0] == 3 and sys.version_info[1] == 9:
        DEPENDENCIES.append('opencv-python!=4.5.4.58')
    else:
        DEPENDENCIES.append('opencv-python<5.0')



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
    try:
        subprocess.check_call(pip_update_cmd)
    except CalledProcessError as e:
        print(f"\n\n\033[31m\033[1m[Warning]\033[0m An error occurred while trying to update pip: {e}\n")
        print("This likely stems from the fact that you're not using a Python virtual environment.")
        venv_creation_instructions = textwrap.dedent(f"""\
            \033[94m\033[1m
            Here's how you can create and activate a virtual environment:
            
            1. Create a virtual environment:
               - For Linux or MacOS, use: python3 -m venv {parent_dir}/.env
               - For Windows, use: python -m venv {parent_dir}/.env
            
            2. Activate the virtual environment:
               - For Linux or MacOS, use: source {parent_dir}/.env/bin/activate
               - For Windows, use: {parent_dir}/.env/Scripts/activate
            
            Once activated, you'll be working within the virtual environment. You can then attempt to re-run this script.
            To exit the virtual environment when you're done, simply use the command: deactivate
            
            For more detailed instructions, please refer to the official Python documentation on virtual environments: https://docs.python.org/3/tutorial/venv.html
            \033[0m
            """)
        
        print(textwrap.indent(venv_creation_instructions, '    '))
        exit(0)


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
        depthai_install_cmd = [*pip_package_install, '-U', '--force-reinstall', 'depthai~=2.0']
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

    nn_model_configs = {
        "mobilenet-ssd": {
            "shaves": [5, 6, 8],
            "compile_params": ["-ip U8"],
            "zoo_type": "intel",
            "default_ov_version": "2021.4"
        },
        "person-detection-retail-0013": {
            "shaves": [7],
            "compile_params": ["-ip U8"],
            "zoo_type": "intel",
            "default_ov_version": "2021.4"
        },
        "yolo-v4-tiny-tf": {
            "shaves": [6],
            "compile_params": ["-ip U8"],
            "zoo_type": "intel",
            "default_ov_version": "2021.4"
        },
        "yolo-v3-tiny-tf": {
            "shaves": [6],
            "compile_params": ["-ip U8"],
            "zoo_type": "intel",
            "default_ov_version": "2021.4"
        },
        "yolov6n_thermal_people_256x192": {
            "shaves": [6],
            "compile_params": ["-ip FP16"],
            "zoo_type": "depthai",
            "default_ov_version": "2022.1"
        },
          "yolov8n_coco_640x352": {
          "shaves": [6],
          "compile_params": ["-ip U8"],
          "zoo_type": "depthai",
          "default_ov_version": "2021.4"
        },
    }

    blobconverter_cmds = [
        [sys.executable, 
        "-m", "blobconverter", 
        "-zn", nn_name, 
        "-sh", str(nn_shave), 
        "-o", f"{examples_dir}/models", 
        "-zt", nn_model_configs[nn_name]["zoo_type"],
        *(["--compile-params", " ".join(nn_model_configs[nn_name]["compile_params"])] if nn_model_configs[nn_name]["compile_params"] else []),
        *(["-v", args.convert] if args.convert != convert_default else ["-v", nn_model_configs[nn_name]["default_ov_version"]])]
        for nn_name in nn_model_configs
        for nn_shave in nn_model_configs[nn_name]["shaves"]
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
