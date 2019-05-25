git clone https://github.com/alex1701c/NordVPNKrunner
cd NordVPNKrunner/
mkdir build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make
sudo make install

kquitapp5 krunner 2> /dev/null || echo "No krunner instance has been running."
kstart5 --windowclass krunner krunner > /dev/null 2>&1 &

echo "Installation finished !";