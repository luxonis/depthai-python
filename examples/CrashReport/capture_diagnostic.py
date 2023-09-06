import depthai as dai
import zipfile
from json import dump , JSONEncoder
from os.path import exists
import os

class CustomEncoder(JSONEncoder):
    def default(self, obj):
        # If the object has a `__str__` method, use it
        if hasattr(obj, '__str__'):
            return str(obj)
        # Otherwise, use the default encoder's method
        return super().default(obj)

data = {}

with dai.Device() as device:

    data['BootloaderVersion'] = device.getBootloaderVersion()
    data['ConnectedIMU'] = device.getConnectedIMU()
    data['imuFirmwareVersion'] = device.getIMUFirmwareVersion()
    filename = 'metadata.json'
    with open(filename, 'w') as f:
        dump(data, f, indent=2, cls=CustomEncoder)


    usercalibPath = 'userCalib.json'
    try:
        
        with open(usercalibPath,'w') as f:
            dump(device.readCalibration2().eepromToJson(), f, indent=2)
            dump(device.readCalibrationRaw(), f, indent=4)
    except Exception as ex:
        
        with open(usercalibPath,'w') as f:
            dump(str(ex), f, indent=2)

    factcalibPath = 'factoryCalib.json'
    try:
        
        with open(factcalibPath,'w') as f:
            dump(device.readFactoryCalibration().eepromToJson(), f, indent=2)
            dump(device.readFactoryCalibrationRaw(), f, indent=4)
    except Exception as ex:
        
        with open(factcalibPath,'w') as f:
            dump(str(ex), f, indent=2)

    zip_name = 'device_info.zip'
    if device.hasCrashDump():
        crashDump = device.getCrashDump()
        commitHash = crashDump.depthaiCommitHash
        deviceId = crashDump.deviceId

        json = crashDump.serializeToJson()
        
        i = -1
        while True:
            i += 1
            destPath = "crashDump_" + str(i) + "_" + deviceId + "_" + commitHash + ".json"
            if exists(destPath):
                continue

            with open(destPath, 'w', encoding='utf-8') as f:
                dump(json, f, ensure_ascii=False, indent=4)

            print("Crash dump found on your device!")
            print(f"Saved to {destPath}")
            print("Please report to developers!")
            with zipfile.ZipFile(zip_name, 'w') as z:
                z.write(filename)
                z.write(usercalibPath)
                z.write(factcalibPath)
                z.write(destPath)
            os.remove(filename)
            os.remove(usercalibPath)
            os.remove(factcalibPath)
            os.remove(destPath)
            break
    else:
        with zipfile.ZipFile(zip_name, 'w') as z:
            z.write(filename)
            z.write(usercalibPath)
            z.write(factcalibPath)
        os.remove(filename)
        os.remove(usercalibPath)
        os.remove(factcalibPath)
        print("There was no crash dump found on your device!")
# Zip the JSON file



print("Please report to developers via forum with the zip attached!")
