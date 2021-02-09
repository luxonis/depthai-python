# readthedocs CI build limitation circumvent
import subprocess
import os

# Get current working directory
cwd = os.path.dirname(os.path.realpath(__file__))

# Prepare dependencies for library
subprocess.check_call(['wget', 'https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.tar.bz2'])
subprocess.check_call(['tar', 'xf', 'libusb-1.0.24.tar.bz2'])
subprocess.check_call(['./configure', '--disable-udev', '--prefix', cwd+'/../libusb'], cwd='libusb-1.0.24')
subprocess.check_call(['make', '-j'], cwd='libusb-1.0.24')
subprocess.check_call(['make', 'intall'], cwd='libusb-1.0.24')

# Add libusb to path
env=os.environ.copy()
env['PATH'] = cwd+'/libusb/include:'+cwd+'/libusb/lib'+':'+env['PATH']
print('Path set to: ', env['PATH'])

# Build library and generate actual conf.py
subprocess.run(["cmake -P ci.cmake"], cwd=cwd, shell=True, check=True, env=env)

# Load generated conf.py which includes all needed information to build with sphinx
with open("../../build/docs/conf.py") as infile:
    exec(infile.read())
