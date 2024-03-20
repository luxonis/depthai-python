import cv2
import depthai as dai
import numpy as np

camRes = dai.ColorCameraProperties.SensorResolution.THE_1080_P
camSocket = dai.CameraBoardSocket.CAM_A
ispScale = (1,2)

def getMesh(calibData, ispSize):
    M1 = np.array(calibData.getCameraIntrinsics(camSocket, ispSize[0], ispSize[1]))
    d1 = np.array(calibData.getDistortionCoefficients(camSocket))
    R1 = np.identity(3)
    mapX, mapY = cv2.initUndistortRectifyMap(M1, d1, R1, M1, ispSize, cv2.CV_32FC1)

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

    return mesh, meshWidth, meshHeight

def create_pipeline(calibData):
    pipeline = dai.Pipeline()

    cam = pipeline.create(dai.node.ColorCamera)
    cam.setIspScale(ispScale)
    cam.setBoardSocket(camSocket)
    cam.setResolution(camRes)

    manip = pipeline.create(dai.node.ImageManip)
    mesh, meshWidth, meshHeight = getMesh(calibData, cam.getIspSize())
    manip.setWarpMesh(mesh, meshWidth, meshHeight)
    manip.setMaxOutputFrameSize(cam.getIspWidth() * cam.getIspHeight() * 3 // 2)
    cam.isp.link(manip.inputImage)

    cam_xout = pipeline.create(dai.node.XLinkOut)
    cam_xout.setStreamName("Undistorted")
    manip.out.link(cam_xout.input)

    dist_xout = pipeline.create(dai.node.XLinkOut)
    dist_xout.setStreamName("Distorted")
    cam.isp.link(dist_xout.input)

    return pipeline

with dai.Device() as device:

    calibData = device.readCalibration()
    pipeline = create_pipeline(calibData)
    device.startPipeline(pipeline)

    queues = [device.getOutputQueue(name, 4, False) for name in ['Undistorted', 'Distorted']]

    while True:
        for q in queues:
            frame = q.get().getCvFrame()
            cv2.imshow(q.getName(), frame)

        if cv2.waitKey(1) == ord('q'):
            break