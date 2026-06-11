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

if ! command -v ak >/dev/null 2>&1; then
    curl -fsSL https://raw.githubusercontent.com/artifact-keeper/artifact-keeper-cli/main/install.sh | sh
fi

export AK_INSTANCE="${AK_INSTANCE:-${AK_URL}}"
export AK_NO_INPUT="${AK_NO_INPUT:-1}"

ak artifact push "$AK_REPO" 'wheelhouse/audited/*'
