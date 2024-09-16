import cv2
import depthai as dai
import numpy as np

camRes = dai.ColorCameraProperties.SensorResolution.THE_1200_P
camSocket = dai.CameraBoardSocket.CAM_C

meshCellSize = 32

def getMesh(calibData, size):
    M1 = np.array(calibData.getCameraIntrinsics(camSocket, size[0], size[1]))
    mapX, mapY = cv2.initUndistortRectifyMap(M1, None, None, None, size, cv2.CV_32FC1)

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

mesh, meshWidth, meshHeight = getMesh(calibData, cam.getVideoSize())
print(f"Mesh size: {meshWidth}x{meshHeight}")
cam.loadMeshData(list(mesh.flatten().tobytes()))
cam.setMeshSize(meshWidth, meshHeight)

dist_xout = pipeline.create(dai.node.XLinkOut)
dist_xout.setStreamName("WARPED")
cam.video.link(dist_xout.input)

inmesh_xin = pipeline.create(dai.node.XLinkIn)
inmesh_xin.setStreamName("inmesh")
inmesh_xin.out.link(cam.inputMesh)

with device:

    device.startPipeline(pipeline)

    queues = [device.getOutputQueue(name, 4, False) for name in ['WARPED']]

    inWarpQ = device.getInputQueue("inmesh")

    while True:
        for q in queues:
            frame = q.get().getCvFrame()
            cv2.imshow(q.getName(), frame)

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('x'):

            mesh = np.random.rand(meshHeight, meshWidth, 2).astype(np.float32)

            print(mesh.shape)

            runtimeMesh = dai.Buffer()
            runtimeMesh.setData(list(mesh.flatten().tobytes()))

            inWarpQ.send(runtimeMesh)