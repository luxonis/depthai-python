.. _components_bootloader:

Bootloader
==========

DepthAI bootloader is a small program which handles the booting process, either by **booting the flashed application** (see :ref:`Standalone mode`),
or by **initializing the OAK PoE camera** so DepthAI API can connect to it. OAK PoE cameras already come with factory bootloader flashed at the factory.

Bootloader is bundled inside with the ``depthai`` library, so to eg. flash the newest bootloader, you should use the newest ``depthai`` library.

Device Manager
##############

``device_manager.py`` is a Python script that interfaces with device :ref:`Bootloader` and bootloader configuration.
It can be found at `depthai-python/utilities <https://github.com/luxonis/depthai-python/tree/main/utilities>`__.

.. image:: https://github.com/luxonis/depthai-python/assets/18037362/1de0d86f-58bf-4979-b7d0-5ca7723db599

About Device
------------

Allows you to select the device you want to connect to, and you see metadata of connected device.

* First we have to select the device we want to connect (boot) to, you can select that using:

  * **Dropdown** which contains found device MX Ids. Dropdown will only get updated when starting the app.
  * **Specify IP** button if your OAK PoE camera isn't in the same LAN.
  * **Search** feature - a new window will show that has a table with all available cameras (either via USB port or on LAN - OAK PoEs), their MxId, name, and status. Clicking on a table row will select that device and boot to it.

Configuration settings
----------------------

After you select a device that has bootloader flashed, you can also configure bootloader configuration.

- If the device has **NETWORK bootloader flashed**, you will be able to set its static/dynamic IP/mask/gateway, DNS, MAC, etc.
- If the device has **USB bootloader flashed**, you will be able to set its USB max speed and USB timeout.

After setting some values, you have to click on the ``Flash configuration`` button. You can also ``Clear configuration``, or ``View configuration`` (its JSON).

.. figure:: https://github.com/luxonis/depthai-python/assets/18037362/4bced0ab-92fa-4a73-986f-4a3ba8848940

  When flashing static IP, make sure to also set the gateway/mask

Applications settings
---------------------

Useful when dealing with :ref:`Standalone mode` applications.

- **Flash application**: Select DepthAI Application Package (``.dap``) you want to flash the device. Below is a code snippet that showcases how to create the dap file.
- **Remove application**: Removes/clears the flashed application from the device
- **Open device streaming application**: Starts streaming frames from all connected cameras on the device.

.. code-block:: python

  import depthai as dai
  pipeline = dai.Pipeline()
  # ... add nodes to pipeline
  dai.DeviceBootloader.saveDepthaiApplicationPackage(
      './file.dap', # Where to save the .dap file
      pipeline, # My pipeline
      compress=True, # Compress the FW and assets. In my case, it went from 24MB -> 9.5MB
      applicationName='myAppName' # Optional, so you know which app is flashed afterwards
    )

Danger Zone
-----------

.. warning::

    This section can potentially soft-brick your device, so be careful when using it.

To prevent soft-bricking, OAK devices (since 2023) have factory bootloader and user bootloader. If user flashes a corrupted user bootloader, it will fallback to using factory bootloader. When updating bootloader,
Device Manager will try to flash the user bootloader first, if flashed (factory) bootloader supports it. If it's not possible, it will flash the factory bootloader.

* **Update Bootloader** button will flash the newest bootloader to the device. You can select AUTO, USB or NETWORK bootloader.

  * AUTO will select the connection type of bootloader with which the camera is currently connected to. If you are connected via USB (doing factory reset) to an OAK PoE camera, you shouldn't select AUTO, as it will flash USB bootloader.
  * USB bootloader will try to boot the application that is stored on flash memory. If it can't find flashed application, it will just behave as normal USB OAK - so it will wait until a host computer initializes the application.
  * NETWORK bootloader is used by the OAK PoE cameras, and is flashed at the factory. It handles network initialization so the OAK PoE cameras can be booted through the LAN.
* **Flash Factory Bootloader**: If you want to flash the factory bootloader, you can use this button. It will flash the factory bootloader, even if the user bootloader is already flashed.
* **Factory reset** will erase the whole flash content and re-flash it with only the USB or NETWORK bootloader. Flashed application (pipeline, assets) and bootloader configurations will be lost.
* **Boot into USB recovery mode** will force eg. OAK PoE camera to be available through the USB connector, even if its boot pins are set to PoE booting. It is mostly used by our firmware developers.

Factory and User bootloader
###########################

There are two types of bootloaders:

- **Factory bootloader**: bootloader that is flashed in the factory. We don't recommend re-flashing this bootloader, as it is not meant to be edited by end users.
- **User bootloader**: bootloader that can be flashed by the user. If booting is unsuccessful (eg. gets corrupted when flashing), it will fallback to factory bootloader.

USB devices don't support user bootloader. If device has User bootloader, it will be used by default. If user bootloader is not flashed, it will fallback to Factory bootloader.

Boot switches
#############

- **Boot from flash** - DIP switch: 0x03 (switches 5,6 ON) - used by OAK PoE and USB cameras when bootloader is installed.
- **Recovery mode for USB** - DIP switch: 0x16 (switches 2,4,5 ON) - to boot directly into USB mode, so camera waits for the host to connect to it via USB.

.. figure:: https://user-images.githubusercontent.com/18037362/154956812-c3fcc961-af46-4dfd-8080-e15c8c6b43f0.png

    OAK-D-PoE with switches 2,4,5 ON, for the purpose of connecting to the device via USB.

On newer versions of OAK devices we have 0 ohm resistors (see image below) instead of a DIP switch, which means OAK will boot into flash by default. These new devices
have the bootloader flashed, which handles the booting process. There's also an additional button on the baseboard that switches boot to recovery mode when
pressed, which can be useful if the bootloader hasn't yet been flashed (eg. early access devices). You need to press this button when powering the device (when booting
happens).

.. image:: https://user-images.githubusercontent.com/18037362/207295832-613fae0a-c0ae-411e-b03b-8a4736f1bfc7.png

API
###

**DeviceBootloader** is a class to communicate with the bootloader. It is used to flash created :ref:`Pipeline`, depthai application package
or update the bootloader itself.

.. code-block: cpp

    std::tuple<bool, std::string> flash(std::function<void(float)> progressCb, Pipeline& pipeline);

    std::tuple<bool, std::string> flashDepthaiApplicationPackage(std::function<void(float)> progressCb, std::vector<uint8_t> package);

    std::tuple<bool, std::string> flashBootloader(std::function<void(float)> progressCb, std::string path = "");

progressCb parameter takes a callback function, which will be called each time an progress update occurs (rate limited to 1 second). This is mainly
used to inform the user of the current flashing progress.

.. _DAP:

DepthAI Application Package (.dap)
##################################

**DepthAI application package** is a binary file format which stores sections of data. The purpose of this format is to be able to extract
individual sections and do OTA updates without requiring to update all data. Example: Between update 1 and 2 of users application,
Depthai firmware, Asset storage (50MiB neural network) and asset structure remained the same, but some additional processing nodes were added
to the pipeline. Instead of transferring the whole package, only Pipeline description can be sent and updated.

Depthai application package (**.dap**) consists of:

- SBR (512B header which describes sections of data)
- Depthai device firmware (section "__firmware")
- Pipeline description (section "pipeline")
- Assets structure (section "assets")
- Asset storage (section "asset_storage")

MAC address
###########

All OAK PoE cameras have a unique MAC address which is used to identify the device on the network. It is calculated from the
MxID of the device, see `logic here <https://gist.github.com/Erol444/85d6ef7ae9c946f5688854b86c548dc7>`__.
The MAC address is stored in the DeviceBootloader configuration.

.. include::  ../includes/footer-short.rst
