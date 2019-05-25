###
Work In Progress ... 🙃


NordVPNKrunner Build Instructions
----------------------
```
git clone https://github.com/alex1701c/NordVPNKrunner  
cd NordVPNKrunner/
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=MYPREFIX ..
make
make install
```
Restart krunner to load the runner (in a terminal type: `kquitapp5 krunner;kstart5 krunner` )


### Syntax Examples:

`vpn` Shows status and connect/disconnect options  
`vpn de` Connect to Germany instead of default if disconnected  
`vpn us 3335`, Connect to United States with server number 3335, the space is optional  
`vpn reconnect us` Disconnect and connect to US. If the current server is in the US, the exact same server is chosen.  
`vpn d` / `vpn disconnect` If connected the disconnect option is shown first  

#### Settings:

Valid keys are:

* `default` Default country/server code to connect to e.g. DE or US2921, default value is US.
* `icon` The path of the icon used for Krunner and the Notifications.
* `source` Command used to fetch the status from NordVPN, the default one is `nordvpn status`. This is useful if you have the issue that the NordVPN CLI is sometimes very slow when displaying the status. Instead of executing the slow command you can fetch the data from a file that gets regulary updated. I personally use this in combination with the Command Output Plasmoid, which executes `nordvpn status | tee <path> | grep 'Status'` and the Krunner plugin executed `cat <path>`.
* `script` Command that gets executed after the connection is changed(disconnect, connect, reconnect), the default is empty. This can be used to update a file(as in source explained). Krunner detaches from the process.
* `clean_history` Remove commands from the Krunner history that were used to change the settings. The default value is true.
 
`vpn set defaults` /  `vpn set * default` Set all settings to their original value  
`vpn set icon default` Set icon to default value  
`vpn set icon script default` Set icon and script to default value  
`vpn set icon /home/alex/icon.png ` Set value of icon to provided path, everything after `icon ` gets written in config  
