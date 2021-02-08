# readthedocs CI build limitation circumvent
import subprocess
import os

# Prepare dependencies for library
subprocess.check_call(['wget', 'https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.tar.bz2'])
subprocess.check_call(['tar', 'xf', 'libusb-1.0.24.tar.bz2'])
subprocess.check_call(['./configure', '--disable-udev', '--prefix', pwd+'/../libusb'], cwd='libusb-1.0.24')
# Add libusb to path
os.environ['PATH'] = os.environ['PATH']+':'+pwd+'/libusb/include:'+pwd+'/libusb/lib'
print('Path set to: ', os.environ['PATH'])

# Build library and generate actual conf.py
current_dir = os.path.dirname(os.path.realpath(__file__))
subprocess.run(["cmake -P ci.cmake"], cwd=current_dir, shell=True, check=True)

# Load generated conf.py which includes all needed information to build with sphinx
with open("../../build/docs/conf.py") as infile:
    exec(infile.read())
