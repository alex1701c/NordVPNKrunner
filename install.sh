#!/bin/bash

# Exit if something fails
set -e

if [[ $(basename "$PWD") != "NordVPNKrunner"* ]];then
    git clone https://github.com/alex1701c/NordVPNKrunner
    cd NordVPNKrunner/
fi

mkdir -p build
cd build

cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

sudo make install
sudo curl https://s1.nordcdn.com/nordvpn/media/1.170.0/images/global/favicon/apple-touch-icon-57x57.png -o /usr/share/pixmaps/nordvpn.png

kquitapp5 krunner 2> /dev/null
kstart5 --windowclass krunner krunner > /dev/null 2>&1 &

echo "Installation finished !";