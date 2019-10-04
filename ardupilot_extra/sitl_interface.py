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
def getJoystickUpdates():
    joy_input = []
    joy_input.append(map2pwm(j_interface.get_axis(0))) # Roll, RC 1
    joy_input.append(map2pwm(j_interface.get_axis(1))) # Pitch, RC 2
    joy_input.append(map2pwm(j_interface.get_axis(2))) # Yaw, RC 4
    joy_input.append(map2pwm(-j_interface.get_axis(3))) # Throttle (Needs inverted), RC 3
    return joy_input

# Declare list of booleans for the buttons
btn_toggle_states = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

# Returns list of button commands (Mode changes, Macros, etc.)
# Buttons need to be toggable
def getButtonUpdates():
    global btn_toggle_states
    btn_inputs = []
    for button_index in range(j_interface.get_numbuttons()):
        # If the button is pushed, flip the corresponding bool
        if (j_interface.get_button(button_index) == 1):
            btn_toggle_states[button_index] ^= 1
        # Depending on the state of the button, map to pwm and then save to the list
        btn_inputs.append(map2pwm(btn_toggle_states[button_index]))
    return btn_inputs

# Arm vehicle



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
print("Vehicle is ready")
# Will make these button or macro togglable
# vehicle.mode = VehicleMode("LOITER")
# vehicle.armed = True

# print("Waiting for arming")
# while not vehicle.armed:
#     time.sleep(1)


# Main loop
pilot_joy_enable = True
joystick_inputs = [1000, 1000, 1000, 1000]
btn_inputs = [1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000]
try:
    while True:
        
        # Check to see if any Gamepad events happened

        for event in pygame.event.get():
            if event.type == pygame.JOYAXISMOTION:
                joystick_inputs = getJoystickUpdates()
            if event.type == pygame.JOYBUTTONDOWN:
                btn_inputs = getButtonUpdates()
                # We only need to send out message when the button changes state, not stream continuously
                if btn_inputs[0] == 1900:
                    vehicle.mode = VehicleMode("LOITER")
                if btn_inputs[1] == 1900:
                    vehicle.armed = True
                    print("Waiting for arming")
                    while not vehicle.armed:
                        time.sleep(1)
                    print("armed")
                else:
                    vehicle.armed = False
                    print("disarmed")

        #print "Attitude: %s" % vehicle.attitude
        #print "Global Location (relative altitude): %s" % vehicle.location.global_relative_frame.alt
        # Send Joystick Inputs if enabled
        if pilot_joy_enable:
            vehicle.channels.overrides = {'1': joystick_inputs[0], '2': joystick_inputs[1], '3': joystick_inputs[3], '4': joystick_inputs[2]}


        time.sleep(0.005) # 20 Hz Radio Update Rate
except KeyboardInterrupt:
    pygame.quit()
    vehicle.close()