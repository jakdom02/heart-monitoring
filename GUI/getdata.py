import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import serial
import time 

def getData(com):
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
    new_ir = data[0:i]
    data = data[i + 1:]
    i = data.find(',')
    new_red = data[0:i]
    global ir
    global red
    ir = int(new_ir, base = 16)
    red = int(new_red, base = 16)