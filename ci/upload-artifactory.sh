#!/bin/bash

export PATH_PREFIX=luxonis-myriad-snapshot-local/depthai-device-side

curl -fL https://getcli.jfrog.io | sh
./jfrog rt config --url=$ARTIFACTORY_URL --user=$ARTIFACTORY_USER --password=$ARTIFACTORY_PASS
./jfrog rt u "depthai-0.0.1-cp*" "$PATH_PREFIX/$GITHUB_SHA/"
