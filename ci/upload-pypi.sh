#!/bin/bash
python3 -m pip install -U twine

echo "[TEST] PYPI_SERVER: $PYPI_SERVER ; PYPI_USER: $PYPI_USER ; PYPI_PASSWORD: $PYPI_PASSWORD"
for file in wheelhouse/audited/*.whl; do
  echo "Uploading $file"
  python3 -m twine upload --repository-url "$PYPI_SERVER" --username $PYPI_USER --password $PYPI_PASSWORD $file
done