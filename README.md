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
![Connect to default country](https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/screenshots/connect_default.png)
#### The notification if the Status is selected and the user is connected
![Notification if Status: Connected is selected](https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/screenshots/connected_notification.png)
#### Reconnect to another country and specify the server
![Reconnect to other county with specific server](https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/screenshots/reconnect_other_country_with_server.png)
#### Configure settings
![Configure settings in GUI](https://raw.githubusercontent.com/alex1701c/NordVPNKrunner/master/screenshots/settings_dialog.png)

### Syntax Examples:

`vpn` Shows status and connect/disconnect options  
`vpn de` Connect to Germany instead of default if disconnected  
`vpn us 3335`, Connect to United States with server number 3335, the space is optional  
`vpn reconnect us` Disconnect and connect to US. If the current server is in the US, the exact same server is chosen.  
`vpn d` / `vpn disconnect` If connected the disconnect option is shown first  

#### Settings:

*All these settings affect the Krunner plugin but not the actual settings of NordVPN*

Valid keys are:

* `default` Default country/server code to connect to e.g. DE or US2921, default value is US.
* `icon` The path of the icon used for Krunner and the Notifications.
* `source` Command used to fetch the status from NordVPN, the default one is `nordvpn status`. This is useful if you have the issue that the NordVPN CLI is sometimes very slow when displaying the status. Instead of executing the slow command you can fetch the data from a file that gets regularly updated. I personally use this in combination with the Command Output Plasmoid, which executes `nordvpn status | tee <path> | grep 'Status'` and the Krunner plugin executed `cat <path>`.
* `script` Command that gets executed after the connection is changed(disconnect, connect, reconnect), the default is empty. This can be used to update a file(as in source explained). Krunner detaches from the process.
* `clean_history` Remove commands from the Krunner history that were used to change the settings. The default value is true.
* `notify` Send notifications if the connection changes, default is true. 
* `status` Format for the text of status option. %st represents the status like "Status: Connected", %c the country abbreviation like "US" and %s the number of the current server. The default value is "%st".
* `status_keys` Set the keys for the status notification. They are used to filter the information from the NordVPN CLI. Valid keys are: `Status, Current server, Country, City, Your new IP, Current protocol, Transfer, Uptime or *`. They need to be separated by a | character. The keys you set are case insensitive and they just need to be contained in the ones from the list, for example "ip" selects the line "Your new IP".  The default value is "Status|Current server|Transfer|IP". 
    
`vpn set defaults` /  `vpn set * default` Set all settings to their original value  
`vpn set icon default` Set icon to default value  
`vpn set icon script default` Set icon and script to default value  
`vpn set icon /home/alex/icon.png ` Set value of icon to provided path, everything after `icon ` gets written in config  
