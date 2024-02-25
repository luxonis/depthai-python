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
    parser = argparse.ArgumentParser(description="DepthAI CLI", add_help=False)
    parser.add_argument("-v", "--version", action="store_true", help="Print version and exit.")
    parser.add_argument("-l", "--list-devices", action="store_true", help="List connected devices.")
    parser.add_argument("commands", nargs=argparse.REMAINDER, help="Commands and options for cam_test")

    args = parser.parse_args()
    if args.version:
        print(dai.__version__)
        return
    if args.list_devices:
        print(dai.Device.getAllConnectedDevices())
        return

    # Handle cam_test command
    if args.commands and args.commands[0] == "cam_test":
        cam_test_path = CAM_TEST_PATH
        # Forward the arguments to cam_test.py, excluding the "cam_test" command itself
        subprocess.run([sys.executable, cam_test_path] + args.commands[1:])
    else:
        # No recognized commands, print help
        parser.print_help()
        print("To run cam_test, use: depthai cam_test [options for cam_test]")

if __name__ == "__main__":
    cli()
