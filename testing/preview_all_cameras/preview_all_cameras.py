#Example for visualization of all cameras from one device
import cv2
import depthai
from depthai import node, message

class all_cams(depthai.Node):
    def __run__(self, right_stereo: message.ImgFrame,left_stereo: message.ImgFrame,color: message.ImgFrame):
        #convert to opencv frames
        right_stereo_cv = right_stereo.getCvFrame()
        left_stereo_cv = left_stereo.getCvFrame()
        color_cv = color.getCvFrame()

        #Show image frames
        cv2.imshow("right_stereo",right_stereo_cv)
        cv2.imshow("left_stereo", left_stereo_cv)
        cv2.imshow("color", color_cv)

        #quit example using q
        if cv2.waitKey(1) == ord('q'): raise KeyboardInterrupt()

pipeline = depthai.Pipeline()
with pipeline:
    right_stereo = node.MonoCamera().out
    left_stereo = node.MonoCamera().out
    color = node.ColorCamera().video
    all_cams(right_stereo,left_stereo,color)
depthai.run(pipeline)
