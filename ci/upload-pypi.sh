#!/bin/bash
python3 -m pip install -U twine

# Uploads prebuilt binary distribution
for file in wheelhouse/audited/*.whl; do
    echo "Uploading $file"
    python3 -m twine upload --repository-url "$PYPI_SERVER" --username "$PYPI_USER" --password "$PYPI_PASSWORD" "$file"
done

# Uploads zip source distribution
for file in wheelhouse/audited/*.zip; do
    echo "Uploading $file"
    python3 -m twine upload --repository-url "$PYPI_SERVER" --username "$PYPI_USER" --password "$PYPI_PASSWORD" "$file"
done

# Uploads tar.bz2 source distribution
for file in wheelhouse/audited/*.tar.bz2; do
    echo "Uploading $file"
    python3 -m twine upload --repository-url "$PYPI_SERVER" --username "$PYPI_USER" --password "$PYPI_PASSWORD" "$file"
done