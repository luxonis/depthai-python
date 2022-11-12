#!/bin/bash -i

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

# run launcher
python "$WORKING_DIR/launcher/launcher.py" -r "$WORKING_DIR"
