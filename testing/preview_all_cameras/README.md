Script change pipeline flow
===================

This example shows how you can change the flow of data inside your pipeline in runtime using the Script node. In this example, we send a message from the host to choose whether we want Script node to forwards color frame to the MobileNetDetectionNetwork

## Demo
![demo](https://user-images.githubusercontent.com/18037362/187734814-df3b46c9-5e04-4a9d-bf6f-d738b40b4421.gif)

## Setup
First download and install depthai when not done previously
```
git clone --recurse-submodules -b preV3 https://github.com/luxonis/depthai-python.git
cd depthai/
pip install .
```

## Pipeline

![pipeline](https://user-images.githubusercontent.com/18037362/187736249-db7ff175-fcea-4d4e-b567-f99087bd82ee.png)