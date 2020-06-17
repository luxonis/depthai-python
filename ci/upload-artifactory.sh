#!/bin/bash

export PATH_PREFIX=luxonis-python-snapshot-local/depthai

curl -fL https://getcli.jfrog.io | sh
./jfrog rt config --url=$ARTIFACTORY_URL --user=$ARTIFACTORY_USER --password=$ARTIFACTORY_PASS
./jfrog rt u "depthai*" "$PATH_PREFIX/$GITHUB_SHA/"
