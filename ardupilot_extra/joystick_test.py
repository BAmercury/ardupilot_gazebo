from dronekit import connect, VehicleMode
import pygame
import time

# Simple program to get Joystick imputs and send out RC commands via MAVLink

# Function to map joystick input to PWM
# PWM range is 1900 to 1100 and joystick range is 1 to -1
def map2pwm(x):
    return int( (x - -1) * (1900 - 1100) / (1 - -1) + 1100)



# Returns a list of joystick commands (Throttle, Yaw, Roll, Pitch) from user
# mapped to PWM values
def getJoystickUpdates(event):
    joy_input = []
    if event.type == pygame.JOYAXISMOTION:
                joy_input.append(map2pwm(j_interface.get_axis(0))) # Roll, RC 1
                joy_input.append(map2pwm(j_interface.get_axis(1))) # Pitch, RC 2
                joy_input.append(map2pwm(j_interface.get_axis(2))) # Yaw, RC 4
                joy_input.append(map2pwm(-j_interface.get_axis(3))) # Throttle (Needs inverted), RC 3
    return joy_input

# Returns list of button commands (Mode changes, Macros, etc.)
def getButtonUpdates(event):
    button_input = []
    

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


# Connect to the UDP endpoint
vehicle = connect('127.0.0.1:14550', wait_ready=True)
print("Mode: %s" % vehicle.mode.name)

print("Waiting for vehicle to initialize...")
while not vehicle.is_armable:
    time.sleep(1)


vehicle.mode = VehicleMode("LOITER")
vehicle.armed = True

print("Waiting for arming")
while not vehicle.armed:
    time.sleep(1)


# Main loop
pilot_joy_enable = True
joystick_inputs = [1000, 1000, 1000, 1000]
try:
    while True:
        
        # Check to see if any Gamepad events happened

        for event in pygame.event.get():
            joystick_inputs = getJoystickUpdates(event)
            button_inputs = getButtonUpdates(event)


        # Send Joystick Inputs if enabled
        if pilot_joy_enable:
            vehicle.channels.overrides = {'1': joystick_inputs[0], '2': joystick_inputs[1], '3': joystick_inputs[3], '4': joystick_inputs[2]}
        time.sleep(0.05) # 20 Hz Radio Update Rate
except KeyboardInterrupt:
    pygame.quit()
    vehicle.close()