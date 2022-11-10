#!/bin/bash -i

# global vars
BASHRC="$HOME/.bashrc"

PYENV_CMD1='export PATH="$HOME/.pyenv/bin:$PATH"'
PYENV_CMD2='eval "$(pyenv init --path)"'
PYENV_CMD3='eval "$(pyenv init -)"'
PYENV_CMD4='eval "$(pyenv virtualenv-init -)"'

write_in_file () {
  # just make sure only strings are appended which are not in there yet
  if ! grep -Fxq "$PYENV_CMD1" "$1"
  then
    echo "$PYENV_CMD1" >> "$1"
  fi
  if ! grep -Fxq "$PYENV_CMD2" "$1"
  then
    echo "$PYENV_CMD2" >> "$1"
  fi
  if ! grep -Fxq "$PYENV_CMD3" "$1"
  then
    echo "$PYENV_CMD3" >> "$1"
  fi
  if ! grep -Fxq "$PYENV_CMD4" "$1"
  then
    echo "$PYENV_CMD3" >> "$1"
  fi
}

echo '\nRunning Linux installer.'
echo "Upgrading sudo-apt."

sudo apt-get update
sudo apt-get install make build-essential libssl-dev zlib1g-dev \
libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm \
libncursesw5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev libffi-dev liblzma-dev


# clone depthai form git

if [ -d "$WORKING_DIR" ]; then
   echo "Demo app already downloaded. Checking out main and updating."

else
   echo "Downloading demo app."
   git clone https://github.com/luxonis/depthai.git "$WORKING_DIR"
fi

cp ./install_python_dependencies.sh "$WORKING_DIR"
cd "$WORKING_DIR"
git checkout demo_app_installation
git pull

# install pyenv
echo "installing pyenv, virtualenv"

if [ ! -d "$HOME/.pyenv/" ]; then
  curl https://pyenv.run | bash
else
  echo "pyenv is already installed"
fi

# pip does not have pyqt5 for arm but for linux it is in the global dependencies
#if [[ $(uname -m) == 'arm64' ]]; then
#  echo "Installing pyqt5 with homebrew."
#  sudo apt-get install python3-pyqt5
#fi

# pyenv installation guide from here: https://brain2life.hashnode.dev/how-to-install-pyenv-python-version-manager-on-ubuntu-2004
write_in_file "$BASHRC"
# activate pyenv
# exec "$SHELL"