import time
import cv2
import depthai as dai
import numpy as np

resolutionMap = {"THE_1080_P": (1920, 1080), "THE_800_P": (1280, 800), "THE_720_P": (1280, 720), "THE_400_P": (640, 400)}
resolution = resolutionMap["THE_1080_P"]
camSocket = dai.CameraBoardSocket.RGB
meshWidth = 2
meshHeight = 2

def getMesh(calibData):
    global meshWidth
    global meshHeight

    M1 = np.array(calibData.getCameraIntrinsics(camSocket, resolution[0], resolution[1]))
    d1 = np.array(calibData.getDistortionCoefficients(camSocket))
    R1 = np.identity(3)
    mapX, mapY = cv2.initUndistortRectifyMap(M1, d1, R1, M1, resolution, cv2.CV_32FC1)

    meshCellSize = 16
    mesh0 = []
    # Creates subsampled mesh which will be loaded on to device to undistort the image
    for y in range(mapX.shape[0] + 1): # iterating over height of the image
        if y % meshCellSize == 0:
            rowLeft = []
            for x in range(mapX.shape[1]): # iterating over width of the image
                if x % meshCellSize == 0:
                    if y == mapX.shape[0] and x == mapX.shape[1]:
                        rowLeft.append(mapX[y - 1, x - 1])
                        rowLeft.append(mapY[y - 1, x - 1])
                    elif y == mapX.shape[0]:
                        rowLeft.append(mapX[y - 1, x])
                        rowLeft.append(mapY[y - 1, x])
                    elif x == mapX.shape[1]:
                        rowLeft.append(mapX[y, x - 1])
                        rowLeft.append(mapY[y, x - 1])
                    else:
                        rowLeft.append(mapX[y, x])
                        rowLeft.append(mapY[y, x])
            if (mapX.shape[1] % meshCellSize) % 2 != 0:
                rowLeft.append(0)
                rowLeft.append(0)

            mesh0.append(rowLeft)

    mesh0 = np.array(mesh0)
    meshWidth = mesh0.shape[1] // 2
    meshHeight = mesh0.shape[0]
    mesh0.resize(meshWidth * meshHeight, 2)

    mesh = list(map(tuple, mesh0))

    return mesh

def create_pipeline(mesh):
    pipeline = dai.Pipeline()

    cam = pipeline.create(dai.node.ColorCamera)
    cam.setBoardSocket(camSocket)
    cam.setResolution(dai.ColorCameraProperties.SensorResolution.THE_1080_P)
    cam.setIspScale(1, 1)

    manip1 = pipeline.create(dai.node.ImageManip)
    manip1.setWarpMesh(mesh, meshWidth, meshHeight)
    manip1.setMaxOutputFrameSize(cam.getIspWidth() * cam.getIspHeight() * 3 // 2)
    # TODO manip1.initialConfig.setWarpBorderFillColor(0, 0, 0)

    cam.isp.link(manip1.inputImage)
    cam_xout = pipeline.createXLinkOut()
    cam_xout.setStreamName("cam_out")

    if 1: # direct
        manip1.out.link(cam_xout.input)
    else: # downscaled
        manip2 = pipeline.create(dai.node.ImageManip)
        manip2.initialConfig.setResize(640, 400)
        manip2.initialConfig.setKeepAspectRatio(False)

        manip1.out.link(manip2.inputImage)
        manip2.out.link(cam_xout.input)

    return pipeline

with dai.Device() as device:

    calibData = device.readCalibration()
    mesh = getMesh(calibData)
    pipeline = create_pipeline(mesh)
    device.startPipeline(pipeline)

    cam_out = device.getOutputQueue("cam_out", 4, False)

    time.sleep(2)
    try:
        while True:
            frame = cam_out.get().getCvFrame()
            if frame is not None:
                print('getting Image')
                cv2.imshow("Camera_view", frame)
                if cv2.waitKey(1) == ord('q'):
                    break
            else:
                print("frame not found")

    except:
        pass

cv2.destroyAllWindows()