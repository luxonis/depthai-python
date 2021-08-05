import depthai as dai

(found, deviceInfo) = dai.DeviceBootloader.getFirstAvailableDevice()
if not found:
    raise RuntimeError("Device not found!")

bootloader = dai.DeviceBootloader(deviceInfo)
progress = lambda p : print(f'Flashing progress: {p*100:.1f}%')
bootloader.flashBootloader(progress)

print("Bootloader flashed successfully")
