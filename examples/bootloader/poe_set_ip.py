import depthai as dai

import argparse
from enum import Enum
from ipaddress import ip_address, ip_network


class Modes(str, Enum):
    IP_STATIC = "static"
    IP_DYNAMIC = "dynamic"
    CLEAR = "clear"
    GET = "get"


def str_to_ip(ip: str) -> int:
    """Converts an IPv4 string to an integer."""
    return sum([int(x) << (8 * i) for i, x in enumerate(ip.split("."))])


def main(mode: Modes, ip: str = "", mask: str = "", gateway: str = "") -> None:
    (found, info) = dai.DeviceBootloader.getFirstAvailableDevice()
    if not found:
        raise ValueError("No device found!")
    # TODO: Support IPv6
    with dai.DeviceBootloader(info) as bl:
        try:
            # Read config, if it exists
            conf = bl.readConfig()
        except RuntimeError:
            # Create a default config if one doesn't exist
            conf = dai.DeviceBootloader.Config()

        if mode == Modes.GET:
            print(
                f"Device (IPv4): {conf.getIPv4()}, Mask: {conf.getIPv4Mask()}, Gateway: {conf.getIPv4Gateway()}"
            )
            return
        if mode in {Modes.IP_STATIC, Modes.IP_DYNAMIC}:
            is_ip_static = mode == Modes.IP_STATIC
            if not gateway:
                conf_d = conf.toJson()
                conf_d.update(
                    {
                        "network": {
                            "ipv4": str_to_ip(ip),
                            "ipv4Mask": str_to_ip(mask),
                            "staticIpv4": is_ip_static,
                        }
                    }
                )
                conf = dai.DeviceBootloader.Config.fromJson(conf_d)
            else:
                set_ip = conf.setStaticIPv4 if is_ip_static else conf.setDynamicIPv4
                set_ip(ip, mask, gateway)
            (success, error) = bl.flashConfig(conf)
        else:
            (success, error) = bl.flashConfigClear()

        if not success:
            print(f"Flashing failed: {error}")
        else:
            print(f"Flashing successful.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="PoE set IP",
        description="You can set a static, dynamic IPv4 of the device or clear the flash config.",
    )
    subparsers = parser.add_subparsers(dest="mode", required=True, help="Choose a mode")
    validate_ip = lambda ip: str(ip_address(ip))

    ip_parser = subparsers.add_parser(
        Modes.IP_STATIC.value, description="Set a static IPv4 address"
    )
    ip_parser.add_argument("ip", type=validate_ip, help="Device IPv4 address")
    ip_parser.add_argument("mask", type=validate_ip, help="Network mask")
    ip_parser.add_argument(
        "-g", "--gateway", type=validate_ip, help="Gateway IPv4 address"
    )
    ip_parser = subparsers.add_parser(
        Modes.IP_DYNAMIC.value, description="Set a dynamic IPv4 address"
    )
    ip_parser.add_argument("ip", type=validate_ip, help="Device IPv4 address")
    ip_parser.add_argument("mask", type=validate_ip, help="Network mask")
    ip_parser.add_argument(
        "-g", "--gateway", type=validate_ip, help="Gateway IPv4 address"
    )
    ip_parser = subparsers.add_parser(
        Modes.CLEAR.value, description="Clear flash configuration"
    )
    ip_parser = subparsers.add_parser(
        Modes.GET.value, description="Get IP configuration"
    )
    args = parser.parse_args()

    # Check if device IP is on the same network as the gateway
    if (
        hasattr(args, "gateway")
        and args.gateway
        and args.mode in {Modes.IP_STATIC, Modes.IP_DYNAMIC}
    ):
        device_ip = ip_address(args.ip)
        mask = ip_address(args.mask)
        gateway = ip_address(args.gateway)

        network_ip = ip_address(int(gateway) & int(mask))
        network = ip_network(f"{network_ip}/{mask}")
        if not device_ip in network:
            raise ValueError("Device IP doesn't belong in the same network as gateway!")

    main(**vars(args))
