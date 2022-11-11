#!/bin/bash -i

echo '\nRunning Linux installer.'
echo "Upgrading sudo-apt."

sudo apt-get update

# clone depthai form git

if [ -d "$WORKING_DIR" ]; then
   echo "Demo app already downloaded. Checking out main and updating."

else
   echo "Downloading demo app."
   git clone https://github.com/luxonis/depthai.git "$WORKING_DIR"
fi

cd "$WORKING_DIR"
git checkout demo_app_installation_v2
git pull

# install python 3.10
echo "installing python 3.10"

sudo add-apt-repository ppa:deadsnakes/ppa
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
# run launcher
echo '\n>>>>>>>>>>>> RUNNING LAUNCHER <<<<<<<<<<<<<<<<<<<\n'
python3.10 "$WORKING_DIR/launcher/launcher.py" -r "$WORKING_DIR"
