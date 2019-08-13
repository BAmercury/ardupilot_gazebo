import control
import numpy as np
from matplotlib import pyplot as plt

m = 1.5
k = 0.1
g = 9.8
A = -k/m
B = 1/m
C = 1
D = 0
u = 100 # Newtons
sys = control.StateSpace(A,B,C,D)
print(sys)

control.pzmap(sys)

plt.figure(2)

T, yout = control.step_response(u*sys)
plt.plot(T, yout)
plt.title("Open Loop Response")


G = 7.4 # places poles at -5
#G = 1
# Calculate Scaling Factor
s = 1
z = np.array([np.zeros(s), 1])
N = np.linalg.inv(np.array([[A,B], [C,D]])) * np.transpose(z)
n_x = N[0]
n_u = N[1]
Nbar = n_u + G*n_x
FFG = Nbar.item(1)
r = 10 # M/s
u = r
u = r + g # Gravity as input bias
sys_fdbk = control.StateSpace(A-(B*G), B*FFG, C, D)
control.pzmap(sys_fdbk)
plt.figure(4)

T, yout = control.step_response(r*sys_fdbk)
plt.plot(T,yout)
plt.title("Closed Loop Response, pole at -5")

plt.show()

