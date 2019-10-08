from dronekit import connect, VehicleMode
import pygame
import time
import json

# Simple program to get Joystick imputs and send out RC commands via MAVLink

# Declare Variables
pilot_joy_enable = False
joystick_inputs = [1500, 1500, 1500, 1500]
btn_inputs = [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000]
# Declare list of booleans for the Precision, Custom Mode, and Arm toggles
toggle_prec = 0
toggle_custom_mode = 0
toggle_arm = 0

# Function to map joystick input to PWM
# PWM range is 1900 to 1100 and joystick range is 1 to -1
def map2pwm(x):
    return int( (x - -1) * (1900 - 1100) / (1 - -1) + 1100)

"""
    Vehicle Macro 1:
    This macro gives collective throttle to the drone to raise it up to a user defined altitude
    Then returns vehicle to an idle throttle. 
    Macro is a blocking function so user cannot input commands while this macro is running
"""

def macro1():
    # Take off in loiter and reach a desired alt, then leave throttle on idle

    # First check to see if the vehicle is actuallly armed:
    if vehicle.armed == True:

        vehicle.mode = VehicleMode("LOITER")
        #desired_alt = 10 # meters
        desired_alt = input("Give me a desired altitude (m): ")
        print("Taking off to desired altitude: %s" % desired_alt)
        while (vehicle.location.global_relative_frame.alt <= desired_alt):
            print("Vehicle Altitude: %s" % vehicle.location.global_relative_frame.alt)
            vehicle.channels.overrides[3] = 1800
        vehicle.channels.overrides[3] = 1500 # Idle throttle
        print("Macro 1 Complete")
    else:
        print("Please Arm the vehicle and try again")

# Returns a list of joystick commands (Throttle, Yaw, Roll, Pitch) from user
# mapped to PWM values
def getJoystickUpdates(mapping):
    joy_input = []
    joy_input.append(map2pwm(j_interface.get_axis(int(mapping['Roll'])))) # Roll, RC 1
    joy_input.append(map2pwm(j_interface.get_axis(int(mapping['Pitch'])))) # Pitch, RC 2
    joy_input.append(map2pwm(j_interface.get_axis(int(mapping['Yaw'])))) # Yaw, RC 4
    joy_input.append(map2pwm(-j_interface.get_axis(int(mapping['Throttle'])))) # Throttle (Needs inverted), RC 3
    return joy_input

# Evaluates states of buttons on controller and outputs corresponding commands
def ButtonUpdates(mapping):
    global pilot_joy_enable, toggle_custom_mode, toggle_arm, toggle_prec

    #  Prec On/OFF
    if (j_interface.get_button(int(mapping['Prec'])) == 1):
        toggle_prec ^= 1 # Flip Prec on or off
        # Depending on the updated state of the button, deploy the action
        if (toggle_prec):
            vehicle.channels.overrides[8] = 2000
            time.sleep(0.5)
            print "Precision ON"
        else:
            
            vehicle.channels.overrides[8] = 1000
            time.sleep(0.5)
            print "Precision OFF"
    # Arm On/off
    if (j_interface.get_button(int(mapping['Arm'])) == 1):
        toggle_arm ^= 1
        # Depending on the updated state of the button, deploy the action
        if (toggle_arm):
            vehicle.armed = True
            print("Waiting for arming")
            while not vehicle.armed:
                time.sleep(1)
            print("armed")
            # Enable Joystick input for pilot
            pilot_joy_enable = True
        else:
            vehicle.armed = False
            print("Disarmed")
    # LOITER Mode
    if (j_interface.get_button(int(mapping['Loiter'])) == 1):
        print("Mode LOITER")
        vehicle.mode = VehicleMode("LOITER")
    # LAND mode
    if (j_interface.get_button(int(mapping['Land'])) == 1):
        print("Mode LAND")
        vehicle.mode = VehicleMode("LAND")
    # Macro 1
    if (j_interface.get_button(int(mapping['Macro1'])) == 1):
        print("Performing Macro 1")
        macro1()




"""
    Main Code starts here
"""

# Init joystick
pygame.init()
pygame.joystick.init() 

# Create a joystick instance
try:
    j_interface = pygame.joystick.Joystick(0)
    j_interface.init()
    print("Enabled Joystick: {0}".format(j_interface.get_name()))
except pygame.error:
    print("No Joystick found, exiting program")
    exit()

# Try loading the configuration file for the button mapping
try:
    with open('config.json', 'r') as f:
        config_map = json.load(f)
except Exception:
    print("Failed to load JSON file. Please check the config.json file and try again")
    pygame.quit()
    exit()
# If the map is empty, the JSOn was not loaded correctly, exit the program and try again
if (bool(config_map) == False):
    print("Configuration file was not loaded properly. Please check the config.json file and try again")
    pygame.quit()
    exit()
else:
    print("Config file loaded correctly")


# Connect to the Ardupilot SITL UDP Endpoint and wait till the Vehicle is done intializing
# Connection time out will cause program to exit. user will have to restart the program
try:
    # Connect to the UDP endpoint
    vehicle = connect(str(config_map['UDP']), wait_ready=True)
    print("Mode: %s" % vehicle.mode.name)

    print("Waiting for vehicle to initialize...")
    while not vehicle.is_armable:
        time.sleep(1)
    print("Vehicle is ready")
except Exception:
    print("Error has occurred. Please try restarting the program")
    pygame.quit()
    exit()


# Main loop

try:
    while True:
        
        # Check to see if any Gamepad events happened
        for event in pygame.event.get():
            # Updates joystick values and sends them for every time joystick is moved from origin
            if event.type == pygame.JOYAXISMOTION:
                joystick_inputs = getJoystickUpdates(config_map)

            if event.type == pygame.JOYBUTTONDOWN:
                ButtonUpdates(config_map)
                #print "Attitude: %s" % vehicle.attitude
                #print "Global Location (relative altitude): %s" % vehicle.location.global_relative_frame.alt

        # Send Joystick Inputs if enabled
        if pilot_joy_enable:
            vehicle.channels.overrides = {'1': joystick_inputs[0], '2': joystick_inputs[1], '3': joystick_inputs[3], '4': joystick_inputs[2]}
        time.sleep(float(config_map['UpdateRate'])) # Radio Update Rate
except KeyboardInterrupt:
    pygame.quit()
    vehicle.close()