import time
import pygame

pygame.init()
pygame.joystick.init()


try:
    j_interface = pygame.joystick.Joystick(0)
    j_interface.init()
    print("Enabled Joystick: {0}".format(j_interface.get_name()))
except pygame.error:
    print("No Joystick found, exiting program")
    exit()


try:

    while True:
        input_list = []
        for event in pygame.event.get():
            if event.type == pygame.JOYAXISMOTION:
                input_list.append(j_interface.get_axis(0))
                input_list.append(j_interface.get_axis(1))
                input_list.append(j_interface.get_axis(2))
                input_list.append(j_interface.get_axis(3))
                print(input_list)
except KeyboardInterrupt:
    pygame.quit()
    exit()