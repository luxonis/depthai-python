#!/bin/bash
set -e

VENV_PATH=".local/share/virtualenvs"

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

if [[ $(uname -s) == "Darwin" ]]; then
  echo _____________________________
  echo "Calling macOS_installer.sh"
  echo _____________________________
  echo "Running macOS installer."

  echo "Installing global dependencies."
  bash -c "$(curl -fL https://raw.githubusercontent.com/luxonis/depthai-python/refs/heads/feat/install-rework/docs/install_dependencies.sh)"

  echo "Upgrading brew."
  brew update

  # clone depthai form git
  if [ -d "$DEPTHAI_DIR" ]; then
     echo "Demo app already downloaded. Checking out main and updating."
  else
     echo "Downloading demo app."
     git clone https://github.com/luxonis/depthai.git "$DEPTHAI_DIR"
  fi
  cd "$DEPTHAI_DIR"
  git fetch
  git checkout main
  git pull

  # install python 3.10 and python dependencies
  brew update

  if [ "$install_python" == "true" ]; then
    echo "installing python 3.10"
    brew install python@3.10
    python_executable=$(which python3.10)
  fi

  # pip does not have pyqt5 for arm
  if [[ $(uname -m) == 'arm64' ]]; then
    echo "Installing pyqt5 with homebrew."
    brew install pyqt@5
  fi

  # create python virtual environment
  echo "Creating python virtual environment in $VENV_DIR"
  echo "$python_executable"
  "$python_executable" -m venv "$VENV_DIR"
  # activate environment
  source "$VENV_DIR/bin/activate"
  python -m pip install --upgrade pip

  # install launcher dependencies
  # only on mac silicon point PYTHONPATH to pyqt5 installation via homebrew, otherwise install pyqt5 with pip
  if [[ $(uname -m) == 'arm64' ]]; then
    if [[ ":$PYTHONPATH:" == *":/opt/homebrew/lib/python3.10/site-packages:"* ]]; then
      echo "/opt/homebrew/lib/python$nr_1.$nr_2/site-packages already in PYTHONPATH"
    else
      export "PYTHONPATH=/opt/homebrew/lib/python$nr_1.$nr_2/site-packages:"$PYTHONPATH
      echo "/opt/homebrew/lib/pythonv$nr_1.$nr_2/site-packages added to PYTHONPATH"
    fi
  else
    pip install pyqt5
  fi

  pip install packaging

elif [[ $(uname -s) == "Linux" ]]; then
  echo _____________________________
  echo "Calling linux_installer.sh"
  echo _____________________________

  echo "Updating sudo-apt."
  sudo apt-get update
  echo -e '\nRunning Linux installer.'


  echo "Installing global dependencies."
  sudo apt-get install -y "${linux_pkgs[@]}"

  echo "Creating python virtual environment in $VENV_PATH"


if [ ! -d "$VENV_PATH" ]; then
  echo "Creating virtual environment at $VENV_PATH"
  mkdir -p "$VENV_PATH"
  python3 -m venv "$VENV_PATH"
fi

  echo "Installing viewer in virtual environment..."
  "$VENV_PATH/bin/python" -m pip install --upgrade pip
  "$VENV_PATH/bin/python" -m pip install depthai-viewer packaging

  echo "Creating udev rules..."
  echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="03e7", MODE="0666"' | sudo tee /etc/udev/rules.d/80-movidius.rules
  sudo udevadm control --reload-rules && sudo udevadm trigger

  echo -e '#!/bin/bash\nsource .local/share/virtualenvs\nexec depthai-viewer "$@"' | sudo tee /usr/local/bin/depthai-viewer > /dev/null
  sudo chmod +x /usr/local/bin/depthai-viewer

else
  echo "Error: Host $(uname -s) not supported."
  exit 99
fi

echo "Creating virtual environment at $VENV_PATH"

if [ ! -d "$VENV_PATH" ]; then
  echo "Creating virtual environment at $VENV_PATH"
  mkdir -p "$VENV_PATH"
  python3 -m venv "$VENV_PATH"
fi



echo "Installation complete successfully"
echo -e '\n\n:::::::::::::::: INSTALATION COMPLETE ::::::::::::::::\n'
echo -e '\nTo run demo app write **depthai-viewer** in terminal.'
