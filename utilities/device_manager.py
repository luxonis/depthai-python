#!/usr/bin/env python3

from datetime import timedelta
import depthai as dai
import tempfile
import PySimpleGUI as sg
import sys

CONF_TEXT_POE = ['ipTypeText', 'ipText', 'maskText', 'gatewayText', 'dnsText', 'dnsAltText', 'networkTimeoutText', 'macText']
CONF_INPUT_POE = ['staticBut', 'dynamicBut', 'ip', 'mask', 'gateway', 'dns', 'dnsAlt', 'networkTimeout', 'mac']
CONF_TEXT_USB = ['usbTimeoutText', 'usbSpeedText']
CONF_INPUT_USB = ['usbTimeout', 'usbSpeed']
USB_SPEEDS = ["UNKNOWN", "LOW", "FULL", "HIGH", "SUPER", "SUPER_PLUS"]

devices = dict()

def PrintException():
    exc_type, exc_obj, tb = sys.exc_info()
    f = tb.tb_frame
    lineno = tb.tb_lineno
    filename = f.f_code.co_filename
    print('Exception in {}, line {}; {}'.format(filename, lineno, exc_obj))

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

class SelectIP:
    def __init__(self):
        self.ok = False
        layout = [
            [sg.Text("Specify the custom IP of the OAK PoE\ncamera you want to connect to")],
            [
                sg.Text("IPv4:", font=('Arial', 10, 'bold'), text_color="black"),
                sg.InputText(key="ip", size=(16, 2)),
            ],
            [sg.Submit(), sg.Cancel()],
        ]
        self.window = sg.Window("Specify IP", layout, size=(300,110), modal=True, finalize=True)
    def wait(self):
        event, values = self.window.Read()
        self.window.close()
        if str(event) == "Cancel" or values is None or not check_ip(values["ip"]):
            return False, ""
        return True, values["ip"]

class SearchDevice:
    def __init__(self):
        self.infos = []
        layout = [
            [sg.Text("Select an OAK camera you would like to connect to.", font=('Arial', 10, 'bold'))],
            [sg.Table(values=[], headings=["MxId", "Name", "State"],
                # col_widths=[25, 17, 17],
                # def_col_width=25,
                # max_col_width=200,
                # background_color='light blue',
                display_row_numbers=True,
                justification='right',
                num_rows=8,
                alternating_row_color='lightyellow',
                key='table',
                row_height=35,
                # size=(500, 300)
                expand_x = True,
                enable_events = True,
                enable_click_events = True,
                )
            ],
            [sg.Button('Search', size=(15, 2), font=('Arial', 10, 'bold'))],
        ]
        self.window = sg.Window("Select Device", layout, size=(550,375), modal=True, finalize=True)
        self.search_devices()

    def search_devices(self):
        self.infos = dai.XLinkConnection.getAllConnectedDevices()
        if not self.infos:
            sg.Popup("No devices found.")
        else:
            rows = []
            for info in self.infos:
                rows.append([info.getMxId(), info.name, deviceStateTxt(info.state)])
            self.window['table'].update(values=rows)

    def wait(self) -> dai.DeviceInfo:
        while True:
            event, values = self.window.Read()
            if event is None:
                self.window.close()
                return None
            elif str(event) == 'Search':
                self.search_devices()
            elif len(event) == 3 and event[0] == "table" and event[1] == "+CLICKED+":
                # User selected a device
                deviceIndex = event[2][0]
                if deviceIndex is not None:
                    deviceSelected = self.infos[deviceIndex]
                    self.window.close()
                    return deviceSelected

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

def getDevices(window):
    try:
        listedDevices = []
        devices.clear()
        deviceInfos = dai.XLinkConnection.getAllConnectedDevices()
        if not deviceInfos:
            window.Element('devices').update("No devices")
            sg.Popup("No devices found.")
        else:
            for deviceInfo in deviceInfos:
                deviceTxt = deviceInfo.getMxId()
                listedDevices.append(deviceTxt)
                devices[deviceTxt] = deviceInfo
            window.Element('devices').update("Select device", values=listedDevices)
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def getConfigs(window, bl: dai.DeviceBootloader, devType, device: dai.DeviceInfo):
    try:
        conf = bl.readConfig()
    except:
        conf = dai.DeviceBootloader.Config()

    try:
        if devType == "POE":
            window.Element('ip').update(conf.getIPv4())
            window.Element('mask').update(conf.getIPv4Mask())
            window.Element('gateway').update(conf.getIPv4Gateway())
            window.Element('dns').update(conf.getDnsIPv4())
            window.Element('dnsAlt').update(conf.getDnsAltIPv4())
            window.Element('networkTimeout').update(int(conf.getNetworkTimeout().total_seconds() * 1000))
            window.Element('mac').update(conf.getMacAddress())
            for el in CONF_INPUT_USB:
                window[el].update("")
        else:
            for el in CONF_INPUT_POE:
                window[el].update("")
            window.Element('usbTimeout').update(int(conf.getUsbTimeout().total_seconds() * 1000))
            window.Element('usbSpeed').update(str(conf.getUsbMaxSpeed()).split('.')[1])

        window.Element('devName').update(device.name)
        window.Element('devNameConf').update(device.getMxId())
        window.Element('newBoot').update(dai.DeviceBootloader.getEmbeddedBootloaderVersion())

        # The "isEmbeddedVersion" tells you whether BL had to be booted,
        # or we connected to an already flashed Bootloader.
        if bl.isEmbeddedVersion():
            window.Element('currBoot').update('Not Flashed')
        else:
            window.Element('currBoot').update(bl.getVersion())

        window.Element('version').update(dai.__version__)
        window.Element('commit').update(dai.__commit__)
        window.Element('devState').update(deviceStateTxt(device.state))
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def flashBootloader(window, device: dai.DeviceInfo):
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
        if type == dai.DeviceBootloader.Type.AUTO:
            type = bl.getType()
        bl.flashBootloader(memory=dai.DeviceBootloader.Memory.FLASH, type=type, progressCallback=progress)
        window.Element('currBoot').update(bl.getVersion())
        pr.finish("Flashed newest bootloader version.")
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def flashConfig(values, device: dai.DeviceInfo, devType: str, staticIp: bool):
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
        PrintException()
        sg.Popup(f'{ex}')

def factoryReset(device: dai.DeviceInfo):
    sel = SelectBootloader(['USB', 'NETWORK'], "Select bootloader type used for factory reset.")
    ok, type = sel.wait()
    if not ok:
        print("Factory reset canceled.")
        return

    try:
        blBinary = dai.DeviceBootloader.getEmbeddedBootloaderBinary(type)
        # Clear 1 MiB for USB BL and 8 MiB for NETWORK BL
        mib = 1 if type == dai.DeviceBootloader.Type.USB else 8
        blBinary = blBinary + ([0xFF] * ((mib * 1024 * 1024 + 512) - len(blBinary)))
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
        PrintException()
        sg.Popup(f'{ex}')

def getDeviceType(bl: dai.DeviceBootloader) -> str:
    try:
        if bl.getType() == dai.DeviceBootloader.Type.NETWORK:
            return "POE"
        else:
            return "USB"
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def flashFromFile(file, device: dai.DeviceInfo):
    try:
        bl = dai.DeviceBootloader(device, True)
        if str(file)[-3:] == "dap":
            bl.flashDepthaiApplicationPackage(file)
        else:
            sg.Popup("Selected file is not .dap!")
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def flashFromUsb(device: dai.DeviceInfo):
    try:
        bl = dai.DeviceBootloader(device, True)
        bl.bootUsbRomBootloader()
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def connectToDevice(device: dai.DeviceInfo) -> dai.DeviceBootloader:
    try:
        bl = dai.DeviceBootloader(device, False)
        return bl
    except Exception as ex:
        PrintException()
        sg.Popup(f'{ex}')

def deviceStateTxt(state: dai.XLinkDeviceState) -> str:
    return str(state).replace("XLinkDeviceState.X_LINK_", "")

# Start defying layout

sg.theme('LightGrey2')

# layout for device tab
aboutDeviceLayout = [
    [sg.Text("About device", size=(30, 1), font=('Arial', 30, 'bold'), text_color="black")],
    [sg.HSeparator()],
    [
        sg.Button("About device", size=(15, 1), font=('Arial', 10, 'bold'), disabled=True,  key="aboutFake"),
        sg.Button("Config", size=(15, 1), font=('Arial', 10, 'bold'), disabled=False,  key="configReal")
    ],
    [sg.HSeparator()],
    [
        sg.Text("Select device: ", size=(15, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.Combo([], "Select device", size=(30, 5), key="devices", enable_events=True),
        sg.Button("Search", font=('Arial', 10, 'bold')),
        sg.Button("Specify IP")
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
        sg.Button("About device", size=(15, 1), font=('Arial', 10, 'bold'), disabled=False, key="aboutReal"),
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

window = sg.Window(title="Device Manager",
    icon="assets/icon.png",
    layout=layout,
    size=(645, 380),
    finalize=True # So we can do First search for devices
    )

# First device search
getDevices(window)

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
            device = devices[values['devices']]
            if deviceStateTxt(device.state) == "BOOTED":
                # device is already booted somewhere else
                sg.Popup("Device is already booted somewhere else!")
            else:
                bl = connectToDevice(device)
                devType = getDeviceType(bl)
                getConfigs(window, bl, devType, device)
                unlockConfig(window, devType)
        else:
            window.Element('progress').update("No device selected.")
    elif event == "Search":
        getDevices(window) # Re-search devices for dropdown
        lockConfig(window)
        selDev = SearchDevice()
        di = selDev.wait()
        if di is not None:
            window.Element('devices').update(di.getMxId())
            bl = connectToDevice(di)
            devType = getDeviceType(bl)
            getConfigs(window, bl, devType, di)
            unlockConfig(window, devType)
    elif event == "Specify IP":
        select = SelectIP()
        ok, ip = select.wait()
        if ok:
            di = dai.DeviceInfo(ip)
            di.state = dai.XLinkDeviceState.X_LINK_BOOTLOADER
            di.protocol = dai.XLinkProtocol.X_LINK_TCP_IP
            devices[ip] = di # Add to devices dict
            window.Element('devices').update(ip) # Show to user
            bl = connectToDevice(di)
            devType = getDeviceType(bl)
            getConfigs(window, bl, devType, di)
            unlockConfig(window, devType)
    elif event == "Flash newest Bootloader":
        bl.close()
        flashBootloader(window, devices[values['devices']])
    elif event == "Flash configuration":
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
    elif event == "Factory reset":
        bl.close()
        factoryReset(devices[values['devices']])
    elif event == "Flash DAP":
        file = sg.popup_get_file("Select .dap file", file_types=(('DepthAI Application Package', '*.dap'), ('All Files', '*.* *')))
        bl = None
        flashFromFile(file, devices[values['devices']])
    elif event == "configReal":
        window['-COL1-'].update(visible=False)
        window['-COL2-'].update(visible=True)
    elif event == "aboutReal":
        window['-COL2-'].update(visible=False)
        window['-COL1-'].update(visible=True)
    elif event == "recoveryMode":
        bl = None
        flashFromUsb(devices[values['devices']])
window.close()
