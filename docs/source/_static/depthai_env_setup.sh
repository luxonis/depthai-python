#!/bin/bash

# create environment variables
export APP_NAME="depthai"
WORKING="$HOME/$APP_NAME"
install_path=""
export WORKING_DIR="$WORKING"
path_correct="false"

trap 'RET=$? ; echo -e >&2 "\n\x1b[31mFailed installing dependencies. Could be a bug in the installer or unsupported platform. Open a bug report over at https://github.com/luxonis/depthai - exited with status $RET at line $LINENO \x1b[0m\n" ; exit $RET' ERR

while [ "$path_correct" = "false" ]
do
  echo ""
  read -p $'ENTER absolute installation path for depthai or leave empty and default path: $HOME will be used.\n' -r install_path
  echo ""

  if [ "$install_path" = "" ]; then
    echo "Using default installation path: $WORKING_DIR"
  else
    echo "Using given installation path: $install_path"
    export WORKING_DIR="$install_path/$APP_NAME"
  fi

  if [ -d "$(dirname "$WORKING_DIR")" ]; then
    echo "Directory: $WORKING_DIR is OK"
    path_correct="true"
  else
    echo "Directory: $WORKING_DIR is not valid. Try again!"
  fi
done

write_in_file () {
  # just make sure only strings are appended which are not in there yet
  # first arg is text to write, second arg is the file path
  if ! grep -Fxq "$1" "$2"
  then
    echo "$1" >> "$2"
  fi
}

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

echo "Finished installing global libraries."

# add depthai working dir to .bashrc if its not already there
COMMENT='# Home directory of depthai and depthai app, enables to run <run_demo> in terminal'
BASHRC="$HOME/.bashrc"
ADD_DEPTHAI_TO_PATH='export PATH=$PATH'":$WORKING_DIR"
ADD_LUXONIS_DEPTHAI_HOME='export LUXONIS_DEPTHAI_HOME='"$WORKING_DIR"

# add to .bashrc only if it is not in there already
write_in_file "$COMMENT" "$BASHRC"
write_in_file "$ADD_DEPTHAI_TO_PATH" "$BASHRC"
write_in_file "$ADD_LUXONIS_DEPTHAI_HOME" "$BASHRC"


if [[ $(uname -s) == "Darwin" ]]; then
  echo _____________________________
  echo "Calling macOS_installer.sh"
  echo _____________________________
  echo "Running macOS installer."
  echo "Upgrading brew."
  brew upgrade

  # clone depthai form git
  if [ -d "$WORKING_DIR" ]; then
     echo "Demo app already downloaded. Checking out main and updating."
  else
     echo "Downloading demo app."
     git clone https://github.com/luxonis/depthai.git "$WORKING_DIR"
  fi
  cd "$WORKING_DIR"
  git fetch
  git checkout demo_app_installation_v2
  git pull

  # install python 3.10 and python dependencies
  brew update
  echo "installing python 3.10"
  brew install python@3.10

  # pip does not have pyqt5 for arm
  if [[ $(uname -m) == 'arm64' ]]; then
    echo "Installing pyqt5 with homebrew."
    brew install pyqt@5
  fi

  # create python virtual environment
  echo "Creating python virtual environment in $WORKING_DIR/venv"

  python3.10 -m venv "$WORKING_DIR/venv"
  # activate environment
  source "$WORKING_DIR/venv/bin/activate"
  pip install --upgrade pip

  # install launcher dependencies
  # only on mac silicon point PYTHONPATH to pyqt5 installation via homebrew, otherwise install pyqth5 with pip
  if [[ $(uname -m) == 'arm64' ]]; then
    #if [[ $(uname -s) == "Darwin" ]]; then
    if [[ ":$PYTHONPATH:" == *":/opt/homebrew/lib/python3.10/site-packages:"* ]]; then
      echo "/opt/homebrew/lib/python3.10/site-packages already in PYTHONPATH"
    else
      export PYTHONPATH=/opt/homebrew/lib/python3.10/site-packages:$PYTHONPATH
      echo "/opt/homebrew/lib/python3.10/site-packages added to PYTHONPATH"
    fi
    #fi
  else
    pip install pyqt5
  fi

  pip install packaging

elif [[ $(uname -s) == "Linux" ]]; then
  echo _____________________________
  echo "Calling linux_installer.sh"
  echo _____________________________
  echo $'\nRunning Linux installer.'
  echo "Updating sudo-apt."
  echo "$WORKING_DIR"

  sudo apt-get update

  # clone depthai form git

  if [ -d "$WORKING_DIR" ]; then
     echo "Demo app already downloaded. Checking out main and updating."

  else
     echo "Downloading demo app."
     git clone https://github.com/luxonis/depthai.git "$WORKING_DIR"
  fi

  cd "$WORKING_DIR"
  git fetch
  git checkout demo_app_installation_v2
  git pull

  # install python 3.10
  echo "installing python 3.10"

  sudo yes "" | sudo add-apt-repository ppa:deadsnakes/ppa
  sudo apt -y install python3.10
  sudo apt -y install python3.10-venv

  # create python virtual environment
  echo "Creating python virtual environment in $WORKING_DIR/venv"

  python3.10 -m venv "$WORKING_DIR/venv"
  # activate environment
  source "$WORKING_DIR/venv/bin/activate"
  pip install --upgrade pip

  pip install packaging
  pip install pyqt5
else
  echo "Error: Host $(uname -s) not supported."
  exit 99
fi

echo $'\n\n:::::::::::::::: INSTALATION COMPLETE ::::::::::::::::\n'
echo $'\nTo run demo app write <run_demo> in terminal.'
read -rsp $'Press ANY KEY to finish and run the demo app...\n' -n1 key
echo "STARTING DEMO APP."
python3.10 "$WORKING_DIR/launcher/launcher.py" -r "$WORKING_DIR"

