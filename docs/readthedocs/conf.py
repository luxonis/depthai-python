# readthedocs CI build limitation circumvent
import subprocess
import os

# Get current working directory
cwd = os.path.dirname(os.path.realpath(__file__))

# Prepare dependencies for library
env=os.environ.copy()

# Libusb
subprocess.check_call(['wget', 'https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.tar.bz2'])
subprocess.check_call(['tar', 'xf', 'libusb-1.0.24.tar.bz2'])
subprocess.check_call(['./configure', '--disable-udev', '--prefix', cwd+'/libusb'], cwd='libusb-1.0.24')
subprocess.check_call(['make', '-j'], cwd='libusb-1.0.24')
subprocess.check_call(['make', 'install'], cwd='libusb-1.0.24')
env['PATH'] = cwd+'/libusb/include:'+cwd+'/libusb/lib'+':'+env['PATH']

# libclang
subprocess.check_call(['wget', 'https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/misc/libclang-11_manylinux2014_x86_64.tar.xz'])
subprocess.check_call(['tar', 'xf', 'libclang-11_manylinux2014_x86_64.tar.xz', '-C', cwd+'/libclang/'])
env['LIBCLANG_PATH'] = cwd+'/libclang/usr/lib/llvm-11/lib/libclang.so.1'

# Build library and generate actual conf.py
subprocess.run(["cmake -P ci.cmake"], cwd=cwd, shell=True, check=True, env=env)

# Load generated conf.py which includes all needed information to build with sphinx
with open("../../build/docs/conf.py") as infile:
    exec(infile.read())
