# TODO: increase defaults in depthai! Below is a workaround for this issue on some setups:
#     Failed to find device after booting, error message: X_LINK_DEVICE_NOT_FOUND
# If this error is still observed, might be a FW crash on initialization
if 1:
    import os
    os.environ["DEPTHAI_WATCHDOG_INITIAL_DELAY"] = "40000"
    os.environ["DEPTHAI_BOOTUP_TIMEOUT"] = "40000"

import depthai as dai
import threading
import traceback
import time
import os

def worker(devinfo, id):
    print(f'[{id}] opening bootloader...')
    try:
        with dai.DeviceBootloader(devinfo) as bl:
            print(f'[{id}] bootloader version: {bl.getVersion()}')
        with dai.Device(devinfo) as dev:
            print(f'[{id}] device started, name: {dev.getDeviceName()}')
            time.sleep(6)
    # Exceptions aren't propagated, so simply exit the app with an error
    except Exception as e:
        print(f'[{id}] FAILED: {e}')
        traceback.print_exc()
        os._exit(-1)

device_infos = dai.Device.getAllAvailableDevices()
n = len(device_infos)
if n <= 0:
    print('No devices found!')
    os._exit(-1)
print(f'Found {n} devices, handling each in a thread...')
threads = []
for devinfo in device_infos:
    id = devinfo.name  # IP address for PoE
    t = threading.Thread(target=worker, args=(devinfo, id, ))
    t.name += f'-{id}'  # Thread name useful for debugging
    t.start()
    threads.append(t)

for t in threads:
    t.join()

print('Multi-threading test succeeded!')
