from dronekit import connect, VehicleMode
import pygame
import time

# Simple program to get Joystick imputs and send out RC commands via MAVLink


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

try:
    while True:
        vehicle.channels.overrides['3'] = 1800
        print("Applying Thrust")
        time.sleep(1)
except KeyboardInterrupt:
    vehicle.close()