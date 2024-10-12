# NordVPNKrunner 

This plugin allows you to run NordVPN commands from KRunner.
Additionally, it supports configurable status notifications and it use the 
KDE KNotifications API.  
It supports both Plasma5 and Plasma6!

If you like this plugin please consider rating it at the KDE Store https://store.kde.org/p/1311130/.  

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
The keys are the same as in the NordVPN CLi but the spaces have been removed. To get the key and value you have to type the key in uppercase letters e.g.`%COUNTRY`,
if you only want to get the value the key should be lowercase like `%yournewip`. The live preview of a status should help with the formatting.
* `Notify` Send notifications if the connection changes, if you use this you might want to disable the notifications
of the nordvpn cli `nordvpn set notify disabled`
* `Status Keys` Set the keys for the status notification. They are used to filter the information from the NordVPN CLI.
The checkbox `Display IP Address by making ...` fetches the IP address online so that you can be sure that you are connected.  

### Build Instructions

#### Dependencies 

<details>
<summary><b>Debian/Ubuntu</b></summary>

Plasma5:  
```bash install-ubuntu-plasma5
sudo apt install git cmake extra-cmake-modules build-essential gettext libkf5runner-dev libkf5i18n-dev libkf5kcmutils-dev libkf5notifications-dev qtdeclarative5-dev libkf5dbusaddons-bin
```
Plasma6:  
```bash install-ubuntu-plasma6
sudo apt install git cmake extra-cmake-modules build-essential gettext libkf6runner-dev libkf6i18n-dev libkf6kcmutils-dev libkf6notifications-dev kf6-kdbusaddons
```

</details>

<details>
<summary><b>OpenSUSE</b></summary>

Plasma5:  
```bash install-opensuse-plasma5
sudo zypper install git cmake gettext-tools libQt5Network-devel extra-cmake-modules ki18n-devel krunner-devel kcmutils-devel knotifications-devel kdbusaddons-tools
```
Plasma6:  
```bash install-opensuse-plasma6
sudo zypper install git cmake gettext-tools qt6-network-devel kf6-extra-cmake-modules kf6-ki18n-devel kf6-krunner-devel kf6-knotifications-devel kf6-kdbusaddons-tools
```

</details>

<details>
<summary><b>Fedora</b></summary>

Plasma5:  
```bash install-fedora-plasma5
sudo dnf install git cmake gettext extra-cmake-modules kf5-ki18n-devel kf5-krunner-devel kf5-kcmutils-devel kf5-knotifications-devel
```
Plasma6:  
```bash install-fedora-plasma6
sudo dnf install git cmake gettext extra-cmake-modules kf6-ki18n-devel kf6-krunner-devel kf6-kcmutils-devel kf6-knotifications-devel
```

</details>

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/install.sh | bash`  

Or you can clone the repo manually and run the install script.
