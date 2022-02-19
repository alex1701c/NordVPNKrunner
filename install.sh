#!/bin/bash

# Exit if something fails
set -e

if [[ $(basename "$PWD") != "NordVPNKrunner"* ]];then
    git clone https://github.com/alex1701c/NordVPNKrunner
    cd NordVPNKrunner/
fi

mkdir -p build
cd build

cmake -DKDE_INSTALL_QTPLUGINDIR=`kf5-config --qt-plugins` -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

sudo make install
sudo curl https://s1.nordcdn.com/nordvpn/media/1.170.0/images/global/favicon/apple-touch-icon-57x57.png -o /usr/share/pixmaps/nordvpn.png

if pgrep -x krunner > /dev/null
then
    kquitapp5 krunner
fi

echo "Installation finished !";
