import depthai as dai
from typing import List, Tuple

def print_system_information(info: dai.SystemInformation):
    print(
        "Ddr: used / total - %.2f / %.2f MiB"
        % (info.ddrMemoryUsage.used
        / (1024.0 * 1024.0),
        info.ddrMemoryUsage.total / (1024.0 * 1024.0),)
    )
    print(
        "Cmx: used / total - %.2f / %.2f MiB"
        % (info.cmxMemoryUsage.used
        / (1024.0 * 1024.0),
        info.cmxMemoryUsage.total / (1024.0 * 1024.0),)
    )
    print(
        "LeonCss heap: used / total - %.2f / %.2f MiB"
        % (info.leonCssMemoryUsage.used
        / (1024.0 * 1024.0),
        info.leonCssMemoryUsage.total / (1024.0 * 1024.0),)
    )
    print(
        "LeonMss heap: used / total - %.2f / %.2f MiB"
        % (info.leonMssMemoryUsage.used
        / (1024.0 * 1024.0),
        info.leonMssMemoryUsage.total / (1024.0 * 1024.0),)
    )
    t = info.chipTemperature
    print(
        "Chip temperature - average: %.2f, css: %.2f, mss: %.2f, upa: %.2f, dss: %.2f"
        % (t.average,
        t.css,
        t.mss,
        t.upa,
        t.dss,)
    )
    print(
        "Cpu usage - Leon CSS: %.2f %%, Leon MSS: %.2f %%"
        % (info.leonCssCpuUsage.average
        * 100,
        info.leonMssCpuUsage.average * 100)
    )

def get_or_download_yolo_blob() -> str:
    import os
    import subprocess
    import sys

    this_file = os.path.realpath(__file__)
    this_dir = os.path.dirname(this_file)
    examples_dir = os.path.join(this_dir, "..", "examples")
    models_dir = os.path.join(examples_dir, "models")
    blob_path = os.path.join(models_dir, "yolo-v4-tiny-tf_openvino_2021.4_6shave.blob")
    downloader_cmd = [sys.executable, f"{examples_dir}/downloader/downloader.py", "--name", "tiny-yolo", "--cache_dir", f"{examples_dir}/downloader/", "--num_attempts", "5", "-o", f"{examples_dir}/models"]
    subprocess.run(downloader_cmd, check=True)
    return blob_path


def stress_test(mxid: str = ""):
    import time
    success, device_info = dai.Device.getDeviceByMxId(mxid)
    cam_args = []  # Device info or no args at all
    if success:
        cam_args.append(device_info)
    with dai.Device(*cam_args) as device:
        pipeline, outputs = build_pipeline(device)
        device.startPipeline(pipeline)
        start_time = time.time()
        queues = [device.getOutputQueue(name, size, False) for name, size in outputs if name != "sys_log"]
        sys_info_q = device.getOutputQueue("sys_log", 1, False)
        usb_speed = device.getUsbSpeed()
        while True:
            for queue in queues:
                queue.tryGetAll()
            
            sys_info = sys_info_q.tryGet()
            if sys_info:
                print("----------------------------------------")
                print(f"[{int(time.time() - start_time)}s] Usb speed {usb_speed}")
                print("----------------------------------------")
                print_system_information(sys_info)

RGB_FPS = 20
MONO_FPS = 20
ENCODER_FPS = 10

def build_pipeline(device: dai.Device) -> Tuple[dai.Pipeline, List[Tuple[str, int]]]:
    cams = device.getConnectedCameraFeatures()
    try:
        calib = device.readCalibration2()
    except:
        print("Couln't read calibration data from device, exiting...")
        exit(-1)

    eeprom = calib.getEepromData()
    left_socket = eeprom.stereoRectificationData.leftCameraSocket
    right_socket = eeprom.stereoRectificationData.rightCameraSocket
    align_socket = [
        cam.socket
        for cam in cams
        if cam.supportedTypes[0] == dai.CameraSensorType.COLOR
    ]
    is_align_socket_color = not align_socket is None
    if not is_align_socket_color:
        print(f"No color camera found, aligning depth with {left_socket}")
        align_socket = [left_socket]
    align_socket = align_socket[0]

    xlink_outs: List[Tuple[str, int]] = [] # [(name, size), ...]
    
    pipeline = dai.Pipeline()
    sys_log = pipeline.createSystemLogger()
    sys_log.setRate(0.2)
    sys_log_out = pipeline.createXLinkOut()
    sys_log_out.setStreamName("sys_log")
    sys_log.out.link(sys_log_out.input)
    sys_log_out.input.setBlocking(False)
    sys_log_out.input.setQueueSize(1)


    left: dai.Node = None
    right: dai.Node = None
    color_cam: dai.Node | None = None # Used for spatial detection network (if available)

    for cam in cams:
        node = None
        cam_kind = cam.supportedTypes[0]
        if cam_kind == dai.CameraSensorType.MONO:
            mono = pipeline.createMonoCamera()
            node = mono
            mono.setBoardSocket(cam.socket)
            mono.setResolution(dai.MonoCameraProperties.SensorResolution.THE_400_P)
            mono.setFps(MONO_FPS)
        elif cam_kind == dai.CameraSensorType.COLOR:
            color = pipeline.createColorCamera()
            node = color
            color.setBoardSocket(cam.socket)
            color.setResolution(
                dai.ColorCameraProperties.SensorResolution.THE_4_K
            )  # TODO might not work with OAK D SR
            color.setFps(RGB_FPS)
            if is_align_socket_color:
                if cam.socket == align_socket:
                    color_cam = color
                    color.setPreviewSize(416, 416)
                    color.setColorOrder(dai.ColorCameraProperties.ColorOrder.BGR)
                    color.setInterleaved(False)

                    xlink_preview = pipeline.createXLinkOut()
                    xlink_preview.setStreamName("preview")
                    color.preview.link(xlink_preview.input)
                    xlink_outs.append(("preview", 4))

            else:
                color_cam = color
        elif cam_kind == dai.CameraSensorType.TOF:
            tof = pipeline.create(dai.node.ToF)
            node = tof
            cam = pipeline.create(dai.node.ColorCamera)
            cam.setBoardSocket(cam.socket)
            cam.raw.link(tof.input)
            tof_xout = pipeline.createXLinkOut()
            tof_xout.setStreamName("tof")
            tof.depth.link(tof_xout.input)
            tofConfig = tof.initialConfig.get()
            tofConfig.depthParams.freqModUsed = dai.RawToFConfig.DepthParams.TypeFMod.MIN
            tofConfig.depthParams.avgPhaseShuffle = False
            tofConfig.depthParams.minimumAmplitude = 3.0
            tof.initialConfig.set(tofConfig)
            xlink_outs.append(("tof", 4))
        else:
            print(f"Unsupported camera type: {cam.supportedTypes[0]}")
            exit(-1)

        output = "out" if cam_kind == dai.CameraSensorType.MONO else "video"
        if cam.socket == left_socket:
            left = node
        elif cam.socket == right_socket:
            right = node

        video_encoder = pipeline.createVideoEncoder()
        video_encoder.setDefaultProfilePreset(
            ENCODER_FPS, dai.VideoEncoderProperties.Profile.H264_MAIN
        )

        edge_detector = pipeline.createEdgeDetector()
        if cam_kind == dai.CameraSensorType.COLOR:
            edge_detector.setMaxOutputFrameSize(8294400)

        getattr(node, output).link(video_encoder.input)
        getattr(node, output).link(edge_detector.inputImage)

        ve_xlink = pipeline.createXLinkOut()
        stream_name = f"{cam.socket}.ve_out"
        ve_xlink.setStreamName(stream_name)
        video_encoder.bitstream.link(ve_xlink.input)
        xlink_outs.append((stream_name, 30))

        edge_detector_xlink = pipeline.createXLinkOut()
        stream_name = f"{cam.socket}.edge_detector"
        edge_detector_xlink.setStreamName(stream_name)
        edge_detector.outputImage.link(edge_detector_xlink.input)
        xlink_outs.append((stream_name, 8))
    
    if left and right:
        stereo = pipeline.createStereoDepth()
        output = "out" if hasattr(left, "out") else "video"
        getattr(left, output).link(stereo.left)
        getattr(right, output).link(stereo.right)
        stereo.setDefaultProfilePreset(dai.node.StereoDepth.PresetMode.HIGH_DENSITY)
        stereo.setDepthAlign(align_socket)
        stereo.setOutputSize(left.getResolutionWidth(), left.getResolutionHeight())

        if color_cam:
            yolo = pipeline.createYoloSpatialDetectionNetwork()
            blob_path = get_or_download_yolo_blob()
            yolo.setBlobPath(blob_path)
            yolo.setConfidenceThreshold(0.5)
            yolo.input.setBlocking(False)
            yolo.setBoundingBoxScaleFactor(0.5)
            yolo.setDepthLowerThreshold(100)
            yolo.setDepthUpperThreshold(5000)
            yolo.setNumClasses(80)
            yolo.setCoordinateSize(4)
            yolo.setAnchors([10, 14, 23, 27, 37, 58, 81, 82, 135, 169, 344, 319])
            yolo.setAnchorMasks({"side26": [1, 2, 3], "side13": [3, 4, 5]})
            yolo.setIouThreshold(0.5)
            color_cam.preview.link(yolo.input)
            stereo.depth.link(yolo.inputDepth)

            xout_depth = pipeline.createXLinkOut()
            depth_q_name = "depth"
            xout_depth.setStreamName(depth_q_name)
            yolo.passthroughDepth.link(xout_depth.input)
            xlink_outs.append((depth_q_name, 4))

            xout_yolo = pipeline.createXLinkOut()
            yolo_q_name = "yolo"
            xout_yolo.setStreamName(yolo_q_name)
            yolo.out.link(xout_yolo.input)
            xlink_outs.append((yolo_q_name, 4))
        else:
            print("Device doesn't have color camera, skipping spatial detection network creation...")

    else:
        print("Device doesn't have stereo cameras, skipping depth and spatial detection network creation...")
    
    return (pipeline, xlink_outs)



if __name__ == "__main__":
    stress_test()
