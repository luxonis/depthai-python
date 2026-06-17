import sys
from typing import Callable, Tuple
from enum import Enum
import depthai

FLASH = depthai.DeviceBootloader.Memory.FLASH

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} [GPIO_MODE/USB_RECOVERY/NORMAL/FAST] [params]")
        print("\tOptions:")
        print(f"\t\t{sys.argv[0]} GPIO_MODE gpioModeNum")
        print(f"\t\t{sys.argv[0]} USB_RECOVERY")
        print(f"\t\t{sys.argv[0]} NORMAL [frequency] [location] [dummyCycles] [offset]")
        print(f"\t\t{sys.argv[0]} FAST [frequency] [location] [dummyCycles] [offset]")
        return 0

    mode = sys.argv[1].lower()
    flash_func = None

    if mode == "gpio_mode":
        # gpio mode header
        if len(sys.argv) < 3:
            print(f"Usage: {sys.argv[0]} GPIO_MODE gpioModeNum")
            return 0
        
        gpio_mode = int(sys.argv[2])
        def flash_gpio(bl: 'depthai.DeviceBootloader') -> Tuple[bool, str]:
            return bl.flashGpioModeBootHeader(FLASH, gpio_mode)
        flash_func = flash_gpio

    elif mode == "usb_recovery":
        # usb recovery header
        def flash_usb(bl: 'depthai.DeviceBootloader') -> Tuple[bool, str]:
            return bl.flashUsbRecoveryBootHeader(FLASH)
        flash_func = flash_usb

    elif mode in ("normal", "fast"):
        if len(sys.argv) != 2 and len(sys.argv) != 3 and len(sys.argv) <= 3:
            print(f"Usage: {sys.argv[0]} NORMAL/FAST [frequency] [location] [dummyCycles] [offset]")
            print(f"Usage: {sys.argv[0]} NORMAL/FAST [frequency]")
            return 0

        offset = -1
        location = -1
        dummy_cycles = -1
        frequency = -1

        if len(sys.argv) > 3:
            if len(sys.argv) >= 3:
                offset = int(sys.argv[2])
            if len(sys.argv) >= 4:
                location = int(sys.argv[3])
            if len(sys.argv) >= 5:
                dummy_cycles = int(sys.argv[4])
            if len(sys.argv) >= 6:
                frequency = int(sys.argv[5])
        elif len(sys.argv) == 3:
            frequency = int(sys.argv[2])

        if mode == "normal":
            def flash_normal(bl: 'depthai.DeviceBootloader') -> Tuple[bool, str]:
                return bl.flashBootHeader(FLASH, frequency, location, dummy_cycles, offset)
            flash_func = flash_normal
        elif mode == "fast":
            def flash_fast(bl: 'depthai.DeviceBootloader') -> Tuple[bool, str]:
                return bl.flashFastBootHeader(FLASH, frequency, location, dummy_cycles, offset)
            flash_func = flash_fast

    # Find device and flash specified boot header
    success, info = depthai.DeviceBootloader.getFirstAvailableDevice()
    
    if success:
        print(f"Found device with name: {info.name}")
        bl = depthai.DeviceBootloader(info)

        # Flash the specified boot header
        if flash_func:
            success, error_msg = flash_func(bl)
            if success:
                print("Successfully flashed boot header!")
            else:
                print(f"Couldn't flash boot header: {error_msg}")
        else:
            print("Invalid boot option header specified")
    else:
        print("No devices found")

    return 0

if __name__ == "__main__":
    main()