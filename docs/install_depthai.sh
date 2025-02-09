#!/bin/bash
set -e

VENV_PATH="$HOME/.local/share/virtualenvs/depthai-viewer"

echo "Installing viewer dependencies..."

readonly linux_pkgs=(
    python3
    python3-pip
    python3-venv
    python3-dev
    udev
    cmake
    git
    python3-numpy
)
PYTHONPATH=$(which python3)
# Function to create virtual environment
create_venv() {
  if [ ! -d "$VENV_PATH" ]; then
    echo "Creating virtual environment at $VENV_PATH"
    mkdir -p "$VENV_PATH"
    "$PYTHONPATH" -m venv "$VENV_PATH"
  else
    echo "Virtual environment already exists at $VENV_PATH"
  fi
}

# Function to install depthai viewer
install_depthai() {
  echo "Installing viewer in virtual environment..."
  "$VENV_PATH/bin/python" -m pip install --upgrade pip
  "$VENV_PATH/bin/python" -m pip install depthai-viewer packaging  depthai

  sudo mkdir -p /usr/local/bin/
  printf '#!/bin/bash\nsource %s/bin/activate\n depthai-viewer "$@"' $VENV_PATH | sudo tee /usr/local/bin/depthai-viewer > /dev/null
  sudo chmod +x /usr/local/bin/depthai-viewer
}

# macOS specific installation
if [[ $(uname -s) == "Darwin" ]]; then
  echo "_____________________________"
  echo "Calling macOS_installer.sh"
  echo "_____________________________"
  echo "Running macOS installer."

  echo "Installing global dependencies."
  bash -c "$(curl -fL https://raw.githubusercontent.com/luxonis/depthai-python/refs/heads/feat/install-rework/docs/install_dependencies.sh)"

  echo "Upgrading brew."
  brew update
  brew install cmake

  # Install Python 3.10 and dependencies if requested
    echo "Installing Python 3.10"
    brew install python@3.10 
    
    PYTHONPATH=$(which python3.10)

  # pip does not have pyqt5 for arm
  if [[ $(uname -m) == 'arm64' ]]; then
    echo "Installing pyqt5 with homebrew."
    brew install pyqt@5
  fi

  create_venv



  "$VENV_PATH/bin/python" -m pip install --upgrade pip setuptools wheel numpy
  # If on ARM, set the PYTHONPATH to include the Homebrew installation path
  if [[ $(uname -m) == 'arm64' ]]; then
    if [[ ":$PYTHONPATH:" == *":/opt/homebrew/lib/python3.10/site-packages:"* ]]; then
      echo "/opt/homebrew/lib/python3.10/site-packages already in PYTHONPATH"
    else
      export "PYTHONPATH=/opt/homebrew/lib/python3.10/site-packages:"$PYTHONPATH
      echo "/opt/homebrew/lib/python3.10/site-packages added to PYTHONPATH"
    fi
  else
   "$VENV_PATH/bin/python" -m  pip install pyqt5
  fi

  install_depthai

# Linux specific installation
elif [[ $(uname -s) == "Linux" ]]; then
  echo "_____________________________"
  echo "Calling linux_installer.sh"
  echo "_____________________________"

  echo "Updating sudo-apt."
  sudo apt-get update
  echo "Running Linux installer."

  echo "Installing global dependencies."
  sudo apt-get install -y "${linux_pkgs[@]}"

  create_venv
  echo "Creating udev rules..."
  echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="03e7", MODE="0666"' | sudo tee /etc/udev/rules.d/80-movidius.rules
  sudo udevadm control --reload-rules && sudo udevadm trigger

  install_depthai

# Catch all other unsupported OS
else
  echo "Error: Host $(uname -s) not supported."
  exit 99
fi

# Final success message
echo "Installation complete successfully"
echo -e '\n\n:::::::::::::::: INSTALLATION COMPLETE ::::::::::::::::\n'
echo -e '\nTo run demo app, write **depthai-viewer** in terminal.'
