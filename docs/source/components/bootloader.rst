.. _components_bootloader:

Bootloader
==========

Depthai bootloader is a small program which aids in booting and updating bootloader or depthai application packages.

To be able to run standalone (:ref:`documentation here <Standalone mode>`), the Depthai bootloader must be first
flashed to the devices flash. This step is required only once.

Once the device has the bootloader flashed, it will perform the same as before. Running pipelines with a host
connected doesn’t require any changes.

Suggested workflow is to perform as much of development as possible with the host connected as the
iteration cycle is greatly improved.

Once desired pipeline is created, use the following function to flash: :code:`DeviceBootloader::flash`

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

You can also check the version of the current bootloader by using the :ref:`Bootloader Version` example.

DepthAI Application Package (.dap)
##################################

Depthai application package is a binary file format which stores sections of data. The purpose of this format is to be able to extract
individual sections and do OTA updates without requiring to update all data. Example: Between update 1 and 2 of users application,
Depthai firmware, Asset storage (50MiB neural network) and asset structure remained the same, but some additional processing nodes were added
to the pipeline. Instead of transferring the whole package only Pipeline description can be sent and updated.

Depthai application package (**.dap**) consists of:

- SBR (512B header which describes sections of data)
- Depthai device firmware (section “__firmware”)
- Pipeline description (section “pipeline”)
- Assets structure (section “assets”)
- Asset storage (section “asset_storage”)

.. include::  ../includes/footer-short.rst
