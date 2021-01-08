# Depthai-Python examples

In this directory there are usage examples of depthai-python repository. 

## Setup

- Build python module 

```
cd depthai-python
./build_module.sh
```

- Install python dependencies

```
cd depthai-python
python3 -m pip install -r requirements.txt
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
cd depthai-python/examples
python3 01_rgb_preview.py
```

## Demo

### 01_rgb_preview.py
![example_1](https://user-images.githubusercontent.com/5244214/104040973-9afd2800-51d8-11eb-8fe6-52649069ded5.gif)

### 02_mono_preview.py
![example_2](https://user-images.githubusercontent.com/5244214/104040960-959fdd80-51d8-11eb-8bde-fd706b5c8670.gif)

### 03_depth_preview.py
![example_3](https://user-images.githubusercontent.com/5244214/104040936-90db2980-51d8-11eb-8e71-3dbd0a3b1ab9.gif)

### 04_rgb_encoding.py
![example_4](https://user-images.githubusercontent.com/5244214/104040930-8f116600-51d8-11eb-888a-88272c34aed4.gif)

### 05_rgb_mono_encoding.py
![example_5](https://user-images.githubusercontent.com/5244214/104040921-8d47a280-51d8-11eb-9bc1-d809ec6cd9f6.gif)

### 06_rgb_full_resolution_saver.py
![example_6](https://user-images.githubusercontent.com/5244214/104040908-8882ee80-51d8-11eb-8817-1c8dca320f2b.gif)

### 07_mono_full_resolution_saver.py
![example_7](https://user-images.githubusercontent.com/5244214/104040905-86209480-51d8-11eb-9e15-5fe94aba7b69.gif)

### 08_rgb_mobilenet.py
![example_8](https://user-images.githubusercontent.com/5244214/104040901-84ef6780-51d8-11eb-96e7-f54fa7853f1f.gif)

### 09_mono_mobilenet.py
![example_9](https://user-images.githubusercontent.com/5244214/104040898-8456d100-51d8-11eb-9498-e316b71d41e6.gif)

### 10_mono_depth_mobilenetssd.py
![example_10](https://user-images.githubusercontent.com/5244214/104040871-7d2fc300-51d8-11eb-951b-0325def8b111.gif)
