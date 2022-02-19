#!/bin/bash

set -e
cd build

xargs echo "Uninstalling files: " < install_manifest.txt
xargs sudo rm < install_manifest.txt

if pgrep -x krunner > /dev/null
then
    kquitapp5 krunner
fi
