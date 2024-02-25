import os
from pathlib import Path
import subprocess

here = os.path.dirname(os.path.realpath(__file__))

if os.path.exists(os.path.join(here, "cam_test.py")):  # Installed package
    CAM_TEST_PATH = Path(here) / "cam_test.py"
else:
    CAM_TEST_PATH = (
        Path(here) / ".." / "utilities" / "cam_test.py"
    )  # Execution from source
CAM_TEST_PATH = str(CAM_TEST_PATH)


def cli():
    import argparse
    import sys
    import depthai as dai

    parser = argparse.ArgumentParser(description="DepthAI CLI")
    parser.add_argument(
        "-v", "--version", action="store_true", help="Print version and exit."
    )
    parser.add_argument(
        "-l", "--list-devices", action="store_true", help="List connected devices."
    )
    parser.add_argument("cam_test", nargs="?", help="Run camera test.", default=False)

    args, _unknown_args = parser.parse_known_args()
    if args.version:
        print(dai.__version__)
    elif args.list_devices:
        print(dai.Device.getAllConnectedDevices())
    elif args.cam_test:
        print("sysargs:", sys.argv, len(sys.argv))
        print("SYSARGS[2:], ", sys.argv[2:])
        subprocess.run([sys.executable, CAM_TEST_PATH, *sys.argv[2:]])
    elif len(sys.argv) == 1:
        parser.print_help()


if __name__ == "__main__":
    cli()
