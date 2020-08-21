# DepthAI Python Library

Python bindings for C++ depthai-core library

## Documentation

Documentation is available over at [Luxonis DepthAI Python API](https://docs.luxonis.com/api/)

## Installation

Prebuilt wheels are available in [Luxonis repository](https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local/)
Make sure pip is upgraded
```
python3 -m pip install -U pip
python3 -m pip install --extra-index-url https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local/ depthai
```
## Building from source

### Dependencies
 - cmake >= 3.2
 - C/C++11 compiler (clang, gcc, msvc, ...)
 - Python

Along these, dependencies of depthai-core are also required
See: [depthai-core dependencies](https://github.com/luxonis/depthai-core#dependencies)


### Building

To build a shared library from source perform the following:
```
mkdir build && cd build
cmake ..
cmake --build . --parallel
```

To build a wheel, execute the following
```
python3 -m pip wheel . -w wheelhouse
```


## Tested platforms

- Windows 10
- Ubuntu 16.04, 18.04;
- Raspbian 10;
- macOS 10.14.6, 10.15.4;

## Troubleshooting

1. Build failure on Ubuntu 18.04 ("relocation ..." link error) with gcc 7.4.0 (default) - [**issue #3**](https://github.com/luxonis/depthai-api/issues/3)
   - the solution was to upgrade gcc to version 8:

         sudo apt install g++-8
         sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 70
         sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 70

