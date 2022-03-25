#!/bin/bash

brew install libusb
python3 -m pip install pyusb
python3 -m pip install --extra-index-url https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local depthai==2.1.0.0.dev+c6625fdd03af266630050a95d03162471059c682

