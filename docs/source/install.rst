Installation
============

.. note::

  Refer to `DepthAI installer documentation <https://docs.luxonis.com/en/latest/pages/tutorials/first_steps>`__ to install
  full `DepthAI demo <https://github.com/luxonis/depthai#depthai-api-demo-program>`__ and its dependencies and requirements.

Follow the steps below to just install depthai api library dependencies for your platform.

.. tabs::

  .. tab:: **macOS**

    Execute the script below to install macOS dependencies:

    .. code-block:: bash

      curl -fL https://docs.luxonis.com/install_dependencies.sh | bash

    Please refer to :ref:`documentation below <macOS>` if any issues occur.

  .. tab:: **Windows 10/11**

    Windows 10/11 users can either **install DepthAI dependencies** via `Windows Installer <https://docs.luxonis.com/en/latest/pages/tutorials/first_steps>`__,
    or follow :ref:`instructions below <Windows 10>`.

  .. tab:: **Linux**

    Execute the script below to install Linux dependencies:

    .. code-block:: bash

      sudo wget -qO- https://docs.luxonis.com/install_dependencies.sh | bash

    Please refer to :ref:`Supported Platforms` if any issues occur.

Once installed, you can :ref:`install the DepthAI library <Install from PyPI>` from PyPI.

Supported Platforms
###################

See documentation below for other platforms or additional information.

.. list-table::
   :header-rows: 1

   * - Platform
     - Instructions
   * - Windows 10/11
     - :ref:`Platform dependencies <Windows 10>`
   * - macOS
     - :ref:`Platform dependencies <macOS>`
   * - Ubuntu
     - :ref:`Platform dependencies <Ubuntu>`
   * - Raspberry Pi OS
     - :ref:`Platform dependencies <Raspberry Pi OS>`
   * - Jetson Nano/Xavier
     - :ref:`Platform dependencies <Jetson>`
   * - ROS
     - Follow tutorial at `depthai-ros <https://github.com/luxonis/depthai-ros>`__
   * - Windows 7
     - :ref:`WinUSB driver <Windows 7>`
   * - Docker
     - :ref:`Pull and run official images <Docker>`
   * - Kernel Virtual Machine
     - :ref:`Run on KVM <Kernel Virtual Machine>`
   * - VMware
     - :ref:`Run on VMware <vmware>`
   * - Virtual Box
     - :ref:`Run on Virtual Box <Virtual Box>`
   * - WSL2
     - :ref:`Run on WSL2 <WSL 2>`

If you do not see a particular platform or package format listed below please reach out to us on `Discord <luxonis.com/discord>`__
or on `Github <https://github.com/luxonis/depthai>`__.

macOS
*****

.. code-block:: bash

  bash -c "$(curl -fL https://docs.luxonis.com/install_depthai.sh)"

Close and re-open the terminal window after this command.

The script also works on M1 Macs, Homebrew being installed under Rosetta 2, as some Python packages are still missing native M1
support.  In case you already have Homebrew installed natively and things don't work, see `here <https://github.com/luxonis/depthai/issues/299#issuecomment-757110966>`__
for some additional troubleshooting steps.

Note that if the video streaming window does not appear consider running the following:

.. code-block:: bash

    python3 -m pip install opencv-python --force-reinstall --no-cache-dir

See the `Video preview window fails to appear on macOS <https://discuss.luxonis.com/d/95-video-preview-window-fails-to-appear-on-macos>`_ thread on our forum for more information.

We provide **Mac M1 prebuilt Python wheels** for depthai since the version ``2.17.3.1``.

Ubuntu
******

.. code-block:: bash

  sudo wget -qO- https://docs.luxonis.com/install_dependencies.sh | bash


Note! If opencv fails with illegal instruction after installing from PyPi, add:

.. code-block:: bash

  echo "export OPENBLAS_CORETYPE=ARMV8" >> ~/.bashrc
  source ~/.bashrc


Raspberry Pi OS
***************

  .. code-block:: bash

    sudo curl -fL https://docs.luxonis.com/install_dependencies.sh | bash

We have also prepared `pre-configured RPi images <https://docs.luxonis.com/projects/hardware/en/latest/pages/guides/raspberrypi.html>`__ so you can get up & running faster.

Jetson
******

To install DepthAI on `Jetson Nano <https://developer.nvidia.com/embedded/jetson-nano-developer-kit>`__ or `Jetson Xavier <https://developer.nvidia.com/embedded/jetson-xavier-nx-devkit>`__,
perform the following steps, after completing a fresh install and setup. On the first log in, **do not** immediately run updates.

This first step is optional: go to the *Software* (App Store) and delete the apps or software that you probably will not use. 

Open a terminal window and run the following commands:

  .. code-block:: bash

    sudo apt update && sudo apt upgrade
    sudo reboot now

Change the size of your SWAP. These instructions come from the `Getting Started with AI on Jetson <https://developer.nvidia.com/embedded/learn/jetson-ai-certification-programs>`__ from Nvidia:

  .. code-block:: bash

    # Disable ZRAM:
    sudo systemctl disable nvzramconfig
    # Create 4GB swap file
    sudo fallocate -l 4G /mnt/4GB.swap
    sudo chmod 600 /mnt/4GB.swap
    sudo mkswap /mnt/4GB.swap

If you have an issue with the final command, you can try the following:

    .. code-block:: bash

      sudo vi /etc/fstab

      # Add this line at the bottom of the file
      /mnt/4GB.swap swap swap defaults 0 0

      # Reboot 
      sudo reboot now

The next step is to install :code:`pip` and :code:`python3`:

  .. code-block:: bash
  
    sudo -H apt install -y python3-pip

After that, install and set up virtual environment:

  .. code-block:: bash

    sudo -H pip3 install virtualenv virtualenvwrapper

Add following lines to the bash script:

  .. code-block:: bash

    sudo vi ~/.bashrc

    # Virtual Env Wrapper Configuration
    export WORKON_HOME=$HOME/.virtualenvs
    export VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3
    source /usr/local/bin/virtualenvwrapper.sh

Save and reload the script by running the command :code:`source ~/.bashrc`. Then create a virtual environment (in this example it's called :code:`depthAI`).
  
  .. code-block:: bash

    mkvirtualenv depthAI -p python3


**Note!** Before installing :code:`depthai`, make sure you're in the virtual environment.

  .. code-block:: bash

    #Download and install the dependency package
    sudo wget -qO- https://docs.luxonis.com/install_dependencies.sh | bash

    #Clone github repository
    git clone https://github.com/luxonis/depthai-python.git
    cd depthai-python

Last step is to edit :code:`.bashrc` with the line:

  .. code-block:: bash

    echo "export OPENBLAS_CORETYPE=ARMV8" >> ~/.bashrc


Navigate to the folder with :code:`depthai` examples folder, run :code:`python install_requirements.py` and then run :code:`python rgb_preview.py`.

Solution provided by `iacisme <https://github.com/iacisme>`__ via our `Discord <https://discord.com/channels/790680891252932659/795742008119132250>`__ channel.

openSUSE
********

For openSUSE, available `in this official article <https://en.opensuse.org/SDB:Install_OAK_AI_Kit>`__ how to install the OAK device on the openSUSE platform.

Windows 10
**********

For Windows 10/11, we recommend using the Chocolatey package manager to install DepthAI's dependencies on Windows. Chocolatey is very similar to Homebrew for macOS.

To `install Chocolatey <https://docs.chocolatey.org/en-us/choco/setup>`__ and
use it to install DepthAI's dependencies do the following:

- Right click on `Start`
- Choose `Windows PowerShell (Admin)` and run the following:

.. code-block:: bash

  Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

- Close the PowerShell and then re-open another PowerShell (Admin) by repeating the first two steps.
- Install Python and PyCharm

.. code-block:: bash

  choco install cmake git python pycharm-community -y

Windows 7
*********

Although we do not officially support Windows 7, members of the community `have
had success <https://discuss.luxonis.com/d/105-run-on-win7-sp1-x64-manual-instal-usb-driver>`__ manually installing WinUSB using `Zadig
<https://zadig.akeo.ie/>`__. After connecting your DepthAI device look for a
device with :code:`USB ID: 03E7 2485` and install the WinUSB driver by
selecting `WinUSB(v6.1.7600.16385)` and then `Install WCID Driver`.

WSL 2
*****

Steps below were performed on WSL 2 running Ubuntu 20.04, while host machine was running Win10 20H2 (OS build 19042.1586).
Original tutorial `written here <https://discuss.luxonis.com/d/693-i-got-depthai-demo-to-run-in-wsl>`__ by SputTheBot.

To get an OAK running on WSL 2, you first need to attach USB device to WSL 2. We have used `usbipd-win <https://github.com/dorssel/usbipd-win/releases>`__ (2.3.0)
for that. Inside WSL 2 you also need to install :ref:`depthai dependencies <Ubuntu>` and `USB/IP client tool <https://github.com/dorssel/usbipd-win/wiki/WSL-support#usbip-client-tools>`__ (2 commands).

To attach the OAK camera to WSL 2, we have prepared a Python script below that you need to execute on the host computer (in Admin mode).

.. code-block:: python

  import time
  import os
  while True:
      output = os.popen('usbipd wsl list').read() # List all USB devices
      rows = output.split('\n')
      for row in rows:
          if ('Movidius MyriadX' in row or 'Luxonis Device' in row) and 'Not attached' in row: # Check for OAK cameras that aren't attached
              busid = row.split(' ')[0]
              out = os.popen(f'usbipd wsl attach --busid {busid}').read() # Attach an OAK camera
              print(out)
              print(f'Usbipd attached Myriad X on bus {busid}') # Log
      time.sleep(.5)

After that, you can check ``lsusb`` command inside the WLS 2 and you should be able to see ``Movidius MyriadX``.

.. note::
  Examples that don't show any frames (eg. IMU example) should work. We haven't spent enough time to get OpenCV display frames inside WSL 2, but you could try it out yourself, some ideas `here <https://stackoverflow.com/questions/65453763/python3-9-on-wsl2-ubuntu-20-04-how-to-display-image-using-cv2-opencv-python-4>`__.

Docker
******

We maintain a Docker image containing DepthAI, it's dependencies and helpful
tools in the `luxonis/depthai-library <https://hub.docker.com/r/luxonis/depthai-library>`__
repository on Docker Hub. It builds upon the `luxonis/depthai-base
<https://hub.docker.com/r/luxonis/depthai-base>`__ image.

Run the :code:`rgb_preview.py` example inside a Docker container on a Linux host
(with the X11 windowing system):

.. code-block:: bash

   docker pull luxonis/depthai-library
   docker run --rm \
       --privileged \
       -v /dev/bus/usb:/dev/bus/usb \
       --device-cgroup-rule='c 189:* rmw' \
       -e DISPLAY=$DISPLAY \
       -v /tmp/.X11-unix:/tmp/.X11-unix \
       luxonis/depthai-library:latest \
       python3 /depthai-python/examples/ColorCamera/rgb_preview.py

To allow the container to update X11 you may need to run :code:`xhost local:root` on the host.

**Note: If you are using OAK POE** device on Linux host machine, you should add :code:`--network=host` argument to your docker command, so depthai inside docker will be able to communicate with the OAK POE.

Kernel Virtual Machine
**********************

To access the OAK-D camera in the `Kernel Virtual Machine <https://www.linux-kvm.org/page/Main_Page>`__, there is a need to attach and detach USB 
devices on the fly when the host machine detects changes in the USB bus.

OAK-D camera changes the USB device type when it is used by DepthAI API. This happens in background when the camera is used natively.
But when the camera is used in a virtual environment the situation is different.

On your host machine, use the following code:

.. code-block:: bash

  SUBSYSTEM=="usb", ACTION=="bind", ENV{ID_VENDOR_ID}=="03e7", MODE="0666", RUN+="/usr/local/bin/movidius_usb_hotplug.sh depthai-vm"
  SUBSYSTEM=="usb", ACTION=="remove", ENV{PRODUCT}=="3e7/2485/1", ENV{DEVTYPE}=="usb_device", MODE="0666", RUN+="/usr/local/bin/movidius_usb_hotplug.sh depthai-vm"
  SUBSYSTEM=="usb", ACTION=="remove", ENV{PRODUCT}=="3e7/f63b/100", ENV{DEVTYPE}=="usb_device", MODE="0666", RUN+="/usr/local/bin/movidius_usb_hotplug.sh depthai-vm"

The script that the udev rule is calling (movidius_usb_hotplug.sh) should then attach/detach the USB device to the virtual machine.
In this case we need to call :code:`virsh` command. For example, the script could do the following:

.. code-block::

  #!/bin/bash
  # Abort script execution on errors
  set -e
  if [ "${ACTION}" == 'bind' ]; then
    COMMAND='attach-device'
  elif [ "${ACTION}" == 'remove' ]; then
    COMMAND='detach-device'
    if [ "${PRODUCT}" == '3e7/2485/1' ]; then
      ID_VENDOR_ID=03e7
      ID_MODEL_ID=2485
    fi
    if [ "${PRODUCT}" == '3e7/f63b/100' ]; then
      ID_VENDOR_ID=03e7
      ID_MODEL_ID=f63b
    fi
  else
    echo "Invalid udev ACTION: ${ACTION}" >&2
    exit 1
  fi
  echo "Running virsh ${COMMAND} ${DOMAIN} for ${ID_VENDOR}." >&2
  virsh "${COMMAND}" "${DOMAIN}" /dev/stdin <<END
  <hostdev mode='subsystem' type='usb'>
    <source>
      <vendor id='0x${ID_VENDOR_ID}'/>
      <product id='0x${ID_MODEL_ID}'/>
    </source>
  </hostdev>
  END
  exit 0


Note that when the device is disconnected from the USB bus, some udev environmental variables are not available (:code:`ID_VENDOR_ID` or :code:`ID_MODEL_ID`),
that is why you need to use :code:`PRODUCT` environmental variable to identify which device has been disconnected.

The virtual machine where DepthAI API application is running should have defined a udev rules that identify the OAK-D camera.
The udev rule is described `here <https://docs.luxonis.com/en/latest/pages/faq/#does-depthai-work-on-the-nvidia-jetson-series>`__

Solution provided by `Manuel Segarra-Abad <https://github.com/maseabunikie>`__

VMware
******

Using the OAK-D device in a VMware requires some extra one-time settings that need to be set up for it to work. 

First of all, make sure  the USB controller is switched from USB2 to USB3. Go to :code:`Virtual Machine Settings -> USB Controller -> USB compatibility` and change 
to USB 3.1 (or USB 3.0 for older VMware versions, as available).

Depending on what state the device is, there could be two devices showing up, and both need to be routed to the VM. 
Those could be visible at :code:`Player -> Removable Devices`:

* Intel Movidius MyriadX
* Intel VSC Loopback Device or Intel Luxonis Device 

In Linux OS, run these commands to give USB permissions for the regular user:

.. code-block:: bash

  echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="03e7", MODE="0666"' | sudo tee /etc/udev/rules.d/80-movidius.rules
  sudo udevadm control --reload-rules && sudo udevadm trigger

If Virtual Machine doesn't detect the device, try the following: find and select option like *Forget connection rule* (for both devices), then try running
the DepthAI example again inside the VM. Choose to route to VM and select to *not ask again* (this is important, as there is a timeout, and the device 
watchdog could get triggered if the host doesn't start communication in few seconds). You may need to repeat running the script a few times, until all gets 
set properly for VMware.

Virtual Box
***********

If you want to use VirtualBox to run the DepthAI source code, please make sure that you allow the VM to access the USB devices. Also, be aware that 
by default, it supports only USB 1.1 devices, and DepthAI operates in two stages:

#. For showing up when plugged in. We use this endpoint to load the firmware onto the device, which is a usb-boot technique.  This device is USB2.
#. For running the actual code. This shows up after USB booting and is USB3.

In order to support the DepthAI modes, you need to download and install `Oracle VM VirtualBox Extension Pack <https://www.virtualbox.org/wiki/Downloads>`__.  Once this is installed, enable USB3 (xHCI) Controller in the USB settings.

Once this is done, you'll need to route the Myriad as USB device from Host to the VBox.  This is the filter for depthai before it has booted, which is 
at that point a USB2 device:

.. image:: https://user-images.githubusercontent.com/32992551/105070455-8d4d6b00-5a40-11eb-9bc6-19b164a55b4c.png
  :alt: Routing the not-yet-booted depthai to the VirtualBox.

The last step is to add the USB Intel Loopback device. The depthai device boots its firmware over USB, and after it has booted, it shows up as a new device.

This device shows just up when the depthai/OAK is trying to reconnect (during runntime, so right after running a pipeline on depthai, such as `:bash: python3 depthai_demo.py`).

It might take a few tries to get this loopback device shown up and added, as you need to do this while depthai is trying to connect after a pipeline has been built (and so it has at that point now booted its internal firmware over USB2).

For enabling it only once, you can see the loopback device here (after the pipeline has been started):

.. image:: https://user-images.githubusercontent.com/32992551/105112208-c527d300-5a7f-11eb-96b4-d14bcf974313.png
  :alt: Find the loopback device right after you tell depthai to start the pipeline, and select it.

And then for permanently enabling this pass-through to virtual box, enable this in setting below:

.. image:: https://user-images.githubusercontent.com/32992551/105070474-93dbe280-5a40-11eb-94b3-6557cd83fe1f.png
  :alt: Making the USB Loopback Device for depthai/OAK, to allow the booted device to communicate in virtualbox

And then for each additional depthai/OAK device you would like to pass through, repeat just this last loopback settings step for each unit (as each unit will have its own unique ID).


Install from PyPI
#################

After installing depthai dependencies, you can either refer to depthai-core for C++ development, or download the depthai **Python library**
`via PyPi <https://pypi.org/project/depthai/>`__:

.. code-block:: bash

  python3 -m pip install depthai

For other installation options, see :ref:`other installation options <Other installation methods>`.

Test installation
#################

We have `a set of examples <https://github.com/luxonis/depthai-python/tree/develop/examples>`__ that should help you verify if your setup was correct.

First, clone the `depthai-python <https://github.com/luxonis/depthai-python/tree/develop>`__ repository and change directory into this repo:

.. code-block:: bash

  git clone https://github.com/luxonis/depthai-python.git
  cd depthai-python

Next install the requirements for this repository.
Note that we recommend installing the dependencies in a virtual environment, so that they don't interfere with other Python
tools/environments on your system.

- For development machines like Mac/Windows/Ubuntu/etc., we recommend the `PyCharm <https://www.jetbrains.com/pycharm/>`__ IDE, as it automatically makes/manages virtual environments for you, along with a bunch of other benefits.  Alternatively, :code:`conda`, :code:`pipenv`, or :code:`virtualenv` could be used directly (and/or with your preferred IDE).
- For installations on resource-constrained systems, such as the Raspberry Pi or other small Linux systems, we recommend :code:`conda`, :code:`pipenv`, or :code:`virtualenv`.  To set up a virtual environment with :code:`virtualenv`, run :code:`virtualenv venv && source venv/bin/activate`.

Using a virtual environment (or system-wide, if you prefer), run the following to install the requirements for this example repository:

.. code-block:: bash

  cd examples
  python3 install_requirements.py

Now, run the :code:`rgb_preview.py` script from within :code:`examples` directory to make sure everything is working:

.. code-block:: bash

  python3 ColorCamera/rgb_preview.py

If all goes well a small window video display should appear.  And example is shown below:

.. raw:: html

    <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto;">
        <iframe src="https://www.youtube.com/embed/WP-Vo-awT9A" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
    </div>


Run Other Examples
##################

After you have run this example, you can run other examples to learn about DepthAI possibilities. You can also proceed to:

- Our tutorials, starting with a Hello World tutorial explaining the API usage step by step (:ref:`here <Hello World>`)
- Our experiments, containing implementations of various user use cases on DepthAI (`here <https://github.com/luxonis/depthai-experiments>`__)

You can also proceed below to learn how to convert your own neural network to run on DepthAI.

And we also have online model training below, which shows you how to train and convert models for DepthAI:

- Online ML Training and model Conversion: `HERE <https://github.com/luxonis/depthai-ml-training/tree/master/colab-notebooks>`__

Other installation methods
##########################

To get the latest and yet unreleased features from our source code, you can go ahead and compile depthai package manually.

.. tabs::

  .. tab:: **Build from source**

      **Dependencies to build from source**

      - CMake > 3.2.0
      - Generation tool (Ninja, make, ...)
      - C/C++ compiler
      - libusb1 development package

      .. tabs::

        .. tab:: Debian/Ubuntu/RPi OS

          On Debian based systems (Raspberry Pi OS, Ubuntu, ...) these can be acquired by running:

          .. code-block:: bash

            sudo apt-get -y install cmake libusb-1.0-0-dev build-essential

        .. tab:: maxOS

          Assuming a stock Mac OS X install, `depthai-python <https://github.com/luxonis/depthai-python>`__ library needs following dependencies

          - Homebrew (If it's not installed already)

            .. code-block:: bash

              /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"

          - Python, :code:`libusb`, CMake, :code:`wget`

            .. code-block:: bash

                brew install coreutils python3 cmake libusb wget

          And now you're ready to clone the `depthai-python <https://github.com/luxonis/depthai-python>`__ from Github and build it for Mac OS X.

  .. tab:: **Install from commit**

    Pip allows users to install the packages from specific commits, even if they are not yet released on PyPi.

    To do so, use the command below - and be sure to replace the :code:`<commit_sha>` with the correct commit hash `from here <https://github.com/luxonis/depthai-python/commits>`__

    .. code-block:: bash

        python3 -m pip install git+https://github.com/luxonis/depthai-python.git@<commit_sha>

  .. tab:: **Using specific branch/PR**

    From time to time, it may be of interest to use a specific branch.  This may occur, for example,
    because we have listened to your feature request and implemented a quick implementation in a branch.
    Or it could be to get early access to a feature that is soaking in our :code:`develop` for stability purposes before being merged into :code:`main`
    (:code:`develop` is the branch we use to soak new features before merging them into :code:`main`):

    So when working in the `depthai-python <https://github.com/luxonis/depthai-python>`__ repository, using a branch can be accomplished
    with the following commands.

    Prior to running the following, you can either clone the repository independently
    (for not over-writing any of your local changes) or simply do a :code:`git pull` first.

    .. code-block:: bash

      git checkout <branch>
      git submodule update --init --recursive
      python3 setup.py develop

  .. tab:: **Install from source**

    If desired, you can also install the package from the source code itself - it will allow you to make the changes
    to the API and see them live in action.

    To do so, first download the repository and then add the package to your python interpreter in development mode

    .. code-block:: bash

      git clone https://github.com/luxonis/depthai-python.git
      cd depthai-python
      git submodule update --init --recursive
      python3 setup.py develop  # you may need to add sudo if using system interpreter instead of virtual environment

    If you want to use other branch (e.g. :code:`develop`) than default (:code:`main`), you can do so by typing

    .. code-block:: bash

      git checkout develop  # replace the "develop" with a desired branch name
      git submodule update --recursive
      python3 setup.py develop

    Or, if you want to checkout a specific commit, type

    .. code-block:: bash

      git checkout <commit_sha>
      git submodule update --recursive
      python3 setup.py develop
