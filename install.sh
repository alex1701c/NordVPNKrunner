#!/bin/bash

# Exit if something fails
set -e

if [[ $(basename "$PWD") != "NordVPNKrunner"* ]];then
    git clone https://github.com/alex1701c/NordVPNKrunner
    cd NordVPNKrunner/
fi

mkdir -p build
cd build

echo "Downloading nordvon icon ..."
sudo curl https://s1.nordcdn.com/nordvpn/media/1.170.0/images/global/favicon/apple-touch-icon-57x57.png -o /usr/share/pixmaps/nordvpn.png

krunner_version=$(krunner --version | grep -oP "(?<=krunner )\d+")
if [[ "$krunner_version" == "6" ]]; then
    echo "Building for Plasma6"
    BUILD_QT6_OPTION="-DBUILD_WITH_QT6=ON"
else
    echo "Building for Plasma5"
    BUILD_QT6_OPTION=""
fi

cmake .. -DCMAKE_BUILD_TYPE=Release -DKDE_INSTALL_USE_QT_SYS_PATHS=ON $BUILD_QT6_OPTION
make -j$(nproc)
sudo make install

# KRunner needs to be restarted for the changes to be applied
if pgrep -x krunner > /dev/null
then
    kquitapp"$krunner_version" krunner
fi

echo "Installation finished!";
