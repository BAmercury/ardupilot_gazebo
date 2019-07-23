# Ardupilot Gazebo Plugin & Models

## Requirements :
Ubuntu Bionic (18.04 LTS)

Gazebo 9.0

ROS Melodic Morenia

## Usage :


````
git clone https://github.com/BAmercury/ardupilot_gazebo
cd ardupilot_gazebo
mkdir build
cd build
cmake ..
make -j4
sudo make install
````
Set Path of Gazebo Models
````
echo 'export GAZEBO_MODEL_PATH=~/ardupilot_gazebo/gazebo_models' >> ~/.bashrc
````

Set Path of Gazebo Worlds
````
export GAZEBO_RESOURCE_PATH=~/ardupilot_gazebo/gazebo_worlds:${GAZEBO_RESOURCE_PATH}
source ~/.bashrc
````

## HELP

How to Launch :  
Launch Ardupilot Software In the Loop Simulation for each vehicle.
On new terminal, Launch Gazebo with basic demo world.
ROVER

````
On 1st Terminal(Launch Ardupilot SITL)
sim_vehicle.py -v APMrover2 -f gazebo-rover  -m --mav10 --map --console -I0

On 2nd Termianal(Launch Gazebo with demo Rover model)
gazebo --verbose (Please Add if there is one.)

````
COPTER (3DR IRIS)
````
On 1st Terminal(Launch Ardupilot SITL)
sim_vehicle.py -v ArduCopter -f gazebo-iris  -m --mav10 --map --console -I0

On 2nd Terminal(Launch Gazebo with demo 3DR Iris model)
gazebo --verbose iris_ardupilot.world
````

PLANE
````
On 1st Terminal(Launch Ardupilot SITL)
sim_vehicle.py -v ArduPlane -f gazebo-zephyr  -m --mav10 --map --console -I0

On 2nd Terminal(Launch Gazebo with demo Zephyr flying wing model)
gazebo --verbose zephyr_ardupilot_demo.world
````

In addition, you can use any GCS of Ardupilot locally or remotely(will require connection setup).
If MAVProxy Developer GCS is uncomfortable. Omit --map --console arguments out of SITL launch. 

And use APMPlanner 2 or QGroundControl instead.

Local connection with APMPlanner2/QGroundControl is automatic, and easier to use.

For APMPlanner2

Download it here http://firmware.eu.ardupilot.org/Tools/APMPlanner/
and launch it in terminal

````
apmplanner2
````

For QGroundControl

Download it here and follow the installation guide.

https://donlakeflyer.gitbooks.io/qgroundcontrol-user-guide/en/download_and_install.html


## Troubleshooting

### Missing libArduPilotPlugin.so... etc 

In case you see this message when you launch gazebo with demo worlds, check you have no error after sudo make install.  
If no error use "ls" on the install path given to see if the plugin is really here.  
If this is correct, check with "cat /usr/share/gazebo/setup.sh" the variable GAZEBO_PLUGIN_PATH. It should be the same as the install path. If not use "cp" to copy the lib to right path. 

For Example

````
sudo cp -a /usr/lib/x86_64-linux-gnu/gazebo-7.0/plugins/ /usr/lib/x86_64-linux-gnu/gazebo-7/
````

path mismatch is confirmed as ROS's glitch. It'll be fixed.

### Future(not activated yet)
To use Gazebo gps, you must offset the heading of +90° as gazebo gps is NWU and ardupilot is NED 
(I don't use GPS altitude for now)  
example : for SITL default location
````
    <spherical_coordinates>
      <surface_model>EARTH_WGS84</surface_model>
      <latitude_deg>-35.363261</latitude_deg>
      <longitude_deg>149.165230</longitude_deg>
      <elevation>584</elevation>
      <heading_deg>87</heading_deg>
    </spherical_coordinates>
````

