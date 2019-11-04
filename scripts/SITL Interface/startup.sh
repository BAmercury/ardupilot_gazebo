#!/bin/sh

# Launch Gazebo with user arguments

echo "Desired World: $1"

if [ "$1" = "cam_rail" ]
    then 
        gazebo --verbose iris_irlock_rail_sim.world
elif ["$1" = "nocam_rail" ]
    then gazebo --verbose iris_irlock_nocam_rail_sim.world
elif ["$1" = "cam_no_rail" ]
    then gazebo --verbose iris_irlock_demo.world
fi

echo "Launching Ardupilot SITL"

gnome-terminal -e "bash -c \"python ~/Documents/ardupilot_beta/Tools/autotest/sim_vehicle.py -v ArduCopter -f gazebo-iris --console; exec bash\""