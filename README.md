# Ardupilot Gazebo Plugin & Models

## Requirements :

Ubuntu Xenial(16.04 LTS)

ArduPilot setup for SITL launch

Gazebo version 7.0


## Repository Structure : 

TBA

# Getting Started :
## How to Install :
I assume you already have Gazebo 7+ installed with ROS (or without).  
If you don't have it yet, install ROS with sudo apt install ros-kinetic-desktop-full
(follow instruction here http://wiki.ros.org/kinetic/Installation/Ubuntu).  

Or install directly gazebo8 from http://gazebosim.org/tutorials?tut=install_ubuntu  

libgazebo7-dev or libgazebo8-dev must be installed.

**For Gazebo 7**
````
sudo apt-get install libgazebo7-dev
````
OR

**Common :**
````
git clone https://github.com/BAmercury/ardupilot_gazebo
cd ardupilot_gazebo
git checkout gazebo7-ver
mkdir build
cd build
cmake ..
make -j4
sudo make install
````
Set Path of Gazebo Models / Worlds...
Open up .bashrc
````
sudo gedit ~/.bashrc
````
Copy & Paste Followings at the end of .bashrc file
````
source /usr/share/gazebo/setup.sh

export GAZEBO_MODEL_PATH=~/Documents/ardupilot_gazebo/gazebo_models
export GAZEBO_RESOURCE_PATH=~/ardupilot_gazebo/gazebo_worlds:${GAZEBO_RESOURCE_PATH}

export PATH=$PATH:$HOME/ardupilot/Tools/autotest
export PATH=/usr/lib/ccache:$PATH
````

Install is complete

## HELP

### How to Launch :  
Launch Ardupilot Software In the Loop Simulation for each vehicle.
On new terminal, Launch Gazebo with basic demo world.


**COPTER (3DR IRIS)**
````
On 1st Terminal(Launch Ardupilot SITL)
sim_vehicle.py -v ArduCopter -f gazebo-iris  -m --mav10 --map --console -I0

On 2nd Terminal(Launch Gazebo with demo 3DR Iris model)
gazebo --verbose iris_ardupilot.world
````

In addition, you can use any GCS that can connect to the Ardupilot locally or remotely(will require connection setup).
If MAVProxy Developer GCS is uncomfortable. Omit --map --console arguments out of SITL launch.

And use APMPlanner2 or QGroundControl instead.
(Possibly MissionPlanner but require Windows PC)

Local connection with APMPlanner2/QGroundControl is automatic, and easier to use.

For APMPlanner2

Download it from here http://firmware.eu.ardupilot.org/Tools/APMPlanner/
and launch it in terminal or run executable

````
apmplanner2
````

For QGroundControl

Download it from here and follow the installation guide.

https://donlakeflyer.gitbooks.io/qgroundcontrol-user-guide/en/download_and_install.html

## Multi-Vehicle simulation
This section explains how to connect any combination of multi-vehicles of ArduPilot

For the multi-vehicle connection, port number is increased by 10 per instance(#)
In SITL launch argument -I # of sim_vehicle.py 

-I 0 has FDM in/out ports of 9002/9003 / GCS connection UDP:14550

-I 1 has FDM in/out ports of 9012/9013 / GCS connection UDP:14560

-I 2 has FDM in/out ports of 9022/9023 / GCS connection UDP:14570

and so on...

You will need to edit your world for any combination of Rover, Plane, Copter, etc...

Additional Note for GCS Connection
You will also need to edit ArduPilot Parameter SYSID_THISMAV to be unique from one another for the GCS connection

### Example
Look simulation of 3 IRIS quadcopter at once from Jonathan Lopes Florêncio
https://www.youtube.com/watch?v=3c7EhVMaqKY&feature=youtu.be

## Troubleshooting

### Missing libArduPilotPlugin.so... etc

In case you see this message when you launch gazebo with demo worlds, check you have no error after sudo make install.  
If no error use "ls" on the install path given to see if the plugin is really here.  
If this is correct, check with "cat /usr/share/gazebo/setup.sh" the variable GAZEBO_PLUGIN_PATH. It should be the same as the install path. If not use "cp" to copy the lib to right path.

**For Example**

````
sudo cp -a /usr/lib/x86_64-linux-gnu/gazebo-7.0/plugins/ /usr/lib/x86_64-linux-gnu/gazebo-7/
````

Path mismatch is confirmed as ROS's glitch. It only happens with Gazebo 7
