Installation
============

Please :ref:`install the necessary dependencies <Supported Platforms>` for your
platform by referring to the table below. Once installed you can :ref:`install
the DepthAI library <Install from PyPI>`.

We are constantly striving to improve how we release our software to keep up
with countless platforms and the numerous ways to package it.  If you do not
see a particular platform or package format listed below please reach out to
us on `Discord <https://discord.com/channels/790680891252932659/794255653870370857>`__
or on `Github <https://github.com/luxonis/depthai>`__.

Supported Platforms
###################

We keep up-to-date, pre-compiled, libraries for the following platforms.  Note that a new change is that for Ubuntu now also work unchanged for the Jetson/Xavier series:

======================== =========================================== ================================================= ================================================================================
Platform                 Instructions                                Tutorial                                          Support
======================== =========================================== ================================================= ================================================================================
Windows 10               :ref:`Platform dependencies <Windows>`      `Video tutorial <https://youtu.be/ekopKJfcWiE>`__ `Discord <https://discord.com/channels/790680891252932659/798284448323731456>`__
macOS                    :ref:`Platform dependencies <macOS>`        `Video tutorial <https://youtu.be/0RGmmjed3Hc>`__ `Discord <https://discord.com/channels/790680891252932659/798283911989690368>`__
Ubuntu & Jetson/Xavier   :ref:`Platform dependencies <Ubuntu>`       `Video tutorial <https://youtu.be/QXeXMaxj4cM>`__ `Discord <https://discord.com/channels/790680891252932659/798302162160451594>`__
Raspberry Pi             :ref:`Platform dependencies <Raspberry Pi>` `Video tutorial <https://youtu.be/BpUMT-xqwqE>`__ `Discord <https://discord.com/channels/790680891252932659/798302708070350859>`__
======================== =========================================== ================================================= ================================================================================

And the following platforms are also supported by a combination of the community and Luxonis.

====================== ===================================================== ================================================================================
Platform               Instructions                                          Support
====================== ===================================================== ================================================================================
Fedora                                                                       `Discord <https://discord.com/channels/790680891252932659/798592589905264650>`__
Robot Operating System                                                       `Discord <https://discord.com/channels/790680891252932659/795749142793420861>`__
Windows 7              :ref:`WinUSB driver <Windows 7>`                      `Discord <https://discord.com/channels/790680891252932659/798284448323731456>`__
Docker                 :ref:`Pull and run official images <Docker>`          `Discord <https://discord.com/channels/790680891252932659/796794747275837520>`__
====================== ===================================================== ================================================================================

macOS
*****

.. code-block:: bash

  bash -c "$(curl -fL http://docs.luxonis.com/_static/install_dependencies.sh)"

Close and re-open the terminal window after this command.

The script also works on M1 Macs, Homebrew being installed under Rosetta 2, as some Python packages are still missing native M1 support.  In case you already have Homebrew installed natively and things don't work, see `here <https://github.com/luxonis/depthai/issues/299#issuecomment-757110966>`__ for some additional troubleshooting steps.

Note that if the video streaming window does not appear consider running the
following:

.. code-block:: bash

    python3 -m pip install opencv-python --force-reinstall --no-cache-dir

See the `Video preview window fails to appear on macOS <https://discuss.luxonis.com/d/95-video-preview-window-fails-to-appear-on-macos>`_ thread on our forum for more information.

Raspberry Pi OS
***************

.. code-block:: bash

  sudo curl -fL http://docs.luxonis.com/_static/install_dependencies.sh | bash

Ubuntu
******

Note that these Ubuntu instructions also work for the **Jetson** and **Xavier** series.

.. code-block:: bash

  sudo wget -qO- http://docs.luxonis.com/_static/install_dependencies.sh | bash

openSUSE
********

For openSUSE, available `in this official article <https://en.opensuse.org/SDB:Install_OAK_AI_Kit>`__ how to install the OAK device on the openSUSE platform.

Windows
*******

We recommend using the Chocolatey package manager to install DepthAI's
dependencies on Windows. Chocolatey is very similar to Homebrew for macOS.
Alternatively, it is also possible to :ref:`install DepthAI and its
dependencies manually <Manually install DepthAI on Windows>`, although it can
be more time consuming and error prone.

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
---------

Although we do not officially support Windows 7, members of the community `have
had success <https://discuss.luxonis.com/d/105-run-on-win7-sp1-x64-manual-instal-usb-driver>`__ manually installing WinUSB using `Zadig
<https://zadig.akeo.ie/>`__. After connecting your DepthAI device look for a
device with :code:`USB ID: 03E7 2485` and install the WinUSB driver by
selecting `WinUSB(v6.1.7600.16385)` and then `Install WCID Driver`.

Docker
******

We maintain a Docker image containing DepthAI, it's dependencies and helpful
tools in the `luxonis/depthai-library <https://hub.docker.com/r/luxonis/depthai-library>`__
repository on Docker Hub. It builds upon the `luxonis/depthai-base
<https://hub.docker.com/r/luxonis/depthai-base>`__ image.

Run the :code:`01_rgb_preview.py` example inside a Docker container on a Linux host
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
       python3 /depthai-python/examples/01_rgb_preview.py

To allow the container to update X11 you may need to run :code:`xhost local:root` on
the host.

Install from PyPI
#################

Our packages are distributed `via PyPi <https://pypi.org/project/depthai/>`__, to install it in your environment use

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

  python3 -m pip install -r requirements.txt

Now, run the :code:`01_rgb_preview.py` script from within :code:`examples` directory to make sure everything is working:

.. code-block:: bash

  python3 examples/01_rgb_preview.py

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

Dependencies to build from source
*********************************

- CMake > 3.2.0
- Generation tool (Ninja, make, ...)
- C/C++ compiler
- libusb1 development package

.. _raspbian:

Ubuntu, Raspberry Pi OS, ... (Debian based systems)
---------------------------------------------------

On Debian based systems (Raspberry Pi OS, Ubuntu, ...) these can be acquired by running:

.. code-block:: bash

  sudo apt-get -y install cmake libusb-1.0-0-dev build-essential

macOS (Mac OS X)
----------------

Assuming a stock Mac OS X install, `depthai-python <https://github.com/luxonis/depthai-python>`__ library needs following dependencies

- Homebrew (If it's not installed already)

  .. code-block:: bash

    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"

- Python, :code:`libusb`, CMake, :code:`wget`

  .. code-block:: bash

      brew install coreutils python3 cmake libusb wget

And now you're ready to clone the `depthai-python <https://github.com/luxonis/depthai-python>`__ from Github and build it for Mac OS X.

Install using GitHub commit
***************************

Pip allows users to install the packages from specific commits, even if they are not yet released on PyPi.

To do so, use the command below - and be sure to replace the :code:`<commit_sha>` with the correct commit hash `from here <https://github.com/luxonis/depthai-python/commits>`__

.. code-block:: bash

    python3 -m pip install git+https://github.com/luxonis/depthai-python.git@<commit_sha>

Using/Testing a Specific Branch/PR
**********************************

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

Install from source
*******************

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
