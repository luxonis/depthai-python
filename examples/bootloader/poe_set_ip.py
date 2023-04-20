import depthai as dai

(found, info) = dai.DeviceBootloader.getFirstAvailableDevice()

def check_str(s: str):
    spl = s.split(".")
    if len(spl) != 4:
        raise ValueError(f"Entered value {s} doesn't contain 3 dots. Value has to be in the following format: '255.255.255.255'")
    for num in spl:
        if 255 < int(num):
            raise ValueError("Entered values can't be above 255!")
    return s

if found:
    print(f'Found device with name: {info.name}')
    print('-------------------------------------')
    print('"1" to set a static IPv4 address')
    print('"2" to set a dynamic IPv4 address')
    print('"3" to clear the config')
    key = input('Enter the number: ').strip()
    print('-------------------------------------')
    if int(key) < 1 or 3 < int(key):
        raise ValueError("Entered value should either be '1', '2' or '3'!")

    with dai.DeviceBootloader(info) as bl:
        if key in ['1', '2']:
            ipv4 = check_str(input("Enter IPv4: ").strip())
            mask = check_str(input("Enter IPv4 Mask: ").strip())
            gateway = check_str(input("Enter IPv4 Gateway: ").strip())
            mode = 'static' if key == '1' else 'dynamic'
            val = input(f"Flashing {mode} IPv4 {ipv4}, mask {mask}, gateway {gateway} to the POE device. Enter 'y' to confirm. ").strip()
            if val != 'y':
                raise Exception("Flashing aborted.")

            conf = dai.DeviceBootloader.Config()
            if key == '1': conf.setStaticIPv4(ipv4, mask, gateway)
            elif key == '2': conf.setDynamicIPv4(ipv4, mask, gateway)
            (success, error) = bl.flashConfig(conf)
        elif key == '3':
            (success, error) = bl.flashConfigClear()

        if not success:
            print(f"Flashing failed: {error}")
        else:
            print(f"Flashing successful.")