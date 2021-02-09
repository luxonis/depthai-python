# readthedocs CI build limitation circumvent
import subprocess
import os
import sys

# Get current working directory
cwd = os.path.dirname(os.path.realpath(__file__))

# Specify source directory
packagedir = cwd+'/../source'

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
subprocess.check_call(['mkdir', '-p', 'libclang'])
subprocess.check_call(['tar', 'xf', 'libclang-11_manylinux2014_x86_64.tar.xz', '-C', cwd+'/libclang/'])
env['LIBCLANG_PATH'] = cwd+'/libclang/usr/lib/llvm-11/lib/libclang.so.1'
env['LLVM_DIR_PATH'] = cwd+'/libclang/usr/lib/llvm-11/'

# pybind11_mkdoc patched
subprocess.check_call(['git', 'clone', 'https://github.com/pybind/pybind11_mkdoc'])
mkdoc_lib_patch = '''
--- mkdoc_lib.py.orig	2021-02-09 19:19:50.196813322 +0000
+++ mkdoc_lib.py	2021-02-09 19:36:34.887916547 +0000
@@ -277,12 +277,15 @@
         def folder_version(d):
             return [int(ver) for ver in re.findall(r'(?<!lib)(?<!\d)\d+', d)]
 
-        llvm_dir = max((
-            path
-            for libdir in ['lib64', 'lib', 'lib32']
-            for path in glob('/usr/%s/llvm-*' % libdir)
-            if os.path.isdir(path)
-        ), default=None, key=folder_version)
+        if 'LLVM_DIR_PATH' in os.environ:
+            llvm_dir = os.environ['LLVM_DIR_PATH']
+        else: 
+            llvm_dir = max((
+                path
+                for libdir in ['lib64', 'lib', 'lib32']
+                for path in glob('/usr/%s/llvm-*' % libdir)
+                if os.path.isdir(path)
+            ), default=None, key=folder_version)
 
 
         if llvm_dir:
'''
p = subprocess.Popen(['patch', 'pybind11_mkdoc/pybind11_mkdoc/mkdoc_lib.py'], stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
stdout_data = p.communicate(input=bytes(mkdoc_lib_patch, encoding='utf-8'))[0]
subprocess.check_call([sys.executable, '-m', 'pip', 'install', 'pybind11_mkdoc/'])


# Build library and generate actual conf.py
subprocess.run(["cmake -P ci.cmake"], cwd=cwd, shell=True, check=True, env=env)

# Load generated conf.py which includes all needed information to build with sphinx
with open("../../build/docs/conf.py") as infile:
    exec(infile.read())
