#!/bin/bash

trap 'RET=$? ; echo -e >&2 "\n\x1b[31mFailed installing dependencies. Could be a bug in the installer or unsupported platform. Open a bug report over at https://github.com/luxonis/depthai - exited with status $RET at line $LINENO \x1b[0m\n" ;
exit $RET' ERR

readonly linux_pkgs=(
    python3
    python3-pip
    udev
    cmake
    git
    python3-numpy
)

readonly debian_pkgs=(
    ${linux_pkgs[@]}
    # https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
    build-essential
    libgtk2.0-dev
    pkg-config
    libavcodec-dev
    libavformat-dev
    libswscale-dev
    python3-dev
    libtbb-dev
    libjpeg-dev
    libpng-dev
    libtiff-dev
    # https://stackoverflow.com/questions/55313610
    ffmpeg
    libsm6
    libxext6
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

readonly debian_arm_pkgs=(
    ${linux_pkgs[@]}
    # https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
    build-essential
    libgtk2.0-dev
    pkg-config
    libavcodec-dev
    libavformat-dev
    libswscale-dev
    python3-dev
    libtbb-dev
    libjpeg-dev
    libpng-dev
    libtiff-dev
    # https://stackoverflow.com/questions/55313610
    ffmpeg
    libsm6
    libxext6
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
    # https://stackoverflow.com/a/53402396/5494277
    libhdf5-dev
    libhdf5-dev
    libatlas-base-dev
    # https://github.com/EdjeElectronics/TensorFlow-Object-Detection-on-the-Raspberry-Pi/issues/18#issuecomment-433953426
    libilmbase-dev
    libopenexr-dev
    libgstreamer1.0-dev
)

readonly debian_pkgs_pre22_04=(
    libdc1394-22-dev
    libgl1-mesa-glx
    libtbb2

)
readonly debian_pkgs_post22_04=(
    libdc1394-dev
    libgl1-mesa-glx
    libtbbmalloc2

)
readonly debian_pkgs_23=(
    libdc1394-dev
    libgl1-mesa-dev
    libtbbmalloc2
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

# Check Bash version
if [[ "${BASH_VERSINFO:-0}" -lt 4 ]]; then
    echo "This script requires Bash 4.0 or higher. You are using Bash ${BASH_VERSION}. Please upgrade your Bash version."
    exit 1
fi

print_action () {
    green="\e[0;32m"
    reset="\e[0;0m"
    printf "\n$green >>$reset $*\n"
}
print_and_exec () {
    print_action $*
    $*
}

version_lte() {
    [[ "$1" == "$(echo -e "$1\n$2" | sort -V | head -n1)" ]]
}

declare -A debian_versions=(
  ["trixie/sid"]="13"
  ["bookworm/sid"]="12"
  ["bullseye/sid"]="11"
  ["buster/sid"]="10"
  ["stretch/sid"]="9"
  ["jessie/sid"]="8"
  ["wheezy/sid"]="7"
  ["squeeze/sid"]="6"
)

# Function to lookup and print Debian version number
lookup_debian_version_number() {
  debian_version_string="$1"
  version_number="${debian_versions[$debian_version_string]}"
  
  if [ -n "$version_number" ]; then
    echo "$version_number"
  else
    echo "None"
  fi
}

if [[ $(uname) == "Darwin" ]]; then
    if ! command -v brew &> /dev/null; then
        echo "During Homebrew install, certain commands need 'sudo'. Requesting access..."
        sudo true
        homebrew_install_url="https://raw.githubusercontent.com/Homebrew/install/master/install.sh"
        print_action "Installing Homebrew from $homebrew_install_url"
        # CI=1 will skip some interactive prompts
        CI=1 /bin/bash -c "$(curl -fsSL $homebrew_install_url)"
        echo
        echo "=== Installed successfully!  IMPORTANT: For changes to take effect,"
        echo "please close and reopen the terminal window, or run:  exec \$SHELL"
    else
        echo "Homebrew is already installed."
    fi

    if ! command -v git &> /dev/null; then
        echo "Git not found, installing using Homebrew..."
        print_and_exec brew install git
    else
        echo "Git Already installed.."
    fi

elif [ -f /etc/os-release ]; then
    source /etc/os-release
    if [ -f /etc/debian_version ]; then
        output=$(cat /etc/debian_version)
        echo $output
        if [[ $output == *sid ]]; then
            version=$(lookup_debian_version_number $output)
        else 
            version=$output
        fi

        # Correctly determine if the architecture is ARM or aarch64
        IS_ARM=false
        if [[ $(uname -m) =~ ^arm* || $(uname -m) == "aarch64" ]]; then
            IS_ARM=true
        fi

        echo "$version"
        echo "$IS_ARM"

        if [ $IS_ARM ]; then
            sudo DEBIAN_FRONTEND=noninteractive apt install -y "${debian_arm_pkgs[@]}"
            if [[ $version == 13* ]]; then
                echo "Detected ARM Debian 13"
                sudo apt install -y "${debian_pkgs_23[@]}"
            elif version_lte "$version" "11.99"; then
                echo "Using pre-22.04 ARM package list"
                sudo apt-get install -y ${debian_pkgs_pre22_04[@]}

                # Check for uvcdynctrl package and recommend removal if found
                if dpkg -s uvcdynctrl &> /dev/null; then
                    echo -e "\033[33mWe detected 'uvcdynctrl' installed on your system.\033[0m"
                    # Instructions for removal
                    echo -e "\033[33m$ sudo apt remove uvcdynctrl uvcdynctrl-data\033[0m"
                    echo -e "\033[33m$ sudo rm -f /var/log/uvcdynctrl-udev.log\033[0m"
                fi


            else
                echo "Using post-22.04 ARM package list"
                sudo apt-get install -y ${debian_pkgs_post22_04[@]}
            fi

            # Add libjasper-dev for ARM but not aarch64
            [[ $(uname -m) =~ ^arm* ]] && { sudo apt install -y libjasper-dev; }

        else
            sudo DEBIAN_FRONTEND=noninteractive apt install -y "${debian_pkgs[@]}"
            if [[ $version == 13* ]]; then
                echo "Detected Debian 13"
                sudo apt install -y "${debian_pkgs_23[@]}"
            elif version_lte "$version" "11.99"; then
                echo "Using pre-22.04 package list"
                sudo apt-get install -y "${debian_pkgs_pre22_04[@]}"
                
            else
                echo "Using post-22.04 package list"
                sudo apt-get install -y "${debian_pkgs_post22_04[@]}"
            fi
        fi

        # Check for uvcdynctrl package and recommend removal if found
        if dpkg -s uvcdynctrl &> /dev/null; then
            echo -e "\033[33mWe detected 'uvcdynctrl' installed on your system.\033[0m"
            # Instructions for removal
            echo -e "\033[33m$ sudo apt remove uvcdynctrl uvcdynctrl-data\033[0m"
            echo -e "\033[33m$ sudo rm -f /var/log/uvcdynctrl-udev.log\033[0m"
        fi

        
            
        if [ "$VERSION_ID" == "21.04" ]; then
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
