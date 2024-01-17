import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


df_data = pd.read_csv("data1.csv", sep=" ")
ired_diode  = [df_data['ired']]
red_diode  = np.array(df_data['red'])

sample_no = len(ired_diode)
with open('data1.csv') as my_file:
    for _ in range(1):
        next(my_file)
        time = []

    # Iteracja po linijkach pliku
    for i, line in enumerate(my_file, start=1):
        # Dodanie czasu do listy
        time.append([i, line.strip()])





#print(ired_diode)
print(sample_no)
print(time)
plt.plot(time, ired_diode)
plt.title("Heart Rate")
plt.show()