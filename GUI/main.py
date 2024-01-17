from typing import Optional
import serial.tools.list_ports
import serial
import sys
import numpy as np
import getdata
from PySide6.QtCore import QObject, QTimer, Qt
from PySide6.QtGui import QPainter
from PySide6.QtWidgets import QApplication, QSizePolicy, QPushButton, QHBoxLayout, QWidget,QVBoxLayout, QComboBox, QLabel
from PySide6.QtCharts import QChart, QChartView, QLineSeries, QValueAxis


comList = []
ax = []

for i in range(0,100):
    ax.append(0)


class Worker(QObject):
    def __init__(self, function, interval):
        super(Worker, self).__init__()
        self._funcion = function
        self._timer = QTimer(self, interval=interval, timeout=self.execute)

    @property
    def running(self):
        return self._timer.isActive()

    def start(self):
        self._timer.start()

    def stop(self):
        self._timer.stop()

    def execute(self):
        self._funcion()
        
class  MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        
        self.i = 1
        self.timeStep = 0.1
        self.maximumXValue = 15.0
        self.time = 0.0
        
        self.comComboBox = QComboBox()
        self.refreshButton = QPushButton("refresh")
        
        self.seriesHeartBeat = QLineSeries()
        
        self.chartHeartBeat = QChart()
        
        mainLayout = QVBoxLayout()
        portsViewLayout = QHBoxLayout()
        
        self.SetChart(self.chartHeartBeat,self.seriesHeartBeat,mainLayout,15000,"x")
        
        
        self.FindAvalibleComports(comList) 
        self.AddPortsToCombo(comList,self.comComboBox)
        self.comComboBox.activated.connect(self.SetCurrentPort)
        
        portsViewLayout.addWidget(self.refreshButton)
        portsViewLayout.addWidget(self.comComboBox)
        
        mainLayout.addLayout(portsViewLayout)
        self.setLayout(mainLayout)
        
        self.worker = Worker(self.UpdatePlot,100)
        self.refreshButton.clicked.connect(self.worker.start)
   
        #self.SetChart(self.chartHeartBeat,self.seriesHeartBeat,2.0,"acc","x")
    def SetChart(self,chart,series,chartLayout,xAxisRange,title):

        chart.legend().hide()
        chart.addSeries(series)
        # .. axis properties ..
        axisX = QValueAxis()
        axisY = QValueAxis()
        
        axisY.setRange(0,xAxisRange) # setting the range for y axis
        
        axisY.setTitleText("Przyśpieszenie[g]") 
        
        axisX.setRange(0.0,self.maximumXValue) # setting the range for x axis
        axisX.setTitleText("Czas[s]") # setting title to x axis
        
        chart.setAxisX(axisX,series) # connecting axis propertis to series 
        chart.setAxisY(axisY,series)
           
        chartView = QChartView(chart) #adding chart to chartView
        chartView.setRenderHint(QPainter.Antialiasing)
        
        chartLayout.addWidget(chartView) # adding chartView to layout
        size = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        size.setHorizontalStretch(1)
        chartView.setSizePolicy(size)
        
    def UpdatePlot(self):    
        #print(self.i)
        #print('updating')
        if self.i == 1:
            self.i = self.i +1
            print(self.i)
        self.time = self.time + self.timeStep
        dataFreqz = 100
        getdata.getData("COM6")
        self.seriesHeartBeat.append(self.time, getdata.ir)
        print(getdata.ir)

        #
        if self.time >= self.maximumXValue:
            self.time = 0.0
            self.seriesHeartBeat.removePoints(0,int(self.maximumXValue/ self.timeStep)+1)


    def FindAvalibleComports(self,comList):

        ports = serial.tools.list_ports.comports()

        for port, desc, hwid in sorted(ports):
            comList.append("{}".format(port))
        print(comList)

    def SetCurrentPort(self,index):
        self.currentComport = self.comComboBox.itemText(index)
        print(self.currentComport)
      
    def AddPortsToCombo(self,comList,combo):
        for port in sorted(comList):
            combo.addItem(port)

if __name__ == "__main__":
    app = QApplication(sys.argv)

    window = MainWindow()
    window.setWindowTitle("Heart Monitoring")
    window.show()
    window.resize(1200, 900)
    # inertial.setInitAng()
    #window.showFullScreen()
    sys.exit(app.exec())