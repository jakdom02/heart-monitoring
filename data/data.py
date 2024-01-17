import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import serial
import time 

def getData(com):
    time_delay = 1 / 100
    ser = serial.Serial(com, timeout=0)
    data = ''
    while 1:
        x = ser.read()
        x = x.decode('utf-8')
        data = data + x
        if x == '|':
            ser.close()
            break
    i = data.find(',')
    new_ax = data[0:i]
    data = data[i + 1:]
    i = data.find(',')
    new_ay = data[0:i]
    print(int(new_ax, base = 16),int(new_ay, base = 16))
    time.sleep(time_delay)
    global ir
    global red





while True:
    getData("COM6")
    #time.sleep(time_delay)

#df_data = pd.read_csv("data1.csv", sep=" ")
#red_diode  = np.array(df_data['red'])
#ired_diode  = [df_data['ired']]


#sample_no = len(ired_diode)
#with open('data1.csv') as my_file:
#    for _ in range(1):
#        next(my_file)
#        time = []

    # Iteracja po linijkach pliku
#    for i, line in enumerate(my_file, start=1):
        # Dodanie czasu do listy
 #       time.append(i, line.strip())





#print(ired_diode)
#print(sample_no)
#print(time)
#plt.plot(time, ired_diode)
#plt.title("Heart Rate")
#plt.show()