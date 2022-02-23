from datetime import timedelta
import depthai as dai
import tempfile
import PySimpleGUI as sg


def check_ip(s: str, window):
    spl = s.split(".")
    if len(spl) != 4:
        sg.Popup("Wrong IP format.\nValue should be similar to 255.255.255.255")
        return False
    for num in spl:
        if 255 < int(num):
            sg.Popup("Wrong IP format.\nValues can not be above 255!")
            return False
    return True


def check_mac(s, window):
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
    window['Flash newest version'].update(disabled=False)
    window['Flash configuration'].update(disabled=False)
    window['Factory reset'].update(disabled=False)
    # window['Reset configuration'].update(disabled=False)


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
    window['Flash newest version'].update(disabled=True)
    window['Flash configuration'].update(disabled=True)
    window['Factory reset'].update(disabled=True)
    window['Reset configuration'].update(disabled=True)

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


def getDevices(window, devices):
    listedDevices = []
    devices.clear()
    deviceInfos = dai.Device.getAllAvailableDevices()
    if not deviceInfos:
        window.Element('devices').update("No devices")
        sg.Popup("No devices found.")
    else:
        for deviceInfo in deviceInfos:
            listedDevices.append(deviceInfo.desc.name)
            devices[deviceInfo.desc.name] = deviceInfo
        sg.Popup("Found devices.")
        window.Element('devices').update("Select device", values=listedDevices)


def getConfigs(window, device, devType):
    bl = dai.DeviceBootloader(device)
    conf = bl.readConfigData()
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
    window.Element('currBoot').update(bl.getVersion())


def flashBootloader(window, device):
    bl = dai.DeviceBootloader(device, True)
    progress = lambda p: p * 100
    bl.flashBootloader(progress)
    window.Element('currBoot').update(bl.getVersion())
    sg.Popup("Flashed newest bootloader version.")


def flashConfig(values, window, device, devType, ipType):
    bl = dai.DeviceBootloader(device, True)
    conf = dai.DeviceBootloader.Config()
    if devType == "Poe":
        if not(check_ip(values['staticIp'], window) and check_ip(values['staticMask'], window)
               and check_ip(values['staticGateway'], window)):
            return
        if not check_mac(values['mac'], window):
            return
        if int(values['networkTimeout']) <= 0:
            window.Element('success').update("Values can not be negative!")
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
            window.Element('success').update("Values can not be negative!")
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
    if conf is None:
        success, error = bl.flashConfig(dai.DeviceBootloader.Config())
    if str(bl.getType()) == "Type.NETWORK":
        return "Poe"
    else:
        return "NonPoe"
