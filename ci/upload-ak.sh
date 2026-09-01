#!/bin/bash
set -euo pipefail

: "${AK_URL:?AK_URL must be set}"
: "${AK_TOKEN:?AK_TOKEN must be set}"

case "$1" in
    --snapshot)
        AK_REPO="luxonis-python-snapshot-local"
        ;;
    --release)
        AK_REPO="luxonis-python-release-local"
        ;;
    *)
        echo "Error: Unknown option $1"
        echo "Usage: $0 [--snapshot | --release]"
        exit 1
        ;;
esac

AK_BASE_URL="${AK_URL%/}"
AK_PYPI_REPOSITORY_URL="${AK_BASE_URL}/pypi/${AK_REPO}/"
AK_PYPI_SIMPLE_URL="${AK_BASE_URL}/pypi/${AK_REPO}/simple/depthai/"

python3 -m pip install -U pip
python3 -m pip install -U twine
python3 -m pip install -U packaging

shopt -s nullglob
wheels=(wheelhouse/audited/*.whl)
if [ "${#wheels[@]}" -eq 0 ]; then
    echo "No wheels found in wheelhouse/audited/"
    exit 1
fi

for wheel in "${wheels[@]}"; do
    echo "Uploading ${wheel}"
    python3 -m twine upload \
        --repository-url "${AK_PYPI_REPOSITORY_URL}" \
        --username "__token__" \
        --password "${AK_TOKEN}" \
        "${wheel}"
done

index_html="$(mktemp)"
trap 'rm -f "${index_html}"' EXIT
curl -fsSL "${AK_PYPI_SIMPLE_URL}" -o "${index_html}"

for wheel in "${wheels[@]}"; do
    wheel_name="$(basename "${wheel}")"
    if ! grep -Fq "${wheel_name}" "${index_html}"; then
        echo "Uploaded wheel is missing from the PyPI simple index: ${wheel_name}"
        exit 1
    fi
done
