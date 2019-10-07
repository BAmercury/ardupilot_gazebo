from dronekit import connect, VehicleMode
import pygame
import time

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
        desired_alt = 10 # meters
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
def getJoystickUpdates():
    joy_input = []
    joy_input.append(map2pwm(j_interface.get_axis(0))) # Roll, RC 1
    joy_input.append(map2pwm(j_interface.get_axis(1))) # Pitch, RC 2
    joy_input.append(map2pwm(j_interface.get_axis(2))) # Yaw, RC 4
    joy_input.append(map2pwm(-j_interface.get_axis(3))) # Throttle (Needs inverted), RC 3
    return joy_input

# Evaluates states of buttons on controller and outputs corresponding commands
def ButtonUpdates():
    global pilot_joy_enable, toggle_custom_mode, toggle_arm, toggle_prec

    #  Button 0 is Prec On/OFF
    if (j_interface.get_button(0) == 1):
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
    # Button 1 is Arm On/off
    if (j_interface.get_button(1) == 1):
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
    # Button 2 for LOITER Mode
    if (j_interface.get_button(2) == 1):
        print("Mode LOITER")
        vehicle.mode = VehicleMode("LOITER")
    # Button 5 for LAND mode
    if (j_interface.get_button(3) == 1):
        print("Mode LAND")
        vehicle.mode = VehicleMode("LAND")
    # Button 5 for Macro 1
    if (j_interface.get_button(4) == 1):
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


# Connect to the Ardupilot SITL UDP Endpoint and wait till the Vehicle is done intializing
# Connection time out will cause program to exit. user will have to restart the program
try:
    # Connect to the UDP endpoint
    vehicle = connect('127.0.0.1:14550', wait_ready=True)
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
                joystick_inputs = getJoystickUpdates()

            if event.type == pygame.JOYBUTTONDOWN:
                ButtonUpdates()
                #print "Attitude: %s" % vehicle.attitude
                #print "Global Location (relative altitude): %s" % vehicle.location.global_relative_frame.alt

        # Send Joystick Inputs if enabled
        if pilot_joy_enable:
            vehicle.channels.overrides = {'1': joystick_inputs[0], '2': joystick_inputs[1], '3': joystick_inputs[3], '4': joystick_inputs[2]}
        time.sleep(0.005) # 20 Hz Radio Update Rate
except KeyboardInterrupt:
    pygame.quit()
    vehicle.close()