#!/usr/bin/env python3

from guiFunctions import *


usbSpeeds = ["UNKNOWN", "LOW", "FULL", "HIGH", "SUPER", "SUPER_PLUS"]

sg.theme('DarkGrey4')

aboutDeviceLayout = [
    [sg.Text("About device", size=(20, 1), font=('Arial', 30, 'bold'), text_color="black")],
    [sg.HSeparator()],
    [
        sg.Text("Select device: ", size=(15, 1), font=('Arial', 10, 'bold'), text_color="black"),
        sg.Combo([], "Search for devices", size=(30, 5), key="devices", enable_events=True),
        sg.Button("Select"), sg.Button("Search")
    ],
    [sg.Text("Name of connected device:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black")],
    [sg.Text("-name-", key="devName", size=(30, 1))],
    [sg.Text("Version of newest bootloader:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black")],
    [sg.Text("-version-", key="newBoot", size=(20, 1))],
    [sg.Text("Current bootloader version:", size=(30, 1), font=('Arial', 10, 'bold'), text_color="black")],
    [sg.Text("-version-", key="currBoot", size=(20, 1))],
    [sg.HSeparator()],
    [
        sg.Text("", size=(5, 2)),
        sg.Button("Flash newest version", size=(20, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00'),
        sg.Button("Factory reset",  size=(20, 2), font=('Arial', 10, 'bold'), disabled=True, button_color='#FFEA00')]
]

deviceConfigLayout = [
    [sg.Text("Configuration settings", size=(20, 1), font=('Arial', 30, 'bold'), text_color="black")],
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
        sg.Text("", size=(20, 2)),
        sg.Button("Flash configuration", size=(20, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00'),
        sg.Button("Clear flash", size=(20, 2), font=('Arial', 10, 'bold'), disabled=True,
                  button_color='#FFEA00')
    ]
]

layout = [
    [sg.VSeparator(),
     sg.Column(aboutDeviceLayout),
     sg.VSeparator(),
     sg.Column(deviceConfigLayout),
     sg.VSeparator()]
]

devices = dict()
devType = ""
window = sg.Window(title="Bootloader GUI", layout=layout)

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
    if event == "Flash newest version":
        flashBootloader(window, devices[values['devices']])
    if event == "Flash configuration":
        flashConfig(values, window, devices[values['devices']], devType, values['StaticBut'])
        getConfigs(window, devices[values['devices']], devType)
        lockConfig(window)
        if devType != "Poe":
            unlockConfig(window, devType)
        else:
            devices.clear()
            window.Element('devices').update("Search for devices", values=[])
    if event == "Factory reset":
        factoryReset(devices[values['devices']])
window.close()
