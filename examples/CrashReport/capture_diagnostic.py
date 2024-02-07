import depthai as dai
import zipfile
from json import dump, JSONEncoder
import io

class CustomEncoder(JSONEncoder):
    def default(self, obj):
        if hasattr(obj, '__str__'):
            return str(obj)
        return super().default(obj)

data = {}

with dai.Device() as device:
    data['BootloaderVersion'] = device.getBootloaderVersion()
    data['ConnectedIMU'] = device.getConnectedIMU()
    data['imuFirmwareVersion'] = device.getIMUFirmwareVersion()

    filenames = [
        "metadata.json",
        "userCalib.json",
        "factoryCalib.json"   
    ]

    # Create in-memory file-like objects
    metadata_io = io.StringIO()
    usercalib_io = io.StringIO()
    factcalib_io = io.StringIO()
    crashDump_io = io.StringIO()
    # Dump data into these in-memory files
    dump(data, metadata_io, indent=2, cls=CustomEncoder)
    
    try:
        dump(device.readCalibration2().eepromToJson(), usercalib_io, indent=2)
        dump(device.readCalibrationRaw(), usercalib_io, indent=4)
    except Exception as ex:
        dump(str(ex), usercalib_io, indent=2)

    try:
        dump(device.readFactoryCalibration().eepromToJson(), factcalib_io, indent=2)
        dump(device.readFactoryCalibrationRaw(), factcalib_io, indent=4)
    except Exception as ex:
        dump(str(ex), factcalib_io, indent=2)

    in_memory_files = [metadata_io, usercalib_io, factcalib_io]

    zip_name = 'device_info.zip'
    
    with zipfile.ZipFile(zip_name, 'w') as z:
        # Zip in-memory file-like objects without writing to disk
        for file_io, filename in zip(in_memory_files, filenames):
            file_io.seek(0)  # Reset cursor to the beginning of the file
            z.writestr(filename, file_io.getvalue())

    if device.hasCrashDump():
        crashDump = device.getCrashDump()
        commitHash = crashDump.depthaiCommitHash
        deviceId = crashDump.deviceId
        filenames.append("crashreport_" + commitHash + "_" + deviceId + ".json")
        dump(crashDump.serializeToJson(), crashDump_io, indent=2)
        in_memory_files.append(crashDump_io)

        with zipfile.ZipFile(zip_name, 'w') as z:
        # Zip in-memory file-like objects without writing to disk
            for file_io, filename in zip(in_memory_files, filenames):
                file_io.seek(0)  # Reset cursor to the beginning of the file
                z.writestr(filename, file_io.getvalue())

        print("Crash dump found on your device!")
    else:
        print("There was no crash dump found on your device!")

print("Please report to developers with the zip attached!")
