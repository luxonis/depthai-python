import cv2
import depthai as dai
import numpy as np

np.set_printoptions(precision=2, suppress=True)

camRes = dai.ColorCameraProperties.SensorResolution.THE_1200_P
camSocket = dai.CameraBoardSocket.CAM_C

meshCellSize = 32

def getMesh(calibData, size):
    M1 = np.array(calibData.getCameraIntrinsics(camSocket, size[0], size[1]))
    d1 = np.array(calibData.getDistortionCoefficients(camSocket))
    mapX, mapY = cv2.initUndistortRectifyMap(M1, d1, None, None, size, cv2.CV_32FC1)

    mesh0 = []

    w = 0
    h = 0
    # Creates subsampled mesh which will be loaded on to device to undistort the image
    for y in range(mapX.shape[0] + 1):
        if y % meshCellSize == 0:
            h += 1
            w = 0

            rowLeft = []
            for x in range(mapX.shape[1] + 1):
                if x % meshCellSize == 0:
                    w += 1
                    if y == mapX.shape[0] and x == mapX.shape[1]:
                        rowLeft.append(mapY[y - 1, x - 1])
                        rowLeft.append(mapX[y - 1, x - 1])
                    elif y == mapX.shape[0]:
                        rowLeft.append(mapY[y - 1, x])
                        rowLeft.append(mapX[y - 1, x])
                    elif x == mapX.shape[1]:
                        rowLeft.append(mapY[y, x - 1])
                        rowLeft.append(mapX[y, x - 1])
                    else:
                        rowLeft.append(mapY[y, x])
                        rowLeft.append(mapX[y, x])
            if (mapX.shape[1] % meshCellSize) % 2 != 0:
                rowLeft.append(0)
                rowLeft.append(0)
                w += 1

            mesh0.append(rowLeft)

    mesh = np.array(mesh0).reshape(h, w, 2)
    # print("mesh shape", mesh.shape)

    return mesh, w, h


device = dai.Device()

calibData = device.readCalibration()

pipeline = dai.Pipeline()

cam = pipeline.create(dai.node.ColorCamera)
cam.setBoardSocket(camSocket)
cam.setResolution(camRes)
cam.setVideoSize(1280, 800)

mesh, meshWidth, meshHeight = getMesh(calibData, cam.getVideoSize())
print(f"Mesh size: {meshWidth}x{meshHeight}")
cam.loadMeshData(list(mesh.flatten().tobytes()))
cam.setMeshSize(meshWidth, meshHeight)

dist_xout = pipeline.create(dai.node.XLinkOut)
dist_xout.setStreamName('WARPED')
cam.video.link(dist_xout.input)

inmesh_xin = pipeline.create(dai.node.XLinkIn)
inmesh_xin.setStreamName("inmesh")
inmesh_xin.out.link(cam.inputMesh)

xoutIsp = pipeline.create(dai.node.XLinkOut)
xoutIsp.setStreamName('UNWARPED')

xoutIsp.input.setBlocking(False)
xoutIsp.input.setQueueSize(1)

cam.isp.link(xoutIsp.input)

with device:

    device.startPipeline(pipeline)

    queues = [device.getOutputQueue(name, 4, False) for name in ['WARPED', 'UNWARPED']]

    inWarpQ = device.getInputQueue("inmesh")

    while True:
        for q in queues:
            frame = q.get().getCvFrame()
            cv2.imshow(q.getName(), frame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('x'):

            M1 = calibData.getCameraIntrinsics(camSocket, cam.getVideoSize()[0], cam.getVideoSize()[1])
            M1[0][0] *= 1.1
            M1[1][1] *= 1.1
            M1 = np.array(M1)

            print(f"Setting new intrinsics: {M1}")

            calibData.setCameraIntrinsics(camSocket, M1, cam.getVideoSize()[0], cam.getVideoSize()[1])

            mesh, meshWidth, meshHeight = getMesh(calibData, cam.getVideoSize())

            runtimeMesh = dai.Buffer()
            runtimeMesh.setData(list(mesh.flatten().tobytes()))

            inWarpQ.send(runtimeMesh)
        elif key == ord('d'):
            d1 = calibData.getDistortionCoefficients(camSocket)
            d1 = [d * 1.1 for d in d1]
            print(f"Setting new distortion coefficients: {d1}")
            calibData.setDistortionCoefficients(camSocket, d1)

            mesh, meshWidth, meshHeight = getMesh(calibData, cam.getVideoSize())

            runtimeMesh = dai.Buffer()
            runtimeMesh.setData(list(mesh.flatten().tobytes()))

            inWarpQ.send(runtimeMesh)
