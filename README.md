###
Work In Progress ... 🙃


Runner Template
----------------------

-- Build instructions --

cd /where/your/runner/is/installed
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=MYPREFIX .. 
make 
make install

(MYPREFIX is where you install your Plasma setup, replace it accordingly)

Restart krunner to load the runner (in a terminal type: `kquitapp krunner;krunner` )

