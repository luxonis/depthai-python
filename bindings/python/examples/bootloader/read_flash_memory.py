import depthai as dai
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-offset', type=str, nargs='?', help="Offset for flash memory reading", default='0')
parser.add_argument('-size', type=str, nargs='?', help="Size for reading", default='1mb')
parser.add_argument('-filename', type=str, nargs='?', help="Path to the file dump. If empty, flash will be printed to the console", default='')
args = parser.parse_args()

def convert(string: str) -> int:
    string = string.upper()
    if 'KB' in string: # KiB
        string = string.replace('KB', '')
        return int(string) * 1024
    elif 'MB' in string: # MiB
        string = string.replace('MB', '')
        return int(string) * 1024 * 1024
    elif string.isnumeric(): # Bytes
        return int(string)
    else:
        print(string)
        raise ValueError("Input can only be number with optional 'KB' or 'MB' suffix")

offset = convert(args.offset)
size = convert(args.size)

print(offset, size, args.filename)

# Find device and read memory
(ok, info) = dai.DeviceBootloader.getFirstAvailableDevice()

if not ok:
    raise Exception('No device found')
print(f'Found device with name: {info.name}')

with dai.DeviceBootloader(info) as bl:
    progress = lambda p : print(f'Reading progress: {p*100:.1f}%')

    if args.filename:
        ok, err = bl.readCustom(dai.DeviceBootloader.Memory.FLASH, offset, size, args.filename, progress)
        if not ok: print(err)
    else:
        ok, err, bytes = bl.readCustom(dai.DeviceBootloader.Memory.FLASH, offset, size, progress)
        if not ok: print(err)

        # Print as hex, 32B aligned
        for i in range(0, len(bytes), 32):
            for j in range(32):
                index = i+j
                if index < len(bytes):
                    print(f'{bytes[index]:02x}', end='')
            print('') # New line
