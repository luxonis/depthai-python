import os
import io
import re
import sys
import platform
import subprocess
import find_version
import multiprocessing

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion
from pathlib import Path

### NAME
MODULE_NAME = 'depthai'

### VERSION
here = os.path.abspath(os.path.dirname(__file__))
version_file = os.path.join(here, "generated", "version.py")
os.makedirs(os.path.join(here, "generated"), exist_ok=True)
if os.environ.get('CI') != None :
    ### If CI build, respect 'BUILD_COMMIT_HASH' to determine final version if set
    final_version = find_version.get_package_version()
    if os.environ.get('BUILD_COMMIT_HASH') != None:
        final_version = find_version.get_package_dev_version(os.environ['BUILD_COMMIT_HASH'])
    with open(version_file, 'w') as vf :
        vf.write("__version__ = '" + final_version + "'")
elif os.path.exists(".git"):
    ### else if .git folder exists, create depthai with commit hash retrieved from git rev-parse HEAD
    commit_hash = 'dev'
    try:
        commit_hash = (
            subprocess.check_output(
                ["git", "rev-parse", "HEAD"], stderr=subprocess.STDOUT
            )
            .splitlines()[0]
            .decode()
        )
    except subprocess.CalledProcessError as e:
        # cannot get commit hash, leave empty
        commit_hash = 'dev'
    final_version = find_version.get_package_dev_version(commit_hash)

    with open(version_file, 'w') as vf :
        vf.write("__version__ = '" + final_version + "'")


# If not generated, generate from find_version
if os.path.isfile(version_file) == False :
    # generate from find_version
    final_version = find_version.get_package_dev_version('dev')
    with open(version_file, 'w') as vf :
        vf.write("__version__ = '" + final_version + "'")

### Get version from version.py (sdist will have this pregenerated)
exec(open(version_file).read())
buildCommitHash = None
if len(__version__.split("+")) > 1 :
    buildCommitHash = __version__.split("+")[1]


## Read description (README.md)
long_description = io.open("README.md", encoding="utf-8").read()

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):

    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.2.0':
                raise RuntimeError("CMake >= 3.2.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):

        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        # initialize cmake_args and build_args
        cmake_args = []
        build_args = []
        env = os.environ.copy()

        # Specify output directory and python executable
        cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir, '-DPYTHON_EXECUTABLE=' + sys.executable]
        # Specify dir of python executable (pybind11)
        if platform.system() == "Windows":
            # Windows - remove case insensitive variants
            env = {key:env[key] for key in env if key.upper() != 'pythonLocation'.upper()}
        env['pythonLocation'] = str(Path(sys.executable).parent.absolute())
        

        # Pass a commit hash
        if buildCommitHash != None :
            cmake_args += ['-DDEPTHAI_PYTHON_COMMIT_HASH=' + buildCommitHash]

        # Pass a docstring option
        if 'DEPTHAI_PYTHON_DOCSTRINGS_INPUT' in os.environ:
            cmake_args += ['-DDEPTHAI_PYTHON_DOCSTRINGS_INPUT='+os.environ['DEPTHAI_PYTHON_DOCSTRINGS_INPUT']]

        # Pass installation directory
        if 'DEPTHAI_INSTALLATION_DIR' in os.environ:
            cmake_args += ['-DDEPTHAI_PYTHON_USE_FIND_PACKAGE=ON']
            cmake_args += ['-DCMAKE_PREFIX_PATH='+os.environ['DEPTHAI_INSTALLATION_DIR']]

        # Set build type (debug vs release for library as well as dependencies)
        cfg = 'Debug' if self.debug else 'Release'
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        cmake_args += ['-DHUNTER_CONFIGURATION_TYPES=' + cfg]
        build_args += ['--config', cfg]

        # Memcheck (guard if it fails)
        freeMemory = 4000
        if platform.system() == "Linux":
            try:
                freeMemory = int(os.popen("free -m").readlines()[1].split()[6])
            except (KeyboardInterrupt, SystemExit):
                raise
            except:
                freeMemory = 4000

        # Configure and build
        # Windows
        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            cmake_args += ['-DCMAKE_TOOLCHAIN_FILE={}'.format(os.path.dirname(os.path.abspath(__file__)) + '/cmake/toolchain/msvc.cmake')]

            # Detect whether 32 / 64 bit Python is used and compile accordingly
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            else:
                cmake_args += ['-A', 'Win32']

            # Add flag to build with maximum available threads
            build_args += ['--', '/m']
        # Unix
        else:
            # if macos add some additional env vars
            if sys.platform == 'darwin':
                from distutils import util
                env['MACOSX_DEPLOYMENT_TARGET'] = '10.9'
                env['_PYTHON_HOST_PLATFORM'] = re.sub(r'macosx-[0-9]+\.[0-9]+-(.+)', r'macosx-10.9-\1', util.get_platform())

            # Specify how many threads to use when building, depending on available memory
            max_threads = multiprocessing.cpu_count()
            num_threads = (freeMemory // 2000)
            num_threads = min(num_threads, max_threads)
            if num_threads <= 0:
                num_threads = 1
            build_args += ['--', '-j' + str(num_threads)]
            cmake_args += ['-DHUNTER_JOBS_NUMBER=' + str(num_threads)]

        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''), self.distribution.get_version())

        # Add additional cmake args from environment
        if 'CMAKE_ARGS' in os.environ:
            cmake_args += [os.environ['CMAKE_ARGS']]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        # Configure and build
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp, env=env)

setup(
    name=MODULE_NAME,
    version=__version__,
    author='Luxonis',
    author_email='support@luxonis.com',
    description='DepthAI Python Library',
    license="MIT",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/luxonis/depthai-python",
    ext_modules=[CMakeExtension(MODULE_NAME)],
    cmdclass={
        'build_ext': CMakeBuild
    },
    zip_safe=False,
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Information Technology",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Operating System :: MacOS",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: Unix",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: C++",
        "Programming Language :: Python :: Implementation :: CPython",
        "Topic :: Scientific/Engineering",
        "Topic :: Software Development",
    ],
    python_requires='>=3.6',
)
