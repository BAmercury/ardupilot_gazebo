Simulation Notes:

Gazebo Physics Engine Setup:
    Real Time Update Rate: 1000 
    Max Step Size: 0.0025

    
Application Notes:

Gazebo Ardupilot Params:
# Iris is X frame
FRAME_CLASS	1
FRAME_TYPE	1
# IRLOCK FEATURE
CH8_OPT     39
PLND_ENABLED    1
# Gazebo SITL
PLND_TYPE       3
# Kalman Filter
#PLND_EST_TYPE   1
# KF Accelerometer Noise
#PLND_ACC_P_NSE	0.2
# SONAR FOR IRLOCK
SIM_SONAR_SCALE 10
RNGFND_TYPE 1
RNGFND_SCALING 10
RNGFND_PIN 0
RNGFND_MAX_CM 6000


1. Went up in Loiter with rc 3 1800
2. Then set Loiter throttle rc 3 1600 and stayed at a hover (ALT: ~15m)
3. Went into Precision Loiter by setting rc 8 1900
4. Drone tracked target (See data)
5. Went into LAND mode and landing was successful and full collision was detected


Test followed from test_121, shows configuration is possibly reproduceable
