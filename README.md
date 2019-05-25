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

`vpn` Shows status and connect/disconnect options.  
`vpn de` Connect to Germany instead of default if disconnected.  
`vpn us 3335`, Connect to United States with server number 3335, the space is optional.  
`vpn reconnect us` Disconnect and connect to US. If the current server is in the US, the exact same erver is chosen.  
`vpn d`/`vpn disconnect` If connected the dosconnect option is shown first.  

