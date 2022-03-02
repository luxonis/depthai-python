#!/usr/bin/env python3

from datetime import timedelta
import depthai as dai
import tempfile
import PySimpleGUI as sg


def check_ip(s: str):
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


def usbSpeed(s):
    if s == "UNKNOWN":
        return dai.UsbSpeed.UNKNOWN
    elif s == "LOW":
        return dai.UsbSpeed.LOW
    elif s == "FULL":
        return dai.UsbSpeed.FULL
    elif s == "HIGH":
        return dai.UsbSpeed.HIGH
    elif s == "SUPER":
        return dai.UsbSpeed.SUPER
    else:
        return dai.UsbSpeed.SUPER_PLUS


def usbSpeedCorrection(s):
    if s == 0:
        return "UNKNOWN"
    elif s == 1:
        return "LOW"
    elif s == 2:
        return "FULL"
    elif s == 3:
        return "HIGH"
    elif s == 4:
        return "SUPER"
    else:
        return "SUPER_PLUS"


def macCorrectFormat(s):
    ret = ""
    for x in s:
        ret += str(hex(x)).strip("0x").upper() + ":"
    return ret[:-1]


def unlockConfig(window, devType):
    if devType == "Poe":
        window['staticIp'].update(disabled=False)
        window['staticMask'].update(disabled=False)
        window['staticGateway'].update(disabled=False)
        window['dns'].update(disabled=False)
        window['dnsAlt'].update(disabled=False)
        window['networkTimeout'].update(disabled=False)
        window['mac'].update(disabled=False)
        window['staticBut'].update(disabled=False)
        window['dynamicBut'].update(disabled=False)
    else:
        window['usbTimeout'].update(disabled=False)
        window['usbSpeed'].update(disabled=False)
    window['Flash newest Bootloader'].update(disabled=False)
    window['Flash configuration'].update(disabled=False)
    window['Factory reset'].update(disabled=False)
    # window['Reset configuration'].update(disabled=False)
    window['Flash DAP'].update(disabled=False)
    window['Boot from USB'].update(disabled=False)


def lockConfig(window):
    window['staticIp'].update(disabled=True)
    window['staticMask'].update(disabled=True)
    window['staticGateway'].update(disabled=True)
    window['dns'].update(disabled=True)
    window['dnsAlt'].update(disabled=True)
    window['networkTimeout'].update(disabled=True)
    window['mac'].update(disabled=True)
    window['staticBut'].update(disabled=True)
    window['dynamicBut'].update(disabled=True)
    window['usbTimeout'].update(disabled=True)
    window['usbSpeed'].update(disabled=True)
    window['Flash newest Bootloader'].update(disabled=True)
    window['Flash configuration'].update(disabled=True)
    window['Factory reset'].update(disabled=True)
    window['Reset configuration'].update(disabled=True)
    window['Flash DAP'].update(disabled=True)
    window['Boot from USB'].update(disabled=True)

    window.Element('staticIp').update("")
    window.Element('staticMask').update("")
    window.Element('staticGateway').update("")
    window.Element('dns').update("")
    window.Element('dnsAlt').update("")
    window.Element('networkTimeout').update("")
    window.Element('mac').update("")
    window.Element('usbTimeout').update("")
    window.Element('usbSpeed').update("")
    window.Element('devName').update("-name-")
    window.Element('newBoot').update("-version-")
    window.Element('currBoot').update("-version-")
    window.Element('version').update("-version-")
    window.Element('commit').update("-commit-")
    window.Element('devState').update("-state-")


def getDevices(window, devices):
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
        # TODO - this action drags down user on correct path, no need for it
        # sg.Popup("Found devices.")
        window.Element('devices').update("Select device", values=listedDevices)


def getConfigs(window, device, devType):
    bl = dai.DeviceBootloader(device)
    # TODO - might be better to readConfig instead of readConfigData
    conf = bl.readConfigData()
    if conf is not None:
        if devType == "Poe":
            window.Element('staticIp').update(conf['network']['ipv4'] if conf['network']['ipv4'] != 0 else "0.0.0.0")
            window.Element('staticMask').update(conf['network']['ipv4Mask'] if conf['network']['ipv4Mask'] != 0
                                                else "0.0.0.0")
            window.Element('staticGateway').update(conf['network']['ipv4Gateway'] if conf['network']['ipv4Gateway'] != 0
                                                else "0.0.0.0")
            window.Element('dns').update(conf['network']['ipv4Dns'] if conf['network']['ipv4Dns'] != 0 else "0.0.0.0")
            window.Element('dnsAlt').update(conf['network']['ipv4DnsAlt'] if conf['network']['ipv4DnsAlt'] != 0
                                            else "0.0.0.0")
            window.Element('networkTimeout').update(conf['network']['timeoutMs'])
            window.Element('mac').update(macCorrectFormat(conf['network']['mac']))
            window.Element('usbTimeout').update("")
            window.Element('usbSpeed').update("")
        else:
            window.Element('staticIp').update("")
            window.Element('staticMask').update("")
            window.Element('staticGateway').update("")
            window.Element('dns').update("")
            window.Element('dnsAlt').update("")
            window.Element('networkTimeout').update("")
            window.Element('mac').update("")
            window.Element('usbTimeout').update(conf['usb']['timeoutMs'])
            window.Element('usbSpeed').update(usbSpeedCorrection(conf['usb']['maxUsbSpeed']))
    window.Element('devName').update(device.desc.name)
    window.Element('newBoot').update(dai.DeviceBootloader.getEmbeddedBootloaderVersion())
    if bl.isEmbeddedVersion() is True:
        window.Element('currBoot').update('None')
    else:
        window.Element('currBoot').update(bl.getVersion())
    window.Element('version').update(dai.__version__)
    window.Element('commit').update(dai.__commit__)
    window.Element('devState').update(str(devices[device.desc.name].state).split(".")[1])


def flashBootloader(window, device):
    bl = dai.DeviceBootloader(device, True)
    progress = lambda p: p * 100
    bl.flashBootloader(progress)
    window.Element('currBoot').update(bl.getVersion())
    sg.Popup("Flashed newest bootloader version.")


def flashConfig(values, device, devType, ipType):
    bl = dai.DeviceBootloader(device, True)
    conf = dai.DeviceBootloader.Config()
    if devType == "Poe":
        if not(check_ip(values['staticIp']) and check_ip(values['staticMask'])
               and check_ip(values['staticGateway'])):
            return
        if not check_mac(values['mac']):
            return
        if int(values['networkTimeout']) <= 0:
            sg.Popup("Values can not be negative!")
            return
        if ipType:
            conf.setStaticIPv4(values['staticIp'], values['staticIp'], values['staticIp'])
        else:
            conf.setDynamicIPv4(values['staticIp'], values['staticIp'], values['staticIp'])
        conf.setDnsIPv4(values['dns'], "")
        conf.setNetworkTimeout(timedelta(seconds=int(values['networkTimeout']) / 1000))
        conf.setMacAddress(values['mac'])
    else:
        if int(values['usbTimeout']) <= 0:
            sg.Popup("Values can not be negative!")
            return
        conf.setUsbTimeout(timedelta(seconds=int(values['usbTimeout']) / 1000))
        conf.setUsbMaxSpeed(usbSpeed(values['usbSpeed']))
    success, error = bl.flashConfig(conf)
    if not success:
        sg.Popup(f"Flashing failed: {error}")
    else:
        sg.Popup("Flashing successful.")


def factoryReset(device):
    blBinary = dai.DeviceBootloader.getEmbeddedBootloaderBinary(dai.DeviceBootloader.Type.NETWORK)
    blBinary = blBinary + ([0xFF] * ((8 * 1024 * 1024 + 512) - len(blBinary)))
    bl = dai.DeviceBootloader(device, True)
    tmpBlFw = tempfile.NamedTemporaryFile(delete=False)
    tmpBlFw.write(bytes(blBinary))
    progress = lambda p: p * 100
    success, msg = bl.flashBootloader(progress, tmpBlFw.name)
    if success:
        sg.Popup("Factory reset was successful.")
    else:
       sg.Popup(f"Factory reset failed. Error: {msg}")
    tmpBlFw.close()


def getDeviceType(device):
    bl = dai.DeviceBootloader(device)
    conf = bl.readConfigData()
    # TODO - Don't modify the device without explicit action
    # if conf is None:
    #     success, error = bl.flashConfig(dai.DeviceBootloader.Config())
    if str(bl.getType()) == "Type.NETWORK":
        return "Poe"
    else:
        return "NonPoe"


def flashFromFile(file, device):
    bl = dai.DeviceBootloader(device)
    if str(file)[-3:] == "dap":
        bl.flashDepthaiApplicationPackage(file)
    else:
        sg.Popup("Selected file is not .dap!")


def flashFromUsb(device):
    bl = dai.DeviceBootloader(device)
    bl.bootUsbRomBootloader()


usbSpeeds = ["UNKNOWN", "LOW", "FULL", "HIGH", "SUPER", "SUPER_PLUS"]

sg.theme('DarkGrey4')

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
        sg.Combo([], "Search for devices", size=(30, 5), key="devices", enable_events=True),
        sg.Button("Select"), sg.Button("Search")
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
        sg.Text("Current bootloader version:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black")
    ],
    [
        sg.Text("-version-", key="newBoot", size=(30, 1)),
        sg.VSeparator(),
        sg.Text("-version-", key="currBoot", size=(30, 1))
    ],
    [
        sg.Text("Current __version__:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
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
        sg.Button("Flash newest Bootloader", size=(17, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00'),
        sg.Button("Factory reset",  size=(17, 2), font=('Arial', 10, 'bold'), disabled=True, button_color='#FFEA00'),
        sg.Button("Boot from USB", size=(17, 2), font=('Arial', 10, 'bold'), disabled=True, button_color='#FFEA00')
    ]
]

# layout for config tab
deviceConfigLayout = [
    [sg.Text("Configuration settings", size=(20, 1), font=('Arial', 30, 'bold'), text_color="black")],
    [sg.HSeparator()],
    [
        sg.Button("Device select", size=(15, 1), font=('Arial', 10, 'bold'), disabled=False, key="aboutReal"),
        sg.Button("Config", size=(15, 1), font=('Arial', 10, 'bold'), disabled=True, key="configFake")
    ],
    [sg.HSeparator()],
    [
        sg.Text("IPv4 type:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.Radio('Static', "ipType", default=True, font=('Arial', 10, 'bold'), text_color="black",
                 key="staticBut", enable_events=True, disabled=True),
        sg.Radio('Dynamic', "ipType", default=False, font=('Arial', 10, 'bold'), text_color="black",
                 key="dynamicBut", enable_events=True, disabled=True)
    ],
    [
        sg.Text("IPv4:", size=(12, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="staticIp", size=(16, 2), disabled=True),
        sg.Text("Mask:", size=(5, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="staticMask", size=(16, 2), disabled=True),
        sg.Text("Gateway:", size=(8, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="staticGateway", size=(16, 2), disabled=True)
    ],
    [
        sg.Text("DNS name:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="dns", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("Alt DNS name:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="dnsAlt", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("USB timeout:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="usbTimeout", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("Network timeout:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="networkTimeout", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("MAC address:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.InputText(key="mac", size=(30, 2), disabled=True)
    ],
    [
        sg.Text("USB max speed:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.Combo(usbSpeeds, "Select speed", key="usbSpeed", size=(30, 6), disabled=True)
    ],
    [sg.HSeparator()],
    [
        sg.Text("", size=(10, 2)),
        sg.Button("Flash configuration", size=(15, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00'),
        sg.Button("Clear flash", size=(15, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00'),
        sg.Button("Flash DAP", size=(15, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00')
    ],
]

# layout of whole GUI with closed tabs set to false
layout = [
    [
        sg.Column(aboutDeviceLayout, key='-COL1-'),
        sg.Column(deviceConfigLayout, visible=False, key='-COL2-'),
    ]
]

devices = dict()
devType = ""
window = sg.Window(title="Device Manager", layout=layout, size=(620, 370))

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED:
        break
    dev = values['devices']
    if event == "Select":
        if dev != "Select device":
            devType = getDeviceType(devices[values['devices']])
            getConfigs(window, devices[values['devices']], devType)
            unlockConfig(window, devType)
        else:
            window.Element('progress').update("No device selected.")
    if event == "Search":
        getDevices(window, devices)
    if event == "Flash newest Bootloader":
        flashBootloader(window, devices[values['devices']])
    if event == "Flash configuration":
        flashConfig(values, devices[values['devices']], devType, values['StaticBut'])
        getConfigs(window, devices[values['devices']], devType)
        lockConfig(window)
        if devType != "Poe":
            unlockConfig(window, devType)
        else:
            devices.clear()
            window.Element('devices').update("Search for devices", values=[])
    if event == "Factory reset":
        factoryReset(devices[values['devices']])
    if event == "Flash DAP":
        file = sg.popup_get_file("Select .dap file")
        flashFromFile(file, devices[values['devices']])
    if event == "configReal":
        window['-COL1-'].update(visible=False)
        window['-COL2-'].update(visible=True)
    if event == "aboutReal":
        window['-COL2-'].update(visible=False)
        window['-COL1-'].update(visible=True)
    if event == "Boot from USB":
        flashFromUsb(devices[values['devices']])
window.close()
