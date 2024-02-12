def cli():
    import argparse
    import sys
    import depthai as dai

    parser = argparse.ArgumentParser(description="DepthAI CLI")
    parser.add_argument("-v", "--version", action="store_true", help="Print version and exit.")
    parser.add_argument("-l", "--list-devices", action="store_true", help="List connected devices.")

    args = parser.parse_args()
    if args.version:
        print(dai.__version__)
    elif args.list_devices:
        print(dai.Device.getAllConnectedDevices())
    elif len(sys.argv) == 1:
        parser.print_help()


if __name__ == "__main__":
    cli()
