import cv2
import depthai as dai

# Create pipeline
pipeline = dai.Pipeline()

# Rotate color frames
camRgb = pipeline.createColorCamera()
camRgb.setPreviewSize(640, 400)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
camRgb.setInterleaved(False)

manipRgb = pipeline.createImageManip()
manipRgb.setMaxOutputFrameSize(2000 * 1500 * 3)
rgbRr = dai.RotatedRect()
rgbRr.center.x, rgbRr.center.y = camRgb.getPreviewWidth() // 2, camRgb.getPreviewHeight() // 2
rgbRr.size.width, rgbRr.size.height = camRgb.getPreviewHeight(), camRgb.getPreviewWidth()
rgbRr.angle = 90
manipRgb.initialConfig.setCropRotatedRect(rgbRr, False)
camRgb.preview.link(manipRgb.inputImage)

manipRgbOut = pipeline.createXLinkOut()
manipRgbOut.setStreamName("manip_rgb")
manipRgb.out.link(manipRgbOut.input)

# Rotate mono frames
monoLeft = pipeline.createMonoCamera()
monoLeft.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
monoLeft.setBoardSocket(dai.CameraBoardSocket.LEFT)

manipLeft = pipeline.createImageManip()
rr = dai.RotatedRect()
rr.center.x, rr.center.y = monoLeft.getResolutionWidth() // 2, monoLeft.getResolutionHeight() // 2
rr.size.width, rr.size.height = monoLeft.getResolutionHeight(), monoLeft.getResolutionWidth()
rr.angle = 90
manipLeft.initialConfig.setCropRotatedRect(rr, False)
monoLeft.out.link(manipLeft.inputImage)

manipLeftOut = pipeline.createXLinkOut()
manipLeftOut.setStreamName("manip_left")
manipLeft.out.link(manipLeftOut.input)

with dai.Device(pipeline) as device:
    qLeft = device.getOutputQueue(name="manip_left", maxSize=8, blocking=False)
    qRgb = device.getOutputQueue(name="manip_rgb", maxSize=8, blocking=False)

    while True:
        inLeft = qLeft.tryGet()
        if inLeft is not None:
            cv2.imshow('Left rotated', inLeft.getCvFrame())

        inRgb = qRgb.tryGet()
        if inRgb is not None:
            cv2.imshow('Color rotated', inRgb.getCvFrame())

        if cv2.waitKey(1) == ord('q'):
            break