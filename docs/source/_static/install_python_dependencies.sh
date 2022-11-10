#!/bin/bash -i

cd "$HOME/depthai_demo_app"
export PYTHONPATH=/opt/homebrew/lib/python3.10/site-packages:$PYTHONPATH
export PYTHONPATH=/opt/homebrew/lib/python3.10/site-packages:$PYTHONPATH
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
   which python
   which python3
   pyenv activate demo_app_venv
   which python
   which python3
   python install_requirements.py
   python depthai_demo.py

else
  echo "Pyenv command does not work, pyenv setup was not successful."
  exit 99
fi