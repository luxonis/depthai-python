#!/bin/bash -i

echo $'\n\n:::::::::: RUNNING PYTHON DEPENDENCY INSTALLER ::::::::::\n'

cd "$HOME/depthai_demo_app"

# only on mac silicon point PYTHONPATH to pyqt5 installation via homebrew
if [[ $(uname -m) == 'arm64' ]]; then
  if [[ $(uname -s) == "Darwin" ]]; then
    if [[ ":$PYTHONPATH:" == *":/opt/homebrew/lib/python3.10/site-packages:"* ]]; then
      echo "/opt/homebrew/lib/python3.10/site-packages already in PYTHONPATH"
    else
      export PYTHONPATH=/opt/homebrew/lib/python3.10/site-packages:$PYTHONPATH
      echo "/opt/homebrew/lib/python3.10/site-packages added to PYTHONPATH"
    fi
  fi
fi


if [[ ":$PATH:" == *":$WORKING_DIR:"* ]]; then
  echo "$WORKING_DIR is already in PATH"
else
  export PATH="$PATH:$WORKING_DIR"
  echo "$WORKING_DIR added to PATH"
fi

# enable using demo_app to start the demo. its a script in the project directory
echo 'export PATH=$PATH:"$WORKING_DIR"' >> "$HOME"/.bashrc

if which pyenv ; then
  echo "installing python dependencies."
   # install python 3.10.8
   if [ ! -d "$HOME/.pyenv/versions/3.10.8" ]; then
     echo "installing python 3.10.8"
     pyenv install 3.10.8
   fi
   if [ ! -d "$HOME/.pyenv/versions/3.10.8/envs/demo_app_venv" ]; then
     pyenv virtualenv 3.10.8 demo_app_venv
  fi
   pyenv activate demo_app_venv
   python install_requirements.py

   echo $'\n\n:::::::::::::::: INSTALATION COMPLETED ::::::::::::::::\n'
   echo $'\nTo run demo app write <run_demo> in terminal.'
   read -rsp $'Press ANY KEY to finish and run the demo app...\n' -n1 key
   echo "STARTING DEMO APP."
   python depthai_demo.py -usbs usb2

else
  echo "Pyenv command does not work, pyenv setup was not successful."
  exit 99
fi