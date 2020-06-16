#!/bin/bash
python3 -m pip install -U twine

for file in depthai-*whl; do
  echo "Uploading $file"
  python3 -m twine upload --repository-url "$PYPI_SERVER" --username $PYPI_USER --password $PYPI_PASSWORD $file
done