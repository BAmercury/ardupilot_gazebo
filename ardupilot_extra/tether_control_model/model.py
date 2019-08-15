import control
import numpy as np
from matplotlib import pyplot as plt

m = 1.5 # Kg, 3DR Iris Drone
k = 0.1 # Air resistance coefficient estimate
g = 9.8 # M/s^2
A = -k/m
B = 1/m
C = 1
D = 0
u = 100 # Newtons step input
# Make a state space model
sys = control.StateSpace(A,B,C,D)
print(sys)
# Look at poles of model
control.pzmap(sys)

# Plot step response open loop
plt.figure(2)

T, yout = control.step_response(u*sys)
plt.plot(T, yout)
plt.title("Open Loop Response")

# State feedback with feedforward gain for tracking
G = 7.4 # places poles at -5
#G = 7.8
#G = 1
# Calculate Scaling Factor
s = 1
z = np.array([np.zeros(s), 1])
N = np.linalg.inv(np.array([[A,B], [C,D]])) * np.transpose(z)
n_x = N[0]
n_u = N[1]
Nbar = n_u + G*n_x
FFG = Nbar.item(1)
print(FFG)
r = 10 # M/s
u = r
u = FFG*r
# Add distrubance from gravity/other process variation

w = g*m
u = u + w

sys_fdbk = control.StateSpace(A-(B*G), B, C, D)
control.pzmap(sys_fdbk)
plt.figure(4)

T, yout = control.step_response(u*sys_fdbk)
plt.plot(T,yout)
plt.title("Closed Loop Response")

plt.show()

