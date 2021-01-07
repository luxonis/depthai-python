# Depthai-Python examples

In this directory there are usage examples of depthai-python repository. 

## Setup

- Build python module 

```
cd depthai-python
./build_module.sh
```

- Export `PYTHONPATH` variable to the build directory

```
export PYTHONPATH=/path/to/depthai-python/build
```

- Download `mobilenet-ssd` blob

```
cd depthai-python/examples
mkdir models
cd models
wget https://artifacts.luxonis.com/artifactory/luxonis-depthai-data-local/network/mobilenet.blob
```

## Usage

To run e.g. `01_rgb_preview.py` simply run

```
python3 01_rgb_preview.py
```