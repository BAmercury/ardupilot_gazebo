import time
import pygame

pygame.init()
pygame.joystick.init()

# Program to test joystick buttons out and identify mapping

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
            if event.type == pygame.JOYBUTTONDOWN:
                for button_index in range(j_interface.get_numbuttons()):
                    input_list.append(j_interface.get_button(button_index))
                print(input_list)
                print('\n')
        time.sleep(0.1)
except KeyboardInterrupt:
    pygame.quit()
    exit()