

# Quick server to test the plugin

import socket
import sys

# Create TCP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind to a port
server_address = ("192.168.16.69", 10500)
sock.bind(server_address)
sock.listen(1)

conn, addr = sock.accept()
counter = 0
step = 50
setpoint = 20
data = str(setpoint)
while True:
    print(data.encode('utf-8'))
    conn.sendall(data.encode('utf-8'))
    #counter = counter + 1
    # if (counter == step):
    #     counter = 0
    #     if (setpoint > 50):
    #         setpoint = 0
    #     setpoint = setpoint + 5
    #     data = str(setpoint)