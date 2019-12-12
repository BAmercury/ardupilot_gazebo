# program to plot data
# Test was to see effect of normalizing the AnglePlugin output data vs not normalizing it
# Noramlizing it seems to work well with Ardupilot. Even though Ardupilot already normalizes it again

import csv
from matplotlib import pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import scipy
import numpy



ux = []
uy = []
x = []
y = []

with open('normtest.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        uy.append(float(row[0]))
        ux.append(float(row[1]))
        y.append(float(row[2]))
        x.append(float(row[3]))
fig1, ax1 = plt.subplots()
ax1.plot(range(len(ux)), ux)
ax1.plot(range(len(x)), x)
ax1.set_title("Normalized X vs X")


fig2, ax2 = plt.subplots()
ax2.plot(range(len(uy)), uy)
ax2.plot(range(len(y)), y)
ax2.set_title("Normalized Y vs Y")

# Test 2, added more data, altitude was 10 m
# Data is packed as: Norm by Height, No Norm, Norm by Mag, Angle
ux = []
uy = []
x = []
y = []
mx = []
my = []
ax = []
ay = []
with open('normtest_2.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        uy.append(float(row[0]))
        ux.append(float(row[1]))
        y.append(float(row[2]))
        x.append(float(row[3]))
        my.append(float(row[4]))
        mx.append(float(row[5]))
        ax.append(float(row[6]))
        ay.append(float(row[7]))

fig3, ax3 = plt.subplots()
ax3.plot(range(len(uy)), uy, label='Norm by Height')
ax3.plot(range(len(y)), y, label='No Norm')
ax3.plot(range(len(my)), my, label='Norm by Mag')
ax3.plot(range(len(ay)), ay, label='Angle')
ax3.legend()
ax3.set_title("Y Axis")

fig4, ax4 = plt.subplots()
ax4.plot(range(len(ux)), ux, label='Norm by Height')
ax4.plot(range(len(x)), x, label='No Norm')
ax4.plot(range(len(mx)), mx, label='Norm by Mag')
ax4.plot(range(len(ax)), ax, label='Angle')
ax4.legend()
ax4.set_title("X Axis")

# Test 3
# NormMag, NormH, Angle
ux = []
uy = []
hx = []
hy = []
ax = []
ay = []

# with open('normtest_3.csv') as f:
#     csv_reader = csv.reader(f, delimiter=',')
#     for row in csv_reader:
#         uy.append(float(row[0]))
#         ux.append(float(row[1]))
#         hy.append(float(row[2]))
#         hx.append(float(row[3]))
#         ax.append(float(row[4]))
#         ay.append(float(row[5]))

# fig5, ax5 = plt.subplots()
# ax5.plot(range(len(uy)), uy, label='Norm by Mag')
# ax5.plot(range(len(hy)), hy, label='Norm by Height')
# ax5.plot(range(len(ay)), ay, label='Angle')
# ax5.legend()
# ax5.set_title("Y Axis")

# fig6, ax6 = plt.subplots()
# ax6.plot(range(len(ux)), ux, label='Norm by Mag')
# ax6.plot(range(len(hx)), hx, label='Norm by Height')
# ax6.plot(range(len(ax)), ax, label='Angle')
# ax6.legend()
# ax6.set_title("X Axis")

# Test 4
# NormMag, NormH
ux = []
uy = []
hx = []
hy = []

with open('normtest_4.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        uy.append(float(row[0]))
        ux.append(float(row[1]))
        hy.append(float(row[2]))
        hx.append(float(row[3]))

fig5, ax5 = plt.subplots()
ax5.plot(range(len(uy)), uy, label='Norm by Mag')
ax5.plot(range(len(hy)), hy, label='Norm by Height')
ax5.legend()
ax5.set_title("Y Axis")

fig6, ax6 = plt.subplots()
ax6.plot(range(len(ux)), ux, label='Norm by Mag')
ax6.plot(range(len(hx)), hx, label='Norm by Height')
ax6.legend()
ax6.set_title("X Axis")


plt.show()
