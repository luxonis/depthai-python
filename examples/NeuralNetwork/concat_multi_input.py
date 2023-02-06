#!/usr/bin/env python3

from pathlib import Path
import sys
import numpy as np
import cv2
import depthai as dai
SHAPE = 300

# Get argument first
nnPath = str((Path(__file__).parent / Path('../models/concat_openvino_2021.4_6shave.blob')).resolve().absolute())
if len(sys.argv) > 1:
    nnPath = sys.argv[1]

if not Path(nnPath).exists():
    import sys
    raise FileNotFoundError(f'Required file/s not found, please run "{sys.executable} install_requirements.py"')

p = dai.Pipeline()
p.setOpenVINOVersion(dai.OpenVINO.VERSION_2021_4)

camRgb = p.createColorCamera()
camRgb.setPreviewSize(SHAPE, SHAPE)
camRgb.setInterleaved(False)
camRgb.setColorOrder(dai.ColorCameraProperties.ColorOrder.BGR)

def create_mono(p, socket):
    mono = p.create(dai.node.MonoCamera)
    mono.setBoardSocket(socket)
    mono.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)

    # ImageManip for cropping (face detection NN requires input image of 300x300) and to change frame type
    manip = p.create(dai.node.ImageManip)
    manip.initialConfig.setResize(300, 300)
    manip.initialConfig.setFrameType(dai.RawImgFrame.Type.BGR888p)
    mono.out.link(manip.inputImage)
    return manip.out

# NN that detects faces in the image
nn = p.createNeuralNetwork()
nn.setBlobPath(nnPath)
nn.setNumInferenceThreads(2)

camRgb.preview.link(nn.inputs['img2'])
create_mono(p, dai.CameraBoardSocket.CAM_B).link(nn.inputs['img1'])
create_mono(p, dai.CameraBoardSocket.CAM_C).link(nn.inputs['img3'])

# Send bouding box from the NN to the host via XLink
nn_xout = p.createXLinkOut()
nn_xout.setStreamName("nn")
nn.out.link(nn_xout.input)

# Pipeline is defined, now we can connect to the device
with dai.Device(p) as device:
    qNn = device.getOutputQueue(name="nn", maxSize=4, blocking=False)
    shape = (3, SHAPE, SHAPE * 3)

    while True:
        inNn = np.array(qNn.get().getFirstLayerFp16())
        # Planar INT8 frame
        frame = inNn.reshape(shape).astype(np.uint8).transpose(1, 2, 0)

        cv2.imshow("Concat", frame)

        if cv2.waitKey(1) == ord('q'):
            break
