# NordVPNKrunner Build Instructions

Required Dependencies on Debian/Ubuntu:  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext libnotify-bin`

The easiest way to install is:  
`curl https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/install.sh | bash`

Or you can do it manually:

```
git clone https://github.com/alex1701c/NordVPNKrunner  
cd NordVPNKrunner/
mkdir build
cd build
cmake -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins` ..
make
sudo make install
```
Restart krunner to load the runner (in a terminal type: `kquitapp5 krunner;kstart5 krunner` )

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
`vpn de` Connect to Germany instead of default if disconnected  
`vpn us 3335`, Connect to United States with server number 3335, the space is optional  
`vpn reconnect us` Disconnect and connect to US. If the current server is in the US, the exact same server is chosen.  
`vpn d` / `vpn disconnect` If connected the disconnect option is shown first  

#### Settings:

*All these settings affect the Krunner plugin but not the actual settings of NordVPN*

Valid keys are:

* `Default connection target` Default country/server code to connect to e.g. DE or US2921, default value is US.
* `Status inside Krunner` Format for the text of status option. All the options are prefixed with a % character.
The keys are the same as in the NordVPN CLi but the spaces have ben removed. To get the key and value you have to type the key in uppercase letters e.g.`%COUNTRY`,
if you only want to get the value the key sould be lowercase like `%yournewip`. The live preview of a status shuld help with the formatting.
* `Icon` File chooser for the icon used in Krunner and the Notifications.
* `Source for the status` Command used to fetch the status from NordVPN, the default one is `nordvpn status`. This is useful if you have the issue that the NordVPN CLI is sometimes very slow when displaying the status.
Instead of executing the slow command you can fetch the data from a file that gets regularly updated. I personally use this in combination with the Command Output Plasmoid,
which executes `nordvpn status | tee <path> | grep 'Status'` and the Krunner plugin executed `cat <path>`.
* `Change script` Command that gets executed after the connection is changed(disconnect, connect, reconnect), the default is empty. This can be used to update a file(as in source explained).
* `Clean History` Remove disconnect command from history.
* `Notify` Send notifications if the connection changes, default is true. 
* `Status Keys` Set the keys for the status notification. They are used to filter the information from the NordVPN CLI.
