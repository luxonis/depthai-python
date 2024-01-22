import depthai as dai
import cv2
import numpy as np
import typing

class KeyboardControl(dai.Node):
    def __node_init__(self):
        self.state = False
    def __run__(self) -> dai.message.Buffer:
        key = cv2.waitKey(1)
        if key == -1: return
        match chr(key):
            case "q": raise KeyboardInterrupt()
            case "t": self.state = not self.state

        buf = dai.message.Buffer()
        buf.setData(self.state)
        return buf

T = typing.TypeVar("T")
class Switch(dai.Script):
    input_desc = {"on": T,
                  "off": T,
                  "control": dai.message.Buffer}
    output_desc = {"output": T}
    def __script__(node):
        state = False
        while True:
            msg = node.io["control"].tryGet()
            if msg is not None:
                state = msg.getData()[0]
                node.warn("Toggle! State: " + ("on" if state else "off"))

            on = node.io["on"].get()
            off = node.io["off"].get()
            node.io["output"].send(on if state else off)

class Viewer(dai.Node):
    def __run__(self, image: dai.message.ImgFrame):
        frame = image.getCvFrame().astype(np.uint8)
        frame = cv2.applyColorMap(frame, cv2.COLORMAP_JET)
        cv2.imshow("Stereo", frame)

def TWIN_MONO(**kwargs):
    return (dai.node.MonoCamera(**kwargs) for _ in range(2))

pipeline = dai.Pipeline()
with pipeline:
    stereo = dai.node.StereoDepth(
            *TWIN_MONO(resolution = dai.MonoCameraProperties.SensorResolution.THE_400_P, 
                       fps=30),
            default_profile_preset = dai.PresetMode.HIGH_DENSITY,
            median_filter = dai.MedianFilter.KERNEL_7x7,
            left_right_check = True,
            extended_disparity = False,
            subpixel = False)
    Viewer(Switch(stereo.depth, stereo.disparity, KeyboardControl()))
dai.run(pipeline)
