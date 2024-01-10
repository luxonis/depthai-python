# Example for visualization of all cameras from one device
import cv2
import depthai
from depthai import node, message

class AllCams(depthai.Node):
    def __run__(self, right_stereo: message.ImgFrame,left_stereo: message.ImgFrame,color: message.ImgFrame):
        # Define names
        cam_names = {
            "right_stereo": right_stereo,
            "left_stereo": left_stereo,
            "color": color
        }

        # Retrieve frames and display them in a loop
        for name, cam_obj in cam_names.items():
            cv_frame = cam_obj.getCvFrame()
            cv2.imshow(name, cv_frame)

        # Quit example using q
        if cv2.waitKey(1) == ord('q'): raise KeyboardInterrupt()

pipeline = depthai.Pipeline()
with pipeline:
    right_stereo = node.MonoCamera().out
    left_stereo = node.MonoCamera().out
    color = node.ColorCamera().video
    AllCams(right_stereo,left_stereo,color)
depthai.run(pipeline)