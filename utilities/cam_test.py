#!/usr/bin/env python3

"""
Use 'T' to trigger autofocus, 'IOKL,.'
for manual exposure/focus:
  Control:      key[dec/inc]  min..max
  exposure time:     I   O      1..33000 [us]
  sensitivity iso:   K   L    100..1600
  focus:             ,   .      0..255 [far..near]
To go back to auto controls:
  'E' - autoexposure
  'F' - autofocus (continuous)

Other controls:
'1' - AWB lock (true / false)
'2' - AE lock (true / false)
'3' - Select control: AWB mode
'4' - Select control: AE compensation
'5' - Select control: anti-banding/flicker mode
'6' - Select control: effect mode
'7' - Select control: brightness
'8' - Select control: contrast
'9' - Select control: saturation
'0' - Select control: sharpness
'[' - Select control: luma denoise
']' - Select control: chroma denoise
'\' - Select control: scene mode
';' - Select control: control mode
''' - Select control: capture intent
'a' 'd' - Increase/decrease dot projector intensity
'w' 's' - Increase/decrease flood LED intensity

For the 'Select control: ...' options, use these keys to modify the value:
  '-' or '_' to decrease
  '+' or '=' to increase

'/' to toggle printing camera settings: exposure, ISO, lens position, color temperature
"""

import os
#os.environ["DEPTHAI_LEVEL"] = "debug"

import cv2
import numpy as np
import argparse
import collections
import time
from itertools import cycle
from pathlib import Path
import sys
import signal


def socket_type_pair(arg):
    socket, type = arg.split(',')
    if not (socket in ['rgb', 'left', 'right', 'cama', 'camb', 'camc', 'camd']):
        raise ValueError("")
    if not (type in ['m', 'mono', 'c', 'color', 't', 'tof']):
        raise ValueError("")
    is_color = True if type in ['c', 'color'] else False
    is_tof = True if type in ['t', 'tof'] else False
    return [socket, is_color, is_tof]


parser = argparse.ArgumentParser()
parser.add_argument('-cams', '--cameras', type=socket_type_pair, nargs='+',
                    default=[['rgb', True, False], ['left', False, False],
                             ['right', False, False], ['camd', True, False]],
                    help="Which camera sockets to enable, and type: c[olor] / m[ono] / t[of]. "
                    "E.g: -cams rgb,m right,c . Default: rgb,c left,m right,m camd,c")
parser.add_argument('-mres', '--mono-resolution', type=int, default=800, choices={480, 400, 720, 800},
                    help="Select mono camera resolution (height). Default: %(default)s")
parser.add_argument('-cres', '--color-resolution', default='1080', choices={'720', '800', '1080', '1012', '1200', '1520', '4k', '5mp', '12mp', '13mp', '48mp'},
                    help="Select color camera resolution / height. Default: %(default)s")
parser.add_argument('-rot', '--rotate', const='all', choices={'all', 'rgb', 'mono'}, nargs="?",
                    help="Which cameras to rotate 180 degrees. All if not filtered")
parser.add_argument('-fps', '--fps', type=float, default=30,
                    help="FPS to set for all cameras")
parser.add_argument('-isp3afps', '--isp3afps', type=int, default=0,
                    help="3A FPS to set for all cameras")
parser.add_argument('-ds', '--isp-downscale', default=1, type=int,
                    help="Downscale the ISP output by this factor")
parser.add_argument('-rs', '--resizable-windows', action='store_true',
                    help="Make OpenCV windows resizable. Note: may introduce some artifacts")
parser.add_argument('-tun', '--camera-tuning', type=Path,
                    help="Path to custom camera tuning database")
parser.add_argument('-raw', '--enable-raw', default=False, action="store_true",
                    help='Enable the RAW camera streams')
parser.add_argument('-tofraw', '--tof-raw', action='store_true',
                    help="Show just ToF raw output instead of post-processed depth")
parser.add_argument('-tofamp', '--tof-amplitude', action='store_true',
                    help="Show also ToF amplitude output alongside depth")
parser.add_argument('-tofcm', '--tof-cm', action='store_true',
                    help="Show ToF depth output in centimeters, capped to 255")
parser.add_argument('-tofmedian', '--tof-median', choices=[0,3,5,7], default=5, type=int,
                    help="ToF median filter kernel size")
parser.add_argument('-rgbprev', '--rgb-preview', action='store_true',
                    help="Show RGB `preview` stream instead of full size `isp`")
parser.add_argument('-show', '--show-meta', action='store_true',
                    help="List frame metadata (seqno, timestamp, exp, iso etc). Can also toggle with `\`")

parser.add_argument('-d', '--device', default="", type=str,
                    help="Optional MX ID of the device to connect to.")

parser.add_argument('-ctimeout', '--connection-timeout', default=30000,
                    help="Connection timeout in ms. Default: %(default)s (sets DEPTHAI_CONNECTION_TIMEOUT environment variable)")

parser.add_argument('-btimeout', '--boot-timeout', default=30000,
                    help="Boot timeout in ms. Default: %(default)s (sets DEPTHAI_BOOT_TIMEOUT environment variable)")

args = parser.parse_args()

# Set timeouts before importing depthai
os.environ["DEPTHAI_CONNECTION_TIMEOUT"] = str(args.connection_timeout)
os.environ["DEPTHAI_BOOT_TIMEOUT"] = str(args.boot_timeout)
import depthai as dai

if len(sys.argv) == 1:
    import cam_test_gui
    cam_test_gui.main()

cam_list = []
cam_type_color = {}
cam_type_tof = {}
print("Enabled cameras:")
for socket, is_color, is_tof in args.cameras:
    cam_list.append(socket)
    cam_type_color[socket] = is_color
    cam_type_tof[socket] = is_tof
    print(socket.rjust(7), ':', 'tof' if is_tof else 'color' if is_color else 'mono')

print("DepthAI version:", dai.__version__)
print("DepthAI path:", dai.__file__)

cam_socket_opts = {
    'rgb'  : dai.CameraBoardSocket.CAM_A,
    'left' : dai.CameraBoardSocket.CAM_B,
    'right': dai.CameraBoardSocket.CAM_C,
    'cama' : dai.CameraBoardSocket.CAM_A,
    'camb' : dai.CameraBoardSocket.CAM_B,
    'camc' : dai.CameraBoardSocket.CAM_C,
    'camd' : dai.CameraBoardSocket.CAM_D,
}

rotate = {
    'rgb': args.rotate in ['all', 'rgb'],
    'left': args.rotate in ['all', 'mono'],
    'right': args.rotate in ['all', 'mono'],
    'cama': args.rotate in ['all', 'rgb'],
    'camb': args.rotate in ['all', 'mono'],
    'camc': args.rotate in ['all', 'mono'],
    'camd': args.rotate in ['all', 'rgb'],
}

mono_res_opts = {
    400: dai.MonoCameraProperties.SensorResolution.THE_400_P,
    480: dai.MonoCameraProperties.SensorResolution.THE_480_P,
    720: dai.MonoCameraProperties.SensorResolution.THE_720_P,
    800: dai.MonoCameraProperties.SensorResolution.THE_800_P,
    1200: dai.MonoCameraProperties.SensorResolution.THE_1200_P,
}

color_res_opts = {
    '720':  dai.ColorCameraProperties.SensorResolution.THE_720_P,
    '800':  dai.ColorCameraProperties.SensorResolution.THE_800_P,
    '1080': dai.ColorCameraProperties.SensorResolution.THE_1080_P,
    '1012': dai.ColorCameraProperties.SensorResolution.THE_1352X1012,
    '1200': dai.ColorCameraProperties.SensorResolution.THE_1200_P,
    '1520': dai.ColorCameraProperties.SensorResolution.THE_2024X1520,
    '4k':   dai.ColorCameraProperties.SensorResolution.THE_4_K,
    '5mp': dai.ColorCameraProperties.SensorResolution.THE_5_MP,
    '12mp': dai.ColorCameraProperties.SensorResolution.THE_12_MP,
    '13mp': dai.ColorCameraProperties.SensorResolution.THE_13_MP,
    '48mp': dai.ColorCameraProperties.SensorResolution.THE_48_MP,
}

def clamp(num, v0, v1):
    return max(v0, min(num, v1))

# Calculates FPS over a moving window, configurable
class FPS:
    def __init__(self, window_size=30):
        self.dq = collections.deque(maxlen=window_size)
        self.fps = 0

    def update(self, timestamp=None):
        if timestamp == None:
            timestamp = time.monotonic()
        count = len(self.dq)
        if count > 0:
            self.fps = count / (timestamp - self.dq[0])
        self.dq.append(timestamp)

    def get(self):
        return self.fps

class Cycle:
    def __init__(self, enum_type, start_item=None):
        self.items = [item for name, item in vars(enum_type).items() if name.isupper()]
        # If start_item is provided, set the index to its position. Otherwise, default to 0
        self.index = self.items.index(start_item) if start_item else 0

    def step(self, n):
        self.index = (self.index + n) % len(self.items)
        return self.items[self.index]

    def next(self):
        return self.step(1)

    def prev(self):
        return self.step(-1)

# Start defining a pipeline
pipeline = dai.Pipeline()
# Uncomment to get better throughput
# pipeline.setXLinkChunkSize(0)

control = pipeline.createXLinkIn()
control.setStreamName('control')

xinTofConfig = pipeline.createXLinkIn()
xinTofConfig.setStreamName('tofConfig')

cam = {}
tof = {}
xout = {}
xout_raw = {}
xout_tof_amp = {}
streams = []
tofConfig = {}
for c in cam_list:
    tofEnableRaw = False
    xout[c] = pipeline.createXLinkOut()
    xout[c].setStreamName(c)
    streams.append(c)
    if cam_type_tof[c]:
        cam[c] = pipeline.create(dai.node.ColorCamera)  # .Camera
        if args.tof_raw:
            tofEnableRaw = True
        else:
            tof[c] = pipeline.create(dai.node.ToF)
            cam[c].raw.link(tof[c].input)
            tof[c].depth.link(xout[c].input)
            xinTofConfig.out.link(tof[c].inputConfig)
            tofConfig = tof[c].initialConfig.get()
            tofConfig.depthParams.freqModUsed = dai.RawToFConfig.DepthParams.TypeFMod.MIN
            tofConfig.depthParams.avgPhaseShuffle = False
            tofConfig.depthParams.minimumAmplitude = 3.0

            if args.tof_median == 0:
                tofConfig.depthParams.median = dai.MedianFilter.MEDIAN_OFF
            elif args.tof_median == 3:
                tofConfig.depthParams.median = dai.MedianFilter.KERNEL_3x3
            elif args.tof_median == 5:
                tofConfig.depthParams.median = dai.MedianFilter.KERNEL_5x5
            elif args.tof_median == 7:
                tofConfig.depthParams.median = dai.MedianFilter.KERNEL_7x7

            tof[c].initialConfig.set(tofConfig)
            if args.tof_amplitude:
                amp_name = 'tof_amplitude_' + c
                xout_tof_amp[c] = pipeline.create(dai.node.XLinkOut)
                xout_tof_amp[c].setStreamName(amp_name)
                streams.append(amp_name)
                tof[c].amplitude.link(xout_tof_amp[c].input)
    elif cam_type_color[c]:
        cam[c] = pipeline.createColorCamera()
        cam[c].setResolution(color_res_opts[args.color_resolution])
        cam[c].setIspScale(1, args.isp_downscale)
        # cam[c].initialControl.setManualFocus(85) # TODO
        if args.rgb_preview:
            cam[c].preview.link(xout[c].input)
        else:
            cam[c].isp.link(xout[c].input)
    else:
        cam[c] = pipeline.createMonoCamera()
        cam[c].setResolution(mono_res_opts[args.mono_resolution])
        cam[c].out.link(xout[c].input)
    cam[c].setBoardSocket(cam_socket_opts[c])
    # Num frames to capture on trigger, with first to be discarded (due to degraded quality)
    # cam[c].initialControl.setExternalTrigger(2, 1)
    # cam[c].initialControl.setStrobeExternal(48, 1)
    # cam[c].initialControl.setFrameSyncMode(dai.CameraControl.FrameSyncMode.INPUT)

    # cam[c].initialControl.setManualExposure(15000, 400) # exposure [us], iso
    # When set, takes effect after the first 2 frames
    # cam[c].initialControl.setManualWhiteBalance(4000)  # light temperature in K, 1000..12000
    # cam[c].initialControl.setAutoExposureLimit(5000)  # can also be updated at runtime
    control.out.link(cam[c].inputControl)
    if rotate[c]:
        cam[c].setImageOrientation(dai.CameraImageOrientation.ROTATE_180_DEG)
    cam[c].setFps(args.fps)
    if args.isp3afps:
        cam[c].setIsp3aFps(args.isp3afps)

    if args.enable_raw or tofEnableRaw:
        raw_name = 'raw_' + c
        xout_raw[c] = pipeline.create(dai.node.XLinkOut)
        xout_raw[c].setStreamName(raw_name)
        streams.append(raw_name)
        cam[c].raw.link(xout_raw[c].input)
        cam[c].setRawOutputPacked(False)

if args.camera_tuning:
    pipeline.setCameraTuningBlobPath(str(args.camera_tuning))

def exit_cleanly(signum, frame):
    print("Exiting cleanly")
    cv2.destroyAllWindows()
    sys.exit(0)

signal.signal(signal.SIGINT, exit_cleanly)


# Pipeline is defined, now we can connect to the device
device = dai.Device.getDeviceByMxId(args.device)
dai_device_args = [pipeline]
if device[0]:
    dai_device_args.append(device[1])
with dai.Device(*dai_device_args) as device:
    # print('Connected cameras:', [c.name for c in device.getConnectedCameras()])
    print('Connected cameras:')
    cam_name = {}
    for p in device.getConnectedCameraFeatures():
        print(
            f' -socket {p.socket.name:6}: {p.sensorName:6} {p.width:4} x {p.height:4} focus:', end='')
        print('auto ' if p.hasAutofocus else 'fixed', '- ', end='')
        print(*[type.name for type in p.supportedTypes])
        cam_name[p.socket.name] = p.sensorName
        if args.enable_raw:
            cam_name['raw_'+p.socket.name] = p.sensorName
        if args.tof_amplitude:
            cam_name['tof_amplitude_'+p.socket.name] = p.sensorName

    print('USB speed:', device.getUsbSpeed().name)

    print('IR drivers:', device.getIrDrivers())

    q = {}
    fps_host = {}  # FPS computed based on the time we receive frames in app
    fps_capt = {}  # FPS computed based on capture timestamps from device
    for c in streams:
        q[c] = device.getOutputQueue(name=c, maxSize=4, blocking=False)
        # The OpenCV window resize may produce some artifacts
        if args.resizable_windows:
            cv2.namedWindow(c, cv2.WINDOW_NORMAL)
            cv2.resizeWindow(c, (640, 480))
        fps_host[c] = FPS()
        fps_capt[c] = FPS()

    controlQueue = device.getInputQueue('control')
    tofCfgQueue = device.getInputQueue('tofConfig')

    # Manual exposure/focus set step
    EXP_STEP = 500  # us
    ISO_STEP = 50
    LENS_STEP = 1 / 1024
    DOT_STEP = 0.05
    FLOOD_STEP = 0.05
    DOT_MAX = 1
    FLOOD_MAX = 1

    # Defaults and limits for manual focus/exposure controls
    lensPos = 0.59
    lensMin = 0.0
    lensMax = 1.0

    expTime = 20000
    expMin = 1
    expMax = 33000

    sensIso = 800
    sensMin = 100
    sensMax = 1600

    dotIntensity = 0
    floodIntensity = 0

    awb_mode = Cycle(dai.CameraControl.AutoWhiteBalanceMode)
    anti_banding_mode = Cycle(dai.CameraControl.AntiBandingMode)
    effect_mode = Cycle(dai.CameraControl.EffectMode)
    scene_mode = Cycle(dai.CameraControl.SceneMode)
    control_mode = Cycle(dai.CameraControl.ControlMode)
    capture_intent = Cycle(dai.CameraControl.CaptureIntent)

    ae_comp = 0
    ae_lock = False
    awb_lock = False
    saturation = 0
    contrast = 0
    brightness = 0
    sharpness = 0
    luma_denoise = 0
    chroma_denoise = 0
    control = 'none'
    show = args.show_meta

    jet_custom = cv2.applyColorMap(np.arange(256, dtype=np.uint8), cv2.COLORMAP_JET)
    jet_custom[0] = [0, 0, 0]

    print("Cam:", *['     ' + c.ljust(8)
          for c in cam_list], "[host | capture timestamp]")

    capture_list = []
    while True:
        for c in streams:
            try:
                pkt = q[c].tryGet()
            except Exception as e:
                print(e)
                exit_cleanly(0, 0)
            if pkt is not None:
                fps_host[c].update()
                fps_capt[c].update(pkt.getTimestamp().total_seconds())
                width, height = pkt.getWidth(), pkt.getHeight()
                frame = pkt.getCvFrame()
                cam_skt = c.split('_')[-1]
                if cam_type_tof[cam_skt] and not (c.startswith('raw_') or c.startswith('tof_amplitude_')):
                    if args.tof_cm:
                        # pixels represent `cm`, capped to 255. Value can be checked hovering the mouse
                        frame = (frame // 10).clip(0, 255).astype(np.uint8)
                    else:
                        frame = (frame.view(np.int16).astype(float))
                        frame = cv2.normalize(frame, frame, alpha=255, beta=0, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)
                        frame = cv2.applyColorMap(frame, jet_custom)
                if show:
                    txt = f"[{c:5}, {pkt.getSequenceNum():4}, {pkt.getTimestamp().total_seconds():.6f}] "
                    txt += f"Exp: {pkt.getExposureTime().total_seconds()*1000:6.3f} ms, "
                    txt += f"ISO: {pkt.getSensitivity():4}, "
                    txt += f"Lens pos: {pkt.getLensPosition():3}, "
                    txt += f"Color temp: {pkt.getColorTemperature()} K"
                    if needs_newline:
                        print()
                        needs_newline = False
                    print(txt)
                capture = c in capture_list
                if capture:
                    capture_file_info = ('capture_' + c + '_' + cam_name[cam_socket_opts[cam_skt].name]
                         + '_' + str(width) + 'x' + str(height)
                         + '_' + capture_time
                         + '_exp_' + str(int(pkt.getExposureTime().total_seconds()*1e6))
                         + '_iso_' + str(pkt.getSensitivity())
                         + '_lens_' + str(pkt.getLensPosition())
                         + '_' + str(pkt.getColorTemperature()) + 'K'
                         + '_' + str(pkt.getSequenceNum())
                        )
                    capture_list.remove(c)
                    print()
                if c.startswith('raw_') or c.startswith('tof_amplitude_'):
                    if capture:
                        filename = capture_file_info + '_10bit.bw'
                        print('Saving:', filename)
                        frame.tofile(filename)
                    # Full range for display, use bits [15:6] of the 16-bit pixels
                    type = pkt.getType()
                    multiplier = 1
                    if type == dai.ImgFrame.Type.RAW10: multiplier = (1 << (16-10))
                    if type == dai.ImgFrame.Type.RAW12: multiplier = (1 << (16-4))
                    frame = frame * multiplier
                    # Debayer as color for preview/png
                    if cam_type_color[cam_skt]:
                        # See this for the ordering, at the end of page:
                        # https://docs.opencv.org/4.5.1/de/d25/imgproc_color_conversions.html
                        # TODO add bayer order to ImgFrame getType()
                        frame = cv2.cvtColor(frame, cv2.COLOR_BayerGB2BGR)
                else:
                    # Save YUV too, but only when RAW is also enabled (for tuning purposes)
                    if capture and args.enable_raw:
                        payload = pkt.getData()
                        filename = capture_file_info + '_P420.yuv'
                        print('Saving:', filename)
                        payload.tofile(filename)
                if capture:
                    filename = capture_file_info + '.png'
                    print('Saving:', filename)
                    cv2.imwrite(filename, frame)
                cv2.imshow(c, frame)
        print("\rFPS:",
              *["{:6.2f}|{:6.2f}".format(fps_host[c].get(), fps_capt[c].get()) for c in cam_list],
              end=' ', flush=True)
        needs_newline = True

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('/'):
            show = not show
            # Print empty string as FPS status new-line separator
            print("" if show else "Printing camera settings: OFF")
        elif key == ord('c'):
            capture_list = streams.copy()
            capture_time = time.strftime('%Y%m%d_%H%M%S')
        elif key == ord('g') and tof:
            f_mod = dai.RawToFConfig.DepthParams.TypeFMod.MAX if tofConfig.depthParams.freqModUsed  == dai.RawToFConfig.DepthParams.TypeFMod.MIN else dai.RawToFConfig.DepthParams.TypeFMod.MIN
            print("ToF toggling f_mod value to:", f_mod)
            tofConfig.depthParams.freqModUsed = f_mod
            tofCfgQueue.send(tofConfig)
        elif key == ord('h') and tof:
            tofConfig.depthParams.avgPhaseShuffle = not tofConfig.depthParams.avgPhaseShuffle
            print("ToF toggling avgPhaseShuffle value to:", tofConfig.depthParams.avgPhaseShuffle)
            tofCfgQueue.send(tofConfig)
        elif key == ord('t'):
            print("Autofocus trigger (and disable continuous)")
            ctrl = dai.CameraControl()
            ctrl.setAutoFocusMode(dai.CameraControl.AutoFocusMode.AUTO)
            ctrl.setAutoFocusTrigger()
            controlQueue.send(ctrl)
        elif key == ord('f'):
            print("Autofocus enable, continuous")
            ctrl = dai.CameraControl()
            ctrl.setAutoFocusMode(
                dai.CameraControl.AutoFocusMode.CONTINUOUS_VIDEO)
            controlQueue.send(ctrl)
        elif key == ord('e'):
            print("Autoexposure enable")
            ctrl = dai.CameraControl()
            ctrl.setAutoExposureEnable()
            controlQueue.send(ctrl)
        elif key in [ord(','), ord('.')]:
            if key == ord(','):
                lensPos -= LENS_STEP
            if key == ord('.'):
                lensPos += LENS_STEP
            lensPos = clamp(lensPos, lensMin, lensMax)
            print("Setting manual focus, lens position: ", lensPos)
            ctrl = dai.CameraControl()
            ctrl.setManualFocusRaw(lensPos)
            controlQueue.send(ctrl)
        elif key in [ord('i'), ord('o'), ord('k'), ord('l')]:
            if key == ord('i'):
                expTime -= EXP_STEP
            if key == ord('o'):
                expTime += EXP_STEP
            if key == ord('k'):
                sensIso -= ISO_STEP
            if key == ord('l'):
                sensIso += ISO_STEP
            expTime = clamp(expTime, expMin, expMax)
            sensIso = clamp(sensIso, sensMin, sensMax)
            print("Setting manual exposure, time: ", expTime, "iso: ", sensIso)
            ctrl = dai.CameraControl()
            ctrl.setManualExposure(expTime, sensIso)
            controlQueue.send(ctrl)
        elif key == ord('1'):
            awb_lock = not awb_lock
            print("Auto white balance lock:", awb_lock)
            ctrl = dai.CameraControl()
            ctrl.setAutoWhiteBalanceLock(awb_lock)
            controlQueue.send(ctrl)
        elif key == ord('2'):
            ae_lock = not ae_lock
            print("Auto exposure lock:", ae_lock)
            ctrl = dai.CameraControl()
            ctrl.setAutoExposureLock(ae_lock)
            controlQueue.send(ctrl)
        elif key == ord('a'):
            dotIntensity = dotIntensity - DOT_STEP
            if dotIntensity < 0:
                dotIntensity = 0
            device.setIrLaserDotProjectorIntensity(dotIntensity)
            print(f'IR Dot intensity:', dotIntensity)
        elif key == ord('d'):
            dotIntensity = dotIntensity + DOT_STEP
            if dotIntensity > DOT_MAX:
                dotIntensity = DOT_MAX
            device.setIrLaserDotProjectorIntensity(dotIntensity)
            print(f'IR Dot intensity:', dotIntensity)
        elif key == ord('w'):
            floodIntensity = floodIntensity + FLOOD_STEP
            if floodIntensity > FLOOD_MAX:
                floodIntensity = FLOOD_MAX
            device.setIrFloodLightIntensity(floodIntensity)
            print(f'IR Flood intensity:', floodIntensity)
        elif key == ord('s'):
            floodIntensity = floodIntensity - FLOOD_STEP
            if floodIntensity < 0:
                floodIntensity = 0
            device.setIrFloodLightIntensity(floodIntensity)
            print(f'IR Flood intensity:', floodIntensity)
        elif key >= 0 and chr(key) in '34567890[]p\\;\'':
            if key == ord('3'):
                control = 'awb_mode'
            elif key == ord('4'):
                control = 'ae_comp'
            elif key == ord('5'):
                control = 'anti_banding_mode'
            elif key == ord('6'):
                control = 'effect_mode'
            elif key == ord('7'):
                control = 'brightness'
            elif key == ord('8'):
                control = 'contrast'
            elif key == ord('9'):
                control = 'saturation'
            elif key == ord('0'):
                control = 'sharpness'
            elif key == ord('\\'):
                control = 'scene_mode'
            elif key == ord(';'):
                control = 'control_mode'
            elif key == ord('\''):
                control = 'capture_intent'
            elif key == ord('['):
                control = 'luma_denoise'
            elif key == ord(']'):
                control = 'chroma_denoise'
            elif key == ord('p'):
                control = 'tof_amplitude_min'
            print("Selected control:", control)
        elif key in [ord('-'), ord('_'), ord('+'), ord('=')]:
            change = 0
            if key in [ord('-'), ord('_')]:
                change = -1
            if key in [ord('+'), ord('=')]:
                change = 1
            ctrl = dai.CameraControl()
            if control == 'none':
                print("Please select a control first using keys 3..9 0 [ ] \\ ; \'")
            elif control == 'ae_comp':
                ae_comp = clamp(ae_comp + change, -9, 9)
                print("Auto exposure compensation:", ae_comp)
                ctrl.setAutoExposureCompensation(ae_comp)
            elif control == 'anti_banding_mode':
                abm = anti_banding_mode.step(change)
                print("Anti-banding mode:", abm)
                ctrl.setAntiBandingMode(abm)
            elif control == 'awb_mode':
                awb = awb_mode.step(change)
                print("Auto white balance mode:", awb)
                ctrl.setAutoWhiteBalanceMode(awb)
            elif control == 'effect_mode':
                eff = effect_mode.step(change)
                print("Effect mode:", eff)
                ctrl.setEffectMode(eff)
            elif control == 'scene_mode':
                sc = scene_mode.step(change)
                print("Scene mode:", sc)
                ctrl.setSceneMode(sc)
            elif control == 'control_mode':
                cm = control_mode.step(change)
                print("Control mode:", cm)
                ctrl.setControlMode(cm)
            elif control == 'capture_intent':
                ci = capture_intent.step(change)
                print("Capture intent:", ci)
                ctrl.setCaptureIntent(ci)
            elif control == 'brightness':
                brightness = clamp(brightness + change, -10, 10)
                print("Brightness:", brightness)
                ctrl.setBrightness(brightness)
            elif control == 'contrast':
                contrast = clamp(contrast + change, -10, 10)
                print("Contrast:", contrast)
                ctrl.setContrast(contrast)
            elif control == 'saturation':
                saturation = clamp(saturation + change, -10, 10)
                print("Saturation:", saturation)
                ctrl.setSaturation(saturation)
            elif control == 'sharpness':
                sharpness = clamp(sharpness + change, 0, 4)
                print("Sharpness:", sharpness)
                ctrl.setSharpness(sharpness)
            elif control == 'luma_denoise':
                luma_denoise = clamp(luma_denoise + change, 0, 4)
                print("Luma denoise:", luma_denoise)
                ctrl.setLumaDenoise(luma_denoise)
            elif control == 'chroma_denoise':
                chroma_denoise = clamp(chroma_denoise + change, 0, 4)
                print("Chroma denoise:", chroma_denoise)
                ctrl.setChromaDenoise(chroma_denoise)
            elif control == 'tof_amplitude_min' and tof:
                amp_min = clamp(tofConfig.depthParams.minimumAmplitude + change, 0, 50)
                print("Setting min amplitude(confidence) to:", amp_min)
                tofConfig.depthParams.minimumAmplitude = amp_min
                tofCfgQueue.send(tofConfig)
            controlQueue.send(ctrl)

    print()
