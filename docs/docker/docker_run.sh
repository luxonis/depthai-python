#!/bin/bash

set -e

cmake -S . -B build -D DEPTHAI_BUILD_DOCS=ON -D DEPTHAI_PYTHON_BUILD_DOCS=ON

./docs/docker/update_docs.sh

when-changed -r docs/source -c "bash /app/docs/docker/update_docs.sh" &

python3 -m http.server --bind 0.0.0.0 8000 --directory build/docs/sphinx
