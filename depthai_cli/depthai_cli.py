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


def cli() -> int:
    import argparse
    import sys
    import depthai as dai
    parser = argparse.ArgumentParser(description="DepthAI CLI", add_help=True)
    parser.add_argument("-v", "--version", action="store_true", help="Print version and exit.")
    parser.add_argument("-l", "--list-devices", action="store_true", help="List connected devices.")
    subparsers = parser.add_subparsers(dest="command", help="Sub-commands")
    # Define the parser for the "cam_test" command
    cam_test_parser = subparsers.add_parser("cam_test", help="Commands and options for cam_test", add_help=False)
    cam_test_parser.add_argument("args", nargs=argparse.REMAINDER, help="Arguments to pass to cam_test")

    # subparser REMINDER args would get parsed too if we used parse_args, so we have to handle unknown args manually
    args, unknown_args = parser.parse_known_args()
    if args.command == "cam_test":
        cam_test_path = CAM_TEST_PATH
        return subprocess.run([sys.executable, cam_test_path] + cam_test_parser.parse_args().args[1:]).returncode
    # Parse other subcommands here
    elif unknown_args:
        parser.error(f"Unrecognized arguments: {unknown_args}") # handles exit internally
    elif args.version:
        print(dai.__version__)
        return 0
    elif args.list_devices:
        print(dai.Device.getAllConnectedDevices())
        return 0
    else:
        # No recognized commands, print help
        parser.print_help()
        return 1


if __name__ == "__main__":
    exit(cli())
