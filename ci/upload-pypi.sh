#!/bin/bash
pip install -U twine

for file in depthai-*whl; do
  echo "Uploading $file"
  twine upload --repository-url $PYPI_SERVER --username $PYPI_USER --password $PYPI_PASSWORD $file
done