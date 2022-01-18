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
# Run script only once. We could also send these values from host.
# Model formula:
# output = (input - mean) / scale

# This configuration will subtract all frame values (pixels) by 127.5
# 0.0 .. 255.0 -> -127.5 .. 127.5
data = NNData(2)
data.setLayer("mean", [127.5])
node.io['mean'].send(data)

# This configuration will divide all frame values (pixels) by 255.0
# -127.5 .. 127.5 -> -0.5 .. 0.5
data = NNData(2)
data.setLayer("scale", [255.0])
node.io['scale'].send(data)
""")

# Re-use the initial values for multiplier/addend
script.outputs['mean'].link(nn.inputs['mean'])
nn.inputs['mean'].setWaitForMessage(False)

script.outputs['scale'].link(nn.inputs['scale'])
nn.inputs['scale'].setWaitForMessage(False)
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
        # Get back the frame. It's currently normalized to -0.5 - 0.5
        frame = inNn.view(np.float16).reshape(shape).transpose(1, 2, 0)
        # To get original frame back (0-255), we add multiply all frame values (pixels) by 255 and then add 127.5 to them
        frame = (frame * 255.0 + 127.5).astype(np.uint8)
        # Show the initial frame
        cv2.imshow("Original frame", frame)

        if cv2.waitKey(1) == ord('q'):
            break
