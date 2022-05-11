#!/usr/bin/env python3

from datetime import timedelta
import depthai as dai
import tempfile
import PySimpleGUI as sg


CONF_TEXT_POE = ['ipTypeText', 'ipText', 'maskText', 'gatewayText', 'dnsText', 'dnsAltText', 'networkTimeoutText', 'macText']
CONF_INPUT_POE = ['staticBut', 'dynamicBut', 'ip', 'mask', 'gateway', 'dns', 'dnsAlt', 'networkTimeout', 'mac']
CONF_TEXT_USB = ['usbTimeoutText', 'usbSpeedText']
CONF_INPUT_USB = ['usbTimeout', 'usbSpeed']
USB_SPEEDS = ["UNKNOWN", "LOW", "FULL", "HIGH", "SUPER", "SUPER_PLUS"]

def check_ip(s: str):
    if s == "":
        return False
    spl = s.split(".")
    if len(spl) != 4:
        sg.Popup("Wrong IP format.\nValue should be similar to 255.255.255.255")
        return False
    for num in spl:
        if 255 < int(num):
            sg.Popup("Wrong IP format.\nValues can not be above 255!")
            return False
    return True

def check_mac(s):
    if s.count(":") != 5:
        sg.Popup("Wrong MAC format.\nValue should be similar to FF:FF:FF:FF:FF:FF")
        return False
    for i in s.split(":"):
        for j in i:
            if j > "F" or (j < "A" and not j.isdigit()) or len(i) != 2:
                sg.Popup("Wrong MAC format.\nValue should be similar to FF:FF:FF:FF:FF:FF")
                return False
    return True

class Progress:
    def __init__(self):
        layout = [
            [sg.Text('Flashing progress: 0.0%', key='txt')],
            [sg.ProgressBar(1.0, orientation='h', size=(20,20), key='progress')],
        ]
        self.window = sg.Window("Progress", layout, modal=True, finalize=True)
    def update(self, val):
        self.window['progress'].update(val)
        self.window['txt'].update(f'Flashing progress: {val*100:.1f}%')
    def finish(self, msg):
        self.window.close()
        sg.Popup(msg)

class SelectBootloader:
    def __init__(self, options, text):
        self.ok = False
        layout = [
            [sg.Text(text)],
            [
                sg.Text("Bootloader type:", font=('Arial', 10, 'bold'), text_color="black"),
                sg.Combo(options, options[0], size=(30, 3), key="bootType"),
            ],
            [sg.Text("Warning! This may soft-brick your device,\nproceed if you are sure what you are doing!")],
            [sg.Submit(), sg.Cancel()],
        ]
        self.window = sg.Window("Select bootloader", layout, size=(300,150), modal=True, finalize=True)
    def wait(self):
        event, values = self.window.Read()
        self.window.close()
        type = getattr(dai.DeviceBootloader.Type, values['bootType'])
        return (str(event) == "Submit", type)

def unlockConfig(window, devType):
    if devType == "POE":
        for el in CONF_INPUT_POE:
            window[el].update(disabled=False)
        for el in CONF_TEXT_POE:
            window[el].update(text_color="black")
    else:
        for el in CONF_INPUT_USB:
            window[el].update(disabled=False)
        for el in CONF_TEXT_USB:
            window[el].update(text_color="black")

    window['Flash newest Bootloader'].update(disabled=False)
    window['Flash configuration'].update(disabled=False)
    window['Factory reset'].update(disabled=False)
    # window['Clear flash'].update(disabled=False)
    window['Flash DAP'].update(disabled=False)
    window['recoveryMode'].update(disabled=False)


def lockConfig(window):
    for conf in [CONF_INPUT_POE, CONF_INPUT_USB]:
        for el in conf:
            window[el].update(disabled=True)
            window[el].update("")
    for conf in [CONF_TEXT_POE, CONF_TEXT_USB]:
        for el in conf:
            window[el].update(text_color="gray")

    window['Flash newest Bootloader'].update(disabled=True)
    window['Flash configuration'].update(disabled=True)
    window['Factory reset'].update(disabled=True)
    window['Clear flash'].update(disabled=True)
    window['Flash DAP'].update(disabled=True)
    window['recoveryMode'].update(disabled=True)

    window.Element('devName').update("-name-")
    window.Element('devNameConf').update("")
    window.Element('newBoot').update("-version-")
    window.Element('currBoot').update("-version-")
    window.Element('version').update("-version-")
    window.Element('commit').update("-version-")
    window.Element('devState').update("-state-")


def getDevices(window, devices):
    try:
        listedDevices = []
        devices.clear()
        deviceInfos = dai.XLinkConnection.getAllConnectedDevices()
        if not deviceInfos:
            window.Element('devices').update("No devices")
            sg.Popup("No devices found.")
        else:
            for deviceInfo in deviceInfos:
                # print(deviceInfo.state)
                listedDevices.append(deviceInfo.desc.name)
                devices[deviceInfo.desc.name] = deviceInfo
            window.Element('devices').update("Select device", values=listedDevices)
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')


def getConfigs(window, bl, devType, device):
    try:
        conf = bl.readConfig()
        if conf is not None:
            if devType == "POE":
                window.Element('ip').update(conf.getIPv4() if conf.getIPv4() is not None else "0.0.0.0")
                window.Element('mask').update(conf.getIPv4Mask() if conf.getIPv4Mask() is not None else "0.0.0.0")
                window.Element('gateway').update(conf.getIPv4Gateway() if conf.getIPv4Gateway() is not None else "0.0.0.0")
                window.Element('dns').update(conf.getDnsIPv4() if conf.getDnsIPv4() is not None else "0.0.0.0")
                window.Element('dnsAlt').update(conf.getDnsAltIPv4() if conf.getDnsAltIPv4() is not None else "0.0.0.0")
                window.Element('networkTimeout').update(int(conf.getNetworkTimeout().total_seconds() * 1000))
                window.Element('mac').update(conf.getMacAddress())
                for el in CONF_INPUT_USB:
                    window[el].update("")
            else:
                for el in CONF_INPUT_POE:
                    window[el].update("")
                window.Element('usbTimeout').update(int(conf.getUsbTimeout().total_seconds() * 1000))
                window.Element('usbSpeed').update(str(conf.getUsbMaxSpeed()).split('.')[1])

        window.Element('devName').update(device.desc.name)
        window.Element('devNameConf').update(device.desc.name)
        window.Element('newBoot').update(dai.DeviceBootloader.getEmbeddedBootloaderVersion())

        # The "isEmbeddedVersion" tells you whether BL had to be booted,
        # or we connected to an already flashed Bootloader.
        if bl.isEmbeddedVersion():
            window.Element('currBoot').update('Not Flashed')
        else:
            window.Element('currBoot').update(bl.getVersion())

        window.Element('version').update(dai.__version__)
        window.Element('commit').update(dai.__commit__)
        window.Element('devState').update(str(devices[device.desc.name].state).split(".")[1])
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')


def flashBootloader(window, device):
    # FIXME - to flash bootloader, boot the same device again (from saved device info) but with allowFlashingBootloader = True
    try:
        sel = SelectBootloader(['AUTO', 'USB', 'NETWORK'], "Select bootloader type to flash.")
        ok, type = sel.wait()
        if not ok:
            print("Flashing bootloader canceled.")
            return

        bl = dai.DeviceBootloader(device, True)

        pr = Progress()
        progress = lambda p : pr.update(p)
        bl.flashBootloader(memory=dai.DeviceBootloader.Memory.FLASH, type=type, progressCallback=progress)
        window.Element('currBoot').update(bl.getVersion())
        pr.finish("Flashed newest bootloader version.")
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')


def flashConfig(values, device, devType, staticIp):
    try:
        bl = dai.DeviceBootloader(device, True)
        conf = dai.DeviceBootloader.Config()
        if devType == "POE":
            if staticIp:
                if check_ip(values['ip']) and check_ip(values['mask']) and check_ip(values['gateway']):
                    conf.setStaticIPv4(values['ip'], values['mask'], values['gateway'])
            else:
                if check_ip(values['ip']) and check_ip(values['mask']) and check_ip(values['gateway']):
                    conf.setDynamicIPv4(values['ip'], values['mask'], values['gateway'])
            if values['dns'] != "" and values['dnsAlt'] != "":
                conf.setDnsIPv4(values['dns'], values['dnsAlt'])
            if values['networkTimeout'] != "":
                if int(values['networkTimeout']) >= 0:
                    conf.setNetworkTimeout(timedelta(seconds=int(values['networkTimeout']) / 1000))
                else:
                    sg.Popup("Values can not be negative!")
            if values['mac'] != "":
                if check_mac(values['mac']):
                    conf.setMacAddress(values['mac'])
        else:
            if values['usbTimeout'] != "":
                if int(values['usbTimeout']) >= 0:
                    conf.setUsbTimeout(timedelta(seconds=int(values['usbTimeout']) / 1000))
                else:
                    sg.Popup("Values can not be negative!")
            if values['usbSpeed'] != "":
                conf.setUsbMaxSpeed(getattr(dai.UsbSpeed, values['usbSpeed']))

        success, error = bl.flashConfig(conf)
        if not success:
            sg.Popup(f"Flashing failed: {error}")
        else:
            sg.Popup("Flashing successful.")
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')


def factoryReset(device):
    sel = SelectBootloader(['USB', 'NETWORK'], "Select bootloader type used for factory reset.")
    ok, type = sel.wait()
    if not ok:
        print("Factory reset canceled.")
        return

    try:
        blBinary = dai.DeviceBootloader.getEmbeddedBootloaderBinary(type)
        blBinary = blBinary + ([0xFF] * ((8 * 1024 * 1024 + 512) - len(blBinary)))
        bl = dai.DeviceBootloader(device, True)
        tmpBlFw = tempfile.NamedTemporaryFile(delete=False)
        tmpBlFw.write(bytes(blBinary))

        pr = Progress()
        progress = lambda p : pr.update(p)
        success, msg = bl.flashBootloader(progress, tmpBlFw.name)
        msg = "Factory reset was successful." if success else f"Factory reset failed. Error: {msg}"
        pr.finish(msg)
        tmpBlFw.close()
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')

def getDeviceType(bl):
    try:
        if bl.getType() == dai.DeviceBootloader.Type.NETWORK:
            return "POE"
        else:
            return "USB"
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')

def flashFromFile(file, device):
    try:
        bl = dai.DeviceBootloader(device, True)
        if str(file)[-3:] == "dap":
            bl.flashDepthaiApplicationPackage(file)
        else:
            sg.Popup("Selected file is not .dap!")
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')


def flashFromUsb(device):
    try:
        bl = dai.DeviceBootloader(device, True)
        bl.bootUsbRomBootloader()
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')


def connectToDevice(device):
    try:
        bl = dai.DeviceBootloader(device, False)
        return bl
    except Exception as ex:
        print(f'Exception: {ex}')
        sg.Popup(f'{ex}')

sg.theme('LightGrey2')

# first device search
allDevices = []
devices = dict()
tmp = "Search for devices"
deviceInfos = dai.XLinkConnection.getAllConnectedDevices()

if deviceInfos:
    tmp = "Select device"
    for deviceInfo in deviceInfos:
        allDevices.append(deviceInfo.desc.name)
        devices[deviceInfo.desc.name] = deviceInfo

# layout for device tab
aboutDeviceLayout = [
    [sg.Text("About device", size=(30, 1), font=('Arial', 30, 'bold'), text_color="black")],
    [sg.HSeparator()],
    [
        sg.Button("Device select", size=(15, 1), font=('Arial', 10, 'bold'), disabled=True,  key="aboutFake"),
        sg.Button("Config", size=(15, 1), font=('Arial', 10, 'bold'), disabled=False,  key="configReal")
    ],
    [sg.HSeparator()],
    [
        sg.Text("Select device: ", size=(15, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.Combo(allDevices, tmp, size=(30, 5), key="devices", enable_events=True),
        sg.Button("Search")
    ],
    [
        sg.Text("Name of connected device:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.VSeparator(),
        sg.Text("Device state:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black")
    ],
    [sg.Text("-name-", key="devName", size=(30, 1)), sg.VSeparator(), sg.Text("-state-", key="devState", size=(30, 1))],
    [
        sg.Text("Version of newest bootloader:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.VSeparator(),
        sg.Text("Flashed bootloader version:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black")
    ],
    [
        sg.Text("-version-", key="newBoot", size=(30, 1)),
        sg.VSeparator(),
        sg.Text("-version-", key="currBoot", size=(30, 1))
    ],
    [
        sg.Text("Current Depthai version:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.VSeparator(),
        sg.Text("Current commit:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
    ],
    [
        sg.Text("-version-", key="version", size=(30, 1)),
        sg.VSeparator(),
        sg.Text("-version-", key="commit", size=(31, 1))
    ],
    [sg.HSeparator()],
    [
        sg.Text("", size=(7, 2)),
        sg.Button("Flash newest Bootloader", size=(20, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFA500'),
        sg.Button("Factory reset",  size=(17, 2), font=('Arial', 10, 'bold'), disabled=True, button_color='#FFA500'),
        sg.Button("Boot into USB\nRecovery mode", size=(20, 2), font=('Arial', 10, 'bold'), disabled=True,
                  key='recoveryMode', button_color='#FFA500')
    ]
]

# layout for config tab
deviceConfigLayout = [
    [sg.Text("Configuration settings", size=(20, 1), font=('Arial', 30, 'bold'), text_color="black")],
    [sg.HSeparator()],
    [
        sg.Button("Device select", size=(15, 1), font=('Arial', 10, 'bold'), disabled=False, key="aboutReal"),
        sg.Button("Config", size=(15, 1), font=('Arial', 10, 'bold'), disabled=True, key="configFake"),
        # TODO create library tab
        # sg.Button("Library", size=(15, 1), font=('Arial', 10, 'bold'), disabled=True, key="configLib"),
        sg.Text("", key="devNameConf", size=(30, 1))

    ],
    [sg.HSeparator()],
    [
        sg.Text("IPv4 type:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="ipTypeText"),
        sg.Radio('Static', "ipType", default=True, font=('Arial', 10, 'bold'), text_color="black",
                 key="staticBut", enable_events=True, disabled=True),
        sg.Radio('Dynamic', "ipType", default=False, font=('Arial', 10, 'bold'), text_color="black",
                 key="dynamicBut", enable_events=True, disabled=True)
    ],
    [
        sg.Text("IPv4:", size=(12, 1), font=('Arial', 10, 'bold'), text_color="gray", key="ipText"),
        sg.InputText(key="ip", size=(16, 2), disabled=True),
        sg.Text("Mask:", size=(5, 1), font=('Arial', 10, 'bold'), text_color="gray", key="maskText"),
        sg.InputText(key="mask", size=(16, 2), disabled=True),
        sg.Text("Gateway:", size=(8, 1), font=('Arial', 10, 'bold'), text_color="gray", key="gatewayText"),
        sg.InputText(key="gateway", size=(16, 2), disabled=True)
    ],
    [
        sg.Text("DNS name:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="dnsText"),
        sg.InputText(key="dns", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("Alt DNS name:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="dnsAltText"),
        sg.InputText(key="dnsAlt", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("Network timeout [ms]:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="networkTimeoutText"),
        sg.InputText(key="networkTimeout", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("MAC address:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="macText"),
        sg.InputText(key="mac", size=(30, 2), disabled=True)
    ],
    [sg.HSeparator()],
    [
        sg.Text("USB timeout [ms]:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="usbTimeoutText"),
        sg.InputText(key="usbTimeout", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("USB max speed:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="gray", key="usbSpeedText"),
        sg.Combo(USB_SPEEDS, "Select speed", key="usbSpeed", size=(30, 6), disabled=True)
    ],
    [sg.HSeparator()],
    [
        sg.Text("", size=(10, 2)),
        sg.Button("Flash configuration", size=(15, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFA500'),
        sg.Button("Clear flash", size=(15, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFA500'),
        sg.Button("Flash DAP", size=(15, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFA500')
    ],
]

# layout of whole GUI with closed tabs set to false
layout = [
    [
        sg.Column(aboutDeviceLayout, key='-COL1-'),
        sg.Column(deviceConfigLayout, visible=False, key='-COL2-'),
    ]
]

devType = ""
bl = None
window = sg.Window(title="Device Manager", icon="assets/icon.png", layout=layout, size=(645, 380))

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED:
        break
    dev = values['devices']
    if event == "devices":
        if dev != "Select device":
            # "allow flashing bootloader" boots latest bootloader first
            # which makes the information of current bootloader, etc.. not correct (can be checked by "isEmbeddedVersion")
            # So leave it to false, uncomment the isEmbeddedVersion below and only boot into latest bootlaoder upon the request to flash new bootloader
            # bl = dai.DeviceBootloader(devices[values['devices']], False)
            if str(devices[values['devices']].state).replace("XLinkDeviceState.X_LINK_", "") == "BOOTED":
                # device is already booted somewhere else
                sg.Popup("Device is already booted somewhere else!")
            else:
                bl = connectToDevice(devices[values['devices']])
                devType = getDeviceType(bl)
                getConfigs(window, bl, devType, devices[values['devices']])
                unlockConfig(window, devType)
        else:
            window.Element('progress').update("No device selected.")
    if event == "Search":
        getDevices(window, devices)
        lockConfig(window)
    if event == "Flash newest Bootloader":
        bl.close()
        flashBootloader(window, devices[values['devices']])
    if event == "Flash configuration":
        bl.close()
        flashConfig(values, devices[values['devices']], devType, values['staticBut'])
        bl = connectToDevice(devices[values['devices']])
        getConfigs(window, bl, devType, devices[values['devices']])
        lockConfig(window)
        if devType != "POE":
            unlockConfig(window, devType)
        else:
            devices.clear()
            window.Element('devices').update("Search for devices", values=[])
    if event == "Factory reset":
        bl.close()
        factoryReset(devices[values['devices']])
    if event == "Flash DAP":
        file = sg.popup_get_file("Select .dap file", file_types=(('DepthAI Application Package', '*.dap'), ('All Files', '*.* *')))
        bl = None
        flashFromFile(file, devices[values['devices']])
    if event == "configReal":
        window['-COL1-'].update(visible=False)
        window['-COL2-'].update(visible=True)
    if event == "aboutReal":
        window['-COL2-'].update(visible=False)
        window['-COL1-'].update(visible=True)
    if event == "recoveryMode":
        bl = None
        flashFromUsb(devices[values['devices']])
window.close()
