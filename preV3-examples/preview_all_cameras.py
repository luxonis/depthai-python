import cv2
import depthai as dai

class Viewer(dai.Node):
    def __run__(self, 
                right: dai.message.ImgFrame,
                left: dai.message.ImgFrame,
                color: dai.message.ImgFrame):
        if cv2.waitKey(1) == ord('q'): raise KeyboardInterrupt()

        cv2.imshow("right", right.getCvFrame())
        cv2.imshow("left", left.getCvFrame())
        cv2.imshow("color", color.getCvFrame())

pipeline = dai.Pipeline()
with pipeline:
    Viewer(
        dai.node.MonoCamera(camera="right"),
        dai.node.MonoCamera(camera="left"),
        dai.node.ColorCamera().video)
dai.run(pipeline)
