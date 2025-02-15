#!/bin/sh

printf 'What is your project named? › '
read PROJECT_NAME

printf 'Would you like to create a venv with all dependencies? [Y/n] '
while true; do
    read -p '> ' yn
    case $yn in
        [Yy]* ) VENV=true; break;;
        [Nn]* ) VENV=false; break;;
        "" ) VENV=true; break;;
        * ) echo "Please answer [Y]es or [n]o.";;
    esac
done
printf 'Would you like to init a git repository? [Y/n] '
while true; do
    read -p '> ' yn
    case $yn in
        [Yy]* ) GIT=true; break;;
        [Nn]* ) GIT=false; break;;
        "" ) GIT=true; break;;
        * ) echo "Please answer [Y]es or [n]o.";;
    esac
done

isNIxos=$(uname -a | grep -c 'NixOS')

if [ "$isNIxos" -eq 1 ]; then
    PYTHON_PACKAGES="robothub blobconverter requests==2.26.0 numpy>=1.21.4,<2.0.0 opencv-contrib-python==4.5.5.62 pyqt5>5,<5.15.6 scipy Qt.py"
elif [[ $(uname -m) == 'arm64' ]]; then
    PYTHON_PACKAGES="opencv-python==4.8.0.74 robothub blobconverter requests==2.26.0 numpy>=1.21.4,<2.0.0 opencv-contrib-python==4.5.5.62 scipy Qt.py"
else
    PYTHON_PACKAGES="opencv-python==4.8.0.74 robothub blobconverter requests==2.26.0 numpy>=1.21.4,<2.0.0 opencv-contrib-python==4.5.5.62 pyqt5>5,<5.15.6 scipy Qt.py"
fi

printf '===============Creating project================\n'
printf 'Name: %s\n' "$PROJECT_NAME"
printf 'Virtual environment: %s\n' "$VENV"
printf 'Git: %s\n' "$GIT"

mkdir -p "$PROJECT_NAME"
cd "$PROJECT_NAME" || exit 1

if [ "$VENV" = true ]; then
    printf '===============Creating virtual environment================\n'
    python3 -m venv .venv
    . .venv/bin/activate
    python3 -m pip install --upgrade pip


    if [[ $(uname -s) == "Darwin" ]]; then
      if [[ $(uname -m) == 'arm64' ]]; then
        echo "Mac tweek installing pyqt@5 with brew"
        brew install pyqt@5
      fi
    fi

    pip install $PYTHON_PACKAGES
    export CMAKE_MAKE_PROGRAM=/usr/bin/make
    pip install depthai --find-links https://artifacts.luxonis.com/artifactory/luxonis-python-snapshot-local/ 2>/dev/null
fi

printf '===============Creating project files================\n'

curl https://raw.githubusercontent.com/luxonis/depthai-tutorials/refs/heads/master/1-hello-world/hello_world.py > main.py

if [ "$GIT" = true ]; then
    printf '===============Initializing git================\n'
    git init
    echo "
__pycache__/
.venv/
.vscode/
build/
dist/
    " >> .gitignore
fi

if [ "$isNIxos" -eq 1 ]; then
    echo "d2l0aCBpbXBvcnQgPG5peHBrZ3M+IHt9OwoKbGV0IAogIHB5dGhvbiA9IGxldAogICAgcGFja2FnZU92ZXJyaWRlcyA9IHNlbGY6IHN1cGVyOiB7CiAgICAgIG9wZW5jdjQgPSBzdXBlci5vcGVuY3Y0Lm92ZXJyaWRlIHsKICAgICAgICBlbmFibGVHdGsyID0gdHJ1ZTsKICAgICAgICBndGsyID0gcGtncy5ndGsyOwogICAgICB9OwogICAgfTsKICBpbiBwa2dzLnB5dGhvbjMub3ZlcnJpZGUgeyBpbmhlcml0IHBhY2thZ2VPdmVycmlkZXM7IHNlbGYgPSBweXRob247IH07CgppbiBzdGRlbnYubWtEZXJpdmF0aW9uIHsKICBMRF9MSUJSQVJZX1BBVEggPSAiJHtwa2dzLnN0ZGVudi5jYy5jYy5saWJ9L2xpYi86L3J1bi9vcGVuZ2wtZHJpdmVyL2xpYi8iOwogIG5hbWUgPSAiaW1wdXJlUHl0aG9uRW52IjsKICBidWlsZElucHV0cyA9IFsKICAgIHB5dGhvbjNQYWNrYWdlcy52aXJ0dWFsZW52CiAgICBweXRob24zUGFja2FnZXMucHlxdDUKICAgIHB5dGhvbjNQYWNrYWdlcy5weXVzYgogICAgZ2xpYmMKICAgIGd0azMKICAgIG9wZW5jdiAKICAgIChweXRob24zLndpdGhQYWNrYWdlcyAocHM6IHdpdGggcHM7IFsKICAgICAgbWF0cGxvdGxpYgogICAgICBudW1weQogICAgICBzY2lweQogICAgICBnbnVyZWFkbGluZQogICAgICAob3BlbmN2NC5vdmVycmlkZSB7CiAgICAgICAgZW5hYmxlR3RrMyA9IHRydWU7CiAgICAgIH0pCiAgICBdKSkKICBdOwoKICBzaGVsbEhvb2sgPSAnJwogICAgZXhwb3J0IExEX0xJQlJBUllfUEFUSD0ke3N0ZGVudi5jYy5jYy5saWJ9L2xpYi86L3J1bi9vcGVuZ2wtZHJpdmVyL2xpYi8KICAgIGV4cG9ydCBRVF9QTFVHSU5fUEFUSD0ke3F0NS5xdGJhc2V9LyR7cXQ1LnF0YmFzZS5xdFBsdWdpblByZWZpeH0gCiAgICBleHBvcnQgU09VUkNFX0RBVEVfRVBPQ0g9JChkYXRlICslcykKICAgIGV4cG9ydCBMQU5HPWVuX1VTLlVURi04CQogICcnOwp9Cg==" | base64 -d > shell.nix

fi
printf '===============To start the project================\n'
printf 'To start the project, run:\n'

printf 'cd %s\n' "$PROJECT_NAME"
if [ "$isNIxos" -eq 1 ]; then
    printf 'nix-shell\n'
fi
if [ "$VENV" = true ]; then
    printf 'source .venv/bin/activate\n'
fi

printf 'python3 main.py\n'
printf 'For more exmaple projects see docs: https://docs.luxonis.com/software/depthai/hello-world/'
