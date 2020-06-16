#!/bin/bash

export BUILD_NAME=gitlabci-depthai-device-side-snapshot-build
export PATH_PREFIX=luxonis-myriad-snapshot-local/depthai-device-side

curl -fL https://getcli.jfrog.io | sh
echo "[TEST] URL: $ARTIFACTORY_URL ; USER: $ARTIFACTORY_USER ; PASS $ARTIFACTORY_PASS"
./jfrog rt config --url=$ARTIFACTORY_URL --user=$ARTIFACTORY_USER --password=$ARTIFACTORY_PASS
./jfrog rt u "depthai-0.0.1-cp*" "$PATH_PREFIX/$GITHUB_SHA/"
