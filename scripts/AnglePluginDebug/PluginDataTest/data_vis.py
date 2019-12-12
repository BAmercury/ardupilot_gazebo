import csv
from matplotlib import pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import scipy
import numpy
# Investigation related to https://github.com/BAmercury/ardupilot_gazebo/issues/50
# program to visualize data recorded from the AnglePlugin
# Trying ot figure out why Ardupilot does not respond to angleplugin data but responds well to IRlockplugin data

# Data from AnglePlugin
# Using RC commands, I did steps on the Y axis
# x = []
# y = []
# with open('working_data.csv') as datafile:
#     csv_reader = csv.reader(datafile, delimiter=',')
#     for row in csv_reader:
#         #print(row[0])
#         x.append(float(row[0]))
#         y.append(float(row[1]))

# # Data from AnglePlugin/This specfic CSV is the correctedPos data, which is
# # normalized relative distance data 

# fig1, ax1 = plt.subplots()
# ax1.plot(range(len(y)), y)
# ax1.set_title("AnglePlugin Data Y Axis")

# # fig2, ax2 = plt.subplots()
# # ax2.plot(range(len(x)), x)
# # ax2.set_title("AnglePlugin Data X Axis")


# ir_x = []
# ir_y = []
# with open('working_IR_data.csv') as f:
#     csv_reader = csv.reader(f, delimiter=',')
#     for row in csv_reader:
#         ir_x.append(float(row[0]))
#         ir_y.append(float(row[1]))

# fig3, ax3 = plt.subplots()
# ax3.plot(range(len(ir_x)), ir_x)
# ax3.set_title("IRLock Data X Axis")

# # fig4, ax4 = plt.subplots()
# # ax4.plot(range(len(ir_y)), ir_y)
# # ax4.set_title("IRLock Data Y Axis")

# # Looks like AnglePlugin Y is IRLock Data X, but inversed. So:
# # -AnglePlugin.Y = IRLockData.X
# # Grabbing next set of CSV files with this change in mind, and will test X axis properly to make sure that looks good



# # Trial 2
# y = []
# with open('AnglePlugin_Y_2.csv') as f:
#     csv_reader = csv.reader(f, delimiter=',')
#     for row in csv_reader:
#         y.append(float(row[1]))

# fig5, ax5 = plt.subplots()
# ax5.plot(range(len(y)), y)
# ax5.set_title("AnglePlugin Data Y Axis Negated")

# # Comparing this to figure 3 it seems that my assumption was correct, We must negatve the Y data from the AnglePlugin
# # then assign that as the X variable we send out to Ardupilot, as IRLock currently does

# # Trial 3

# # Doing same process but for the X axis
# x = []
# with open('AnglePlugin_X.csv') as f:
#     csv_reader = csv.reader(f, delimiter=',')
#     for row in csv_reader:
#         x.append(float(row[0]))
# fig6, ax6 = plt.subplots()
# ax6.plot(range(len(x)), x)
# ax6.set_title("AnglePlugin Data X Axis")

# ir_x = []
# with open('IR_data_X.csv') as f:
#     csv_reader = csv.reader(f, delimiter=',')
#     for row in csv_reader:
#         ir_x.append(float(row[1]))
# fig7, ax7 = plt.subplots()
# ax7.plot(range(len(ir_x)), ir_x)
# ax7.set_title("IR Data Y Axis")

# # Plotting Time from AngelPlugin
# time_v = []
# with open('IR_data_X.csv') as f:
#     csv_reader = csv.reader(f, delimiter=',')
#     for row in csv_reader:
#         time_v.append(float(row[0]))
# fig8, ax8 = plt.subplots()
# ax8.plot(time_v, time_v)
# ax8.set_title("Time")


# Trial 4
# Comparing both data on top of each other
x = []
y = []
with open('data.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        x.append(float(row[0]))
        y.append(float(row[1]))
fig9, ax9 = plt.subplots()
ax9.plot(range(len(x)), x)
ax9.set_title("Angle Data X")

fig10, ax10 = plt.subplots()
ax10.plot(range(len(y)), y)
ax10.set_title("Angle Data Y")


ir_x = []
ir_y = []
with open('IR_data.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        ir_x.append(float(row[0]))
        ir_y.append(float(row[1]))
fig11, ax11 = plt.subplots()
ax11.plot(range(len(ir_x)), ir_x)
ax11.set_title("IR Data X")

fig12, ax12 = plt.subplots()
ax12.plot(range(len(ir_y)), ir_y)
ax12.set_title("IR Data Y")

# Moving Target Simutaneous data
x = []
y = []
with open('data_moving_tar.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        x.append(float(row[0]))
        y.append(float(row[1]))

ir_x = []
ir_y = []
with open('IR_data_moving_tar.csv') as f:
    csv_reader = csv.reader(f, delimiter=',')
    for row in csv_reader:
        ir_x.append(float(row[0]))
        ir_y.append(float(row[1]))
        
        
fig13, ax13 = plt.subplots(2)
ax13[0].plot(range(len(ir_x)), ir_x)
#ax13.plot(range(len(x)), x)
ax13[0].set_title("IR Data X and Angle X")
ax13[1].plot(range(len(x)), x)
# Find time delay between the two signals
# https://stackoverflow.com/questions/4688715/find-time-shift-between-two-similar-waveforms
# https://www.mathworks.com/help/signal/ref/finddelay.html
#ir_f = scipy.fft(ir_x)
#a_f = scipy.fft(x)
#c = scipy.ifft(a_f * scipy.conj(ir_f))

#time_shift = argmax(abs(c))

#print(time_shift)



plt.show()


