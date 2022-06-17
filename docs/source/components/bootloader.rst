.. _components_bootloader:

Bootloader
==========

DepthAI bootloader is a small program which **handles the booting process**, either by **booting the flashed application**,
or by **initializing the OAK PoE camera** so DepthAI API can connect to it.

To be able to run in :ref:`Standalone mode`, the Depthai bootloader must be first flashed to the devices flash.
This step is required only once.

Once the device has the bootloader flashed, it will perform the same as before. Running pipelines with a host
connected doesn't require any changes.

Suggested workflow is to perform as much of development as possible with the host connected as the
iteration cycle is greatly improved.

Device Manager
##############

``device_manager.py`` is a Python helper that interfaces with device :ref:`Bootloader` and bootloader configuration.
It can be found at `depthai-python/utilities <https://github.com/luxonis/depthai-python/tree/main/utilities>`__.

.. image:: https://user-images.githubusercontent.com/18037362/171629704-0f78f31a-1778-4338-8ac0-bdfb0d2d593f.png

Device Manager Usage
--------------------

**About device tab** - Select a camera to see its metadata - like MxID, flashed bootloader version, device state etc.

* First we have to select the device we want to connect (boot) to, you can select that using:

  * **Dropdown** which contains found device MX Ids. Dropdown will only get updated when starting the app.
  * **Specify IP** button if your OAK PoE camera isn't in the same LAN.
  * **Search** feature - a new window will show that has a table with all available cameras (either via USB port or on LAN - OAK PoEs), their MxId, name, and status. Clicking on a table row will select that device and boot to it.
* ``Flash newest Bootloader`` button will flash the ``newest bootloader`` to the device. You can select AUTO, USB or NETWORK bootloader.

  * **AUTO** will select the connection type of bootloader with which the camera is currently connected to. If you are connected via USB (doing factory reset) to an OAK PoE camera, you shouldn't select AUTO, as it will flash USB bootloader.
  * **USB** bootloader will try to boot the application that is stored on flash memory. If it can't find flashed application, it will just behave as normal USB OAK - so it will wait until a host computer initializes the application.
  * **NETWORK** bootloader is used by the OAK PoE cameras, and is flashed at the factory. It handles network initialization so the OAK PoE cameras can be booted through the LAN.
* ``Factory reset`` will erase the whole flash content and re-flash it with only the USB or NETWORK bootloader. Flashed application (pipeline, assets) and bootloader configurations will be lost.
* ``Boot into USB recovery mode`` will force eg. OAK PoE camera to be available through the USB connector, even if its boot pins are set to PoE booting. It is mostly used by our firmware developers.

**Configuration settings tab** - After you select a device that has bootloader flashed, you can also configure bootloader
configuration.

- If the device has **NETWORK bootloader flashed**, you will be able to set its static/dynamic IP/mask/gateway, DNS, MAC, etc.
- If the device has **USB bootloader flashed**, you will be able to set its USB max speed and USB timeout.

After setting some values, you have to click on the ``Flash configuration`` button. You can also flash a :ref:`DAP`,
or clear the bootloader config.

Boot switches
#############

- **Boot from flash** - DIP switch: 0x03 (switches 5,6 ON) - used by OAK PoE and USB cameras when bootloader is installed.
- **Recovery mode for USB** - DIP switch: 0x16 (switches 2,4,5 ON) - to boot directly into USB mode, so camera waits for the host to connect to it via USB.

.. image:: https://user-images.githubusercontent.com/18037362/154956812-c3fcc961-af46-4dfd-8080-e15c8c6b43f0.png

OAK-D-PoE with switches 2,4,5 ON, for the purpose of connecting to the device via USB.

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
- Depthai device firmware (section “__firmware”)
- Pipeline description (section “pipeline”)
- Assets structure (section “assets”)
- Asset storage (section “asset_storage”)

.. include::  ../includes/footer-short.rst
