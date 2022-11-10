#!/bin/bash

trap 'RET=$? ; echo -e >&2 "\n\x1b[31mFailed installing dependencies. Could be a bug in the installer or unsupported platform. Open a bug report over at https://github.com/luxonis/depthai - exited with status $RET at line $LINENO \x1b[0m\n" ; exit $RET' ERR

readonly linux_pkgs=(
    python3
    python3-pip
    udev
    cmake
    git
    python3-numpy
)

readonly ubuntu_pkgs=(
    ${linux_pkgs[@]}
    # https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
    build-essential
    libgtk2.0-dev
    pkg-config
    libavcodec-dev
    libavformat-dev
    libswscale-dev
    python3-dev
    libtbb2
    libtbb-dev
    libjpeg-dev
    libpng-dev
    libtiff-dev
    # https://stackoverflow.com/questions/55313610
    ffmpeg
    libsm6
    libxext6
    libgl1-mesa-glx
    python3-pyqt5
    python3-pyqt5.qtquick
    qml-module-qtquick-controls2
    qml-module-qt-labs-platform
    qtdeclarative5-dev
    qml-module-qtquick2
    qtbase5-dev
    qtchooser
    qt5-qmake
    qtbase5-dev-tools
    qml-module-qtquick-layouts
    qml-module-qtquick-window2
)

readonly ubuntu_pkgs_pre22_04=(
    "${ubuntu_pkgs[@]}"
    libdc1394-22-dev
)

readonly ubuntu_pkgs_22_04=(
    "${ubuntu_pkgs[@]}"
    libdc1394-dev
)

readonly ubuntu_arm_pkgs=(
    "${ubuntu_pkgs[@]}"
    libdc1394-22-dev
    # https://stackoverflow.com/a/53402396/5494277
    libhdf5-dev
    libhdf5-dev
    libatlas-base-dev
    libjasper-dev
    # https://github.com/EdjeElectronics/TensorFlow-Object-Detection-on-the-Raspberry-Pi/issues/18#issuecomment-433953426
    libilmbase-dev
    libopenexr-dev
    libgstreamer1.0-dev
)

readonly fedora_pkgs=(
    ${linux_pkgs[@]}
    gtk2-devel
    # Fedora uses pkgconf instead of pkg-config
    tbb-devel
    libjpeg-turbo-devel
    libpng-devel
    libtiff-devel
    libdc1394-devel
    # TODO(PM): ffmpeg requires enabling rpmfusion-free-updates
    # TODO(PM): libavcodec-dev libavformat-dev libswscale-dev python-dev libtbb2
    #           libsm6 libxext6 libgl1-mesa-glx
)

print_action () {
    green="\e[0;32m"
    reset="\e[0;0m"
    printf "\n$green >>$reset $*\n"
}
print_and_exec () {
    print_action $*
    $*
}

if [[ $(uname) == "Darwin" ]]; then
    echo "During Homebrew install, certain commands need 'sudo'. Requesting access..."
    sudo true
    homebrew_install_url="https://raw.githubusercontent.com/Homebrew/install/master/install.sh"
    print_action "Installing Homebrew from $homebrew_install_url"
    # CI=1 will skip some interactive prompts
    CI=1 /bin/bash -c "$(curl -fsSL $homebrew_install_url)"
    print_and_exec brew install git
    echo
    echo "=== Installed successfully!  IMPORTANT: For changes to take effect,"
    echo "please close and reopen the terminal window, or run:  exec \$SHELL"
elif [ -f /etc/os-release ]; then
    # shellcheck source=/etc/os-release
    source /etc/os-release

    if [[ "$ID" == "ubuntu" || "$ID" == "debian" || "$ID_LIKE" == "ubuntu" || "$ID_LIKE" == "debian" || "$ID_LIKE" == "ubuntu debian" ]]; then
        if [[ ! $(uname -m) =~ ^arm* ]]; then
            sudo apt-get update
            if [[ "$VERSION_ID" == "22.04" ]]; then
                sudo apt-get install -y "${ubuntu_pkgs_22_04[@]}"
            else
                sudo apt-get install -y "${ubuntu_pkgs_pre22_04[@]}"
            fi
            python3 -m pip install --upgrade pip
        elif [[ $(uname -m) =~ ^arm* ]]; then
            sudo apt-get update
            sudo apt-get install -y "${ubuntu_arm_pkgs[@]}"
            python3 -m pip install --upgrade pip
        fi

        # As set -e is set, retrieve the return value without exiting
        RET=0
        dpkg -s uvcdynctrl > /dev/null 2>&1 || RET=$? || true
        # is uvcdynctrl installed
        if [[ "$RET" == "0" ]]; then
          echo -e "\033[33mWe detected \"uvcdynctrl\" installed on your system. \033[0m"
          echo -e "\033[33mWe recommend removing this package, as it creates a huge log files if a camera is used in UVC mode (webcam)\033[0m"
          echo -e "\033[33mYou can do so by running the following commands:\033[0m"
          echo -e "\033[33m$ sudo apt remove uvcdynctrl uvcdynctrl-data\033[0m"
          echo -e "\033[33m$ sudo rm -f /var/log/uvcdynctrl-udev.log\033[0m"
          echo ""
        fi

        OS_VERSION=$(lsb_release -r |cut -f2)
        if [ "$OS_VERSION" == "21.04" ]; then
            echo -e "\033[33mThere are known issues with running our demo script on Ubuntu 21.04, due to package \"python3-pyqt5.sip\" not being in a correct version (>=12.9)\033[0m"
            echo -e "\033[33mWe recommend installing the updated version manually using the following commands\033[0m"
            echo -e "\033[33m$ wget http://mirrors.kernel.org/ubuntu/pool/universe/p/pyqt5-sip/python3-pyqt5.sip_12.9.0-1_amd64.deb\033[0m"
            echo -e "\033[33m$ sudo dpkg -i python3-pyqt5.sip_12.9.0-1_amd64.deb\033[0m"
            echo ""
        fi
    elif [[ "$ID" == "fedora" ]]; then
        sudo dnf update -y
        sudo dnf install -y "${fedora_pkgs[@]}"
        sudo dnf groupinstall -y "Development Tools" "Development Libraries"
        python3 -m pip install --upgrade pip
    else
        echo "ERROR: Distribution not supported"
        exit 99
    fi

    # Allow all users to read and write to Myriad X devices
    echo "Installing udev rules..."
    echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="03e7", MODE="0666"' | sudo tee /etc/udev/rules.d/80-movidius.rules > /dev/null
    sudo udevadm control --reload-rules && sudo udevadm trigger
else
    echo "ERROR: Host not supported"
    exit 99
fi

# clone depthai form git
git clone https://github.com/luxonis/depthai.git ~/depthai
cd depthai

# install pyenv, python 3.10 and python dependencies
brew update
echo "installing pyenv, virtualenv and pyqt5"
brew install pyenv, pyenv-virtualenv

# pip does not have pyqt5 for arm
if [[ $(uname -m) == 'arm64' ]]; then
  brew install pyqt@5
fi

# pyenv installation guide from here: https://github.com/pyenv/pyenv

PROFILE=$"~/.profile"
BASH_PROFILE=$"~/.bash_profile"
BASH_LOGIN=$"~/.bash_login"
BASH_PATHS=mypaths=( PROFILE BASH_PROFILE BASH_LOGIN )


# Bash warning: There are some systems where the BASH_ENV variable is configured to point to .bashrc. On such systems, you should almost certainly put the eval "$(pyenv init -)" line into .bash_profile, and not into .bashrc
if [ "$BASH_ENV" = "~/.bashrc" ]
   echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $BASH_PROFILE
   echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $BASH_PROFILE
   echo 'eval "$(pyenv init -)"' >> $BASH_PROFILE
else
   echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bashrc
   echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bashrc
   echo 'eval "$(pyenv init -)"' >> ~/.bashrc
fi

#  if you have ~/.profile, ~/.bash_profile or ~/.bash_login, add the commands there as well. If you have none of these, add them to ~/.profile.
# case none of them exist
if [ ![-f "$PROFILE"] && ![-f $BASH_PROFILE] && ![-f $BASH_LOGIN] ]; then
   echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $PROFILE
   echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $PROFILE
   echo 'eval "$(pyenv init -)"' >> $PROFILE
elif [ -f "$PROFILE" ]
   echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $PROFILE
   echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $PROFILE
   echo 'eval "$(pyenv init -)"' >> $PROFILE
elif [ -f "$BASH_PROFILE" ]
   echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $BASH_PROFILE
   echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $BASH_PROFILE
   echo 'eval "$(pyenv init -)"' >> $BASH_PROFILE
elif [ -f "$BASH_LOGIN" ]
   echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $BASH_LOGIN
   echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $BASH_LOGIN
   echo 'eval "$(pyenv init -)"' >> $BASH_LOGIN
fi

# reset shell
exec "$SHELL"

if [ which pyenv ]; then
  echo "installing python dependencies."
   # install latest python 3.10
   pyenv install 3.10
   pyenv virtualenv 3.10 demo_app_venv
   pyenv activate demo_app_venv
   python install_requirements.py

else
  echo "Pyenv command does not work, pyenv setup was not successful."
  exit 99
