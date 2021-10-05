#!/usr/bin/env python3

from pathlib import Path
import sys
import numpy as np
import cv2
import depthai as dai
SHAPE = 300

# Get argument first
nnPath = str((Path(__file__).parent / Path('../models/normalize_openvino_2021.4_4shave.blob')).resolve().absolute())
if len(sys.argv) > 1:
    nnPath = sys.argv[1]

if not Path(nnPath).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

p = dai.Pipeline()
p.setOpenVINOVersion(dai.OpenVINO.VERSION_2021_4)

camRgb = p.createColorCamera()
# Model expects values in FP16, as we have compiled it with `-ip FP16`
camRgb.setFp16(True)
camRgb.setInterleaved(False)
camRgb.setPreviewSize(SHAPE, SHAPE)

nn = p.createNeuralNetwork()
nn.setBlobPath(nnPath)
nn.setNumInferenceThreads(2)

script = p.create(dai.node.Script)
script.setScript("""
# Run script only once

# This configuration will divide frame values from 0.0-255.0 to 0.0 to 2.0
mul = NNData(2)
mul.setLayer("multiplier", [1/127.5])
node.io['mul'].send(mul)

# This configuration will substract frame values from 0.0-2.0 to -1.0-1.0
add = NNData(2)
add.setLayer("addend", [-1.0])
node.io['add'].send(add)
""")

# Re-use the initial values for multiplier/addend
script.outputs['mul'].link(nn.inputs['multiplier'])
nn.inputs['multiplier'].setWaitForMessage(False)

script.outputs['add'].link(nn.inputs['addend'])
nn.inputs['addend'].setWaitForMessage(False)
# Always wait for the new frame before starting inference
camRgb.preview.link(nn.inputs['frame'])

# Send normalized frame values to host
nn_xout = p.createXLinkOut()
nn_xout.setStreamName("nn")
nn.out.link(nn_xout.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(p) as device:
    qNn = device.getOutputQueue(name="nn", maxSize=4, blocking=False)
    shape = (3, SHAPE, SHAPE)
    while True:
        inNn = np.array(qNn.get().getData())
        # Get back the frame. It's currently normalized to -1.0 - 1.0
        frame = inNn.view(np.float16).reshape(shape).transpose(1, 2, 0)
        # To get original frame back (0-255), we add 1 and multiply all frame values by 127.5
        frame = ((frame + 1) * 127.5).astype(np.uint8).copy()
        cv2.imshow("Original frame", frame)

        if cv2.waitKey(1) == ord('q'):
            break
