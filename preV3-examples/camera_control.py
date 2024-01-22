import cv2
import depthai as dai

class KeyboardControl(dai.Node):
    def __node_init__(self):
        self.exp_time = 20_000 # Exposure time in microseconds
        self.sens_iso = 800    # Sensitivity in ISO

    def __run__(self) -> dai.message.CameraControl:
        key = cv2.waitKey(1)
        if key == -1: return
        
        match chr(key):
            case "q": raise KeyboardInterrupt()
            case "i": self.exp_time -= 500
            case "o": self.exp_time += 500
            case "k": self.sens_iso -= 50
            case "l": self.sens_iso += 50

        self.exp_time = max(1, min(33_000, self.exp_time))
        self.sens_iso = max(100, min(1600, self.sens_iso))

        return dai.message.CameraControl().setManualExposure(
                exposureTimeUs=self.exp_time,
                sensitivityIso=self.sens_iso)

class Viewer(dai.Node):
    def __run__(self, image: dai.message.ImgFrame):
        cv2.imshow("Camera output", image.getCvFrame())

pipeline = dai.Pipeline()
with pipeline:
    camera_control = KeyboardControl()
    video = dai.node.ColorCamera(input_control=camera_control).video
    Viewer(video)
dai.run(pipeline)
