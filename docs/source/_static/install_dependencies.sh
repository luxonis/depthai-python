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

readonly debian_pkgs_pre22_04=(
    "${debian_pkgs[@]}"
    libdc1394-22-dev
)

readonly debian_pkgs_post22_04=(
    "${debian_pkgs[@]}"
    libdc1394-dev
)

debian_arm_pkgs=(
    "${debian_pkgs[@]}"
    # https://stackoverflow.com/a/53402396/5494277
    libhdf5-dev
    libhdf5-dev
    libatlas-base-dev
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

version_lte() {
    [[ "$1" == "$(echo -e "$1\n$2" | sort -V | head -n1)" ]]
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

    # Correctly determine if the architecture is ARM or aarch64
    IS_ARM=false
    if [[ $(uname -m) =~ ^arm* || $(uname -m) == "aarch64" ]]; then
        IS_ARM=true
    fi

    # Determine if ubuntu or debian based
    OS_BASE=""
    if [[ "$ID" == "ubuntu" || "$ID_LIKE" == "ubuntu" ]]; then
        OS_BASE="ubuntu"
    elif [[ "$ID" == "debian" || "$ID_LIKE" == "debian" ]]; then
        OS_BASE="debian"
    fi

    # Check if the version is less than or equal to the thresholds
    if { [[ "$OS_BASE" == "debian" ]] && version_lte "$VERSION_ID" "11"; } || 
    { [[ "$OS_BASE" == "ubuntu" ]] && version_lte "$VERSION_ID" "21.10"; }; then

        echo "Using pre-22.04 package list"
        if $IS_ARM; then
            echo "For ARM architecture"
            # Add libjasper-dev for ARM but not aarch64
            [[ $(uname -m) =~ ^arm* ]] && { debian_arm_pkgs+=("libjasper-dev"); }
            sudo apt-get install -y ${debian_arm_pkgs[@]/${debian_pkgs[@]}/${debian_pkgs_pre22_04[@]}}
        else
            # Install pre-22.04 packages for Debian or Ubuntu
            sudo apt-get install -y "${debian_pkgs_pre22_04[@]}"
        fi

        python3 -m pip install --upgrade pip

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

    # Check if the version is greater than the thresholds
    elif { [[ "$OS_BASE" == "debian" ]] && ! version_lte "$VERSION_ID" "11"; } || 
        { [[ "$OS_BASE" == "ubuntu" ]] && ! version_lte "$VERSION_ID" "21.10"; }; then

        echo "Using post-22.04 package list"
        if $IS_ARM; then
            echo "For ARM architecture"
            debian_arm_pkgs=("${debian_arm_pkgs[@]}")
            # Add libjasper-dev for ARM but not aarch64
            [[ $(uname -m) =~ ^arm* ]] && { debian_arm_pkgs+=("libjasper-dev"); }
            # Switching libbtbb2 to libtbbmalloc2
            debian_arm_pkgs=("${debian_arm_pkgs[@]/libtbb2/libtbbmalloc2}")

            sudo apt-get install -y ${debian_arm_pkgs[@]/${debian_pkgs[@]}/${debian_pkgs_post22_04[@]}}
        else
            # Install post-22.04 packages for Debian or Ubuntu
            sudo apt-get install -y "${debian_pkgs_post22_04[@]}"
        fi

        python3 -m pip install --upgrade pip

        # Check for uvcdynctrl package and recommend removal if found
        if dpkg -s uvcdynctrl &> /dev/null; then
            echo -e "\033[33mWe detected 'uvcdynctrl' installed on your system.\033[0m"
            # Instructions for removal
            echo -e "\033[33m$ sudo apt remove uvcdynctrl uvcdynctrl-data\033[0m"
            echo -e "\033[33m$ sudo rm -f /var/log/uvcdynctrl-udev.log\033[0m"
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
