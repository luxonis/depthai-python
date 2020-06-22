# depthai-api

Host-side DepthAI source code

## Content description

- **host** - contains the code which runs on the host computer (Raspberry Pi or a PC that has a DepthAI device connected). This part has:

  - **py_module** - python module (made only with C++ code with `pybind11`). Builds an `.so` libray that can be imported with python.  
  - **app** - C++ app for easier debugging and development.  
  - **core** - main functionality of the host, which is used in `py_module` and `app`.  

- **shared** - code that is shared between host and DepthAI device. This code gets also into the firmware binary `depthai.cmd`  

## Tested platforms

- Ubuntu 16.04, 18.04;
- Raspbian 10;
- macOS 10.14.6, 10.15.4;

## Setup

- Install development environment dependencies:
  - Linux:

        sudo apt-get install -y git python-pip cmake cmake-gui libusb-1.0-0-dev libcurl4-openssl-dev
  - macOS:

        brew install coreutils python3 cmake libusb wget opencv curl

- After cloning the repo, update the third-party libraries used:

      ./install_dependencies.sh


## Build and run

- **host/py_module**

      cd host/py_module
      mkdir -p build
      cd build
      cmake ..
      make -j
  Alternatively, when this repo is used as a submodule in https://github.com/luxonis/depthai, the build process can be automated with the script below, that is also copying the generated `.so` back to `depthai`:
  - `./build_py_module.sh`  
    first it does a full build, then at subsequent runs compiles only the modified source files.
  - `./build_py_module.sh --clean`  
    cleans up the build folder and does a full rebuild.


- **host/app**  
  (Note: this is currently outdated and doesn't run properly)  
  `DEPTHAI_EXTRAS_PATH` in `main.cpp` may have to be adjusted first.

      cd host/app
      mkdir -p build
      cd build
      cmake ..
      make -j
      ./depthai_app
      
      
## Troubleshooting

1. Build failure on Ubuntu 18.04 ("relocation ..." link error) with gcc 7.4.0 (default) - [**issue #3**](https://github.com/luxonis/depthai-api/issues/3)
   - the solution was to upgrade gcc to version 8:

         sudo apt install g++-8
         sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 70
         sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 70

