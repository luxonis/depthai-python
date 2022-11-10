#!/bin/bash -i

# global vars
PROFILE="$HOME/.profile"
BASH_PROFILE="$HOME/.bash_profile"
BASH_LOGIN="$HOME/.bash_login"
BASH_PATHS=( PROFILE BASH_PROFILE BASH_LOGIN )

# shellcheck disable=SC2016
PYENV_CMD1='export PYENV_ROOT="$HOME/.pyenv"'
# shellcheck disable=SC2016
PYENV_CMD2='command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"'
# shellcheck disable=SC2016
PYENV_CMD3='eval "$(pyenv init -)"'

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
}

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
cp install_python_dependencies.sh "$WORKING_DIR"
cd "$WORKING_DIR"
git checkout demo_app_installation
git pull

# install pyenv, python 3.10 and python dependencies
brew update
echo "installing pyenv, virtualenv"
brew install pyenv pyenv-virtualenv

# pip does not have pyqt5 for arm
if [[ $(uname -m) == 'arm64' ]]; then
  echo "Installing pyqt5 with homebrew."
  brew install pyqt@5
fi

# pyenv installation guide from here: https://github.com/pyenv/pyenv

# Bash warning: There are some systems where the BASH_ENV variable is configured to point to .bashrc. On such systems, you should almost certainly put the eval "$(pyenv init -)" line into .bash_profile, and not into .bashrc
if [ "$BASH_ENV" = "$HOME/.bashrc" ]; then
   write_in_file "$BASH_PROFILE"
else
   write_in_file "$HOME/.bashrc"
fi

#  if you have ~/.profile, ~/.bash_profile or ~/.bash_login, add the commands there as well. If you have none of these, add them to ~/.profile.
# case none of them exist
if [ ! -f "$PROFILE" ] && [ ! -f "$BASH_PROFILE" ] && [ ! -f "$BASH_LOGIN" ] ; then
   echo "exporting pyenv setup to $PROFILE"
   write_in_file "$PROFILE"
elif [ -f "$PROFILE" ]; then
   echo "exporting pyenv setup to $PROFILE"
   write_in_file "$PROFILE"
elif [ -f "$BASH_PROFILE" ]; then
   echo "exporting pyenv setup to $BASH_PROFILE"
   write_in_file "$BASH_PROFILE"
elif [ -f "$BASH_LOGIN" ]; then
   echo "exporting pyenv setup to $BASH_LOGIN"
   write_in_file "$BASH_LOGIN"
fi

./install_python_dependencies.sh