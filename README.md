# NordVPNKrunner 

This plugin allows you to run NordVPN commands from KRunner.
Additionally it supports configurable status notifications and it use the 
KDE KNotifications API.

If you like this plugin please consider rating it at the KDE Store https://store.kde.org/p/1311130/.  

To install this plugin checkout the deb/rpm packages on the release page or build from 
source as explained below.


### Screenshots:
#### Connect to the default country
![Connect to default country](https://raw.githubusercontent.com/alex1701c/Screenshots/master/NordVPNKrunner/connect_default.png)
#### The notification if the Status is selected and the user is connected
![Notification if Status: Connected is selected](https://raw.githubusercontent.com/alex1701c/Screenshots/master/NordVPNKrunner/connected_notification.png)
#### Reconnect to another country and specify the server
![Reconnect to other county with specific server](https://raw.githubusercontent.com/alex1701c/Screenshots/master/NordVPNKrunner/reconnect_other_country_with_server.png)
#### Configure settings
![Configure settings in GUI](https://raw.githubusercontent.com/alex1701c/Screenshots/master/NordVPNKrunner/formatting_example.png)

### Syntax Examples:

`vpn` Shows status and connect/disconnect options  
`vpn de` Connect to Germany
`vpn us 3335`, Connect to United States with server number 3335, the space is optional  
`vpn reconnect` Reconnect to the current server, this is useful if you have issues with your connection  
`vpn d` / `vpn disconnect` If connected the disconnect option is shown first.  

#### Settings:

*All these settings affect the Krunner plugin but not the actual settings of NordVPN*

* `Default connection target` Default country/server code to connect to e.g. DE or US2921, default value is US.
* `Status inside Krunner` Format for the text of status option. All the options are prefixed with a % character.
The keys are the same as in the NordVPN CLi but the spaces have ben removed. To get the key and value you have to type the key in uppercase letters e.g.`%COUNTRY`,
if you only want to get the value the key should be lowercase like `%yournewip`. The live preview of a status should help with the formatting.
* `Notify` Send notifications if the connection changes, if you use this you might want to disable the notifications
of the nordvpn cli `nordvpn set notify disabled`
* `Status Keys` Set the keys for the status notification. They are used to filter the information from the NordVPN CLI.
The checkbox `Display IP Address by making ...` fetches the IP address online so that you can be sure that you are connected.  

### Build Instructions

Required Dependencies on Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext libkf5notifications-dev libkf5kcmutils-dev`

openSUSE  
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel
 kservice-devel krunner-devel gettext-tools kconfigwidgets-devel knotifications-devel kcmutils-devel`  

Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel gettext kf5-knotifications-devel kf5-kcmutils-devel`  

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/install.sh | bash`  

Or you can do it manually:

```
git clone https://github.com/alex1701c/NordVPNKrunner  
cd NordVPNKrunner/
mkdir build
cd build
cmake -DKDE_INSTALL_QTPLUGINDIR=`kf5-config --qt-plugins` -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
sudo make install
kquitapp5 krunner;kstart5 krunner
```
