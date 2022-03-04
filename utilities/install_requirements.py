import subprocess
import sys


in_venv = getattr(sys, "real_prefix", getattr(sys, "base_prefix", sys.prefix)) != sys.prefix
pip_call = [sys.executable, "-m", "pip"]
pip_install = pip_call + ["install"]

if not in_venv:
    pip_install.append("--user")

subprocess.check_call([*pip_install, "pip", "-U"])
subprocess.check_call([*pip_call, "uninstall", "depthai", "--yes"])
subprocess.check_call([*pip_install, "-r", "requirements.txt"])
try:
    subprocess.check_call([*pip_install, "-r", "requirements-optional.txt"])
    if sys.platform == "linux":
        print(f'$ sudo apt install python3-tk')
except subprocess.CalledProcessError as ex:
    print(f"Optional dependencies were not installed (exit code {ex.returncode})")