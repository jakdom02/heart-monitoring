from typing import Optional
import serial.tools.list_ports
import serial
import sys
import numpy as np
import getdata
import matplotlib.pyplot as plt
from scipy.signal import lfilter 
from tsmoothie.smoother import *
from PySide6.QtCore import QObject, QTimer, Qt
from PySide6.QtGui import QPainter, QFont, QFontDatabase, QBrush,QColor
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

        self.currentComport = "COM4"
        self.i = 1
        self.timeStep = 0.01
        self.maximumXValue = 4.0
        self.time = 0.0
        self.hrRateArray = []
        self.redArray = []

        self.comComboBox = QComboBox()
        self.refreshButton = QPushButton("refresh")

        self.spoLabel = QLabel()
        self.BPMlabel = QLabel()
        self.infoLabel = QLabel()

        self.axisY = QValueAxis()

        self.infoFont = QFont()
        self.infoFont.setPointSize(40)
        self.infoFont.setFamily("ISOCT3")
        self.infoFont

        self.fonts = QFontDatabase()
        #print(self.fonts.families())    

        self.BPMlabel.setText('BPM: ...')
        self.BPMlabel.setFont(self.infoFont)

        self.spoLabel.setText('SPO<sub>2</sub>: ...')
        self.spoLabel.setFont(self.infoFont)
        
        
        self.seriesHeartBeat = QLineSeries()
        
        self.chartHeartBeat = QChart()
        self.chartHeartBeat.setBackgroundBrush(QBrush(QColor("#F1F4FF")))
        
        mainLayout = QVBoxLayout()
        chartViewLayout = QHBoxLayout()
        portsViewLayout = QHBoxLayout()
        infoPanelLayout = QHBoxLayout()
        
        self.SetChart(self.chartHeartBeat,self.seriesHeartBeat,chartViewLayout,15000,"x")   
        
        self.FindAvalibleComports(comList) 
        self.AddPortsToCombo(comList,self.comComboBox)
        self.comComboBox.activated.connect(self.SetCurrentPort)
        
        portsViewLayout.addWidget(self.refreshButton)
        portsViewLayout.addWidget(self.comComboBox)

        infoPanelLayout.addWidget(self.BPMlabel)
        infoPanelLayout.addWidget(self.spoLabel)
        infoPanelLayout.addWidget(self.spoLabel)

        mainLayout.addLayout(portsViewLayout)
        mainLayout.addLayout(infoPanelLayout)
        mainLayout.addLayout(chartViewLayout)

        mainLayout.setStretchFactor(portsViewLayout,1)
        mainLayout.setStretchFactor(infoPanelLayout,1)
        mainLayout.setStretchFactor(chartViewLayout,6)
        
        self.setLayout(mainLayout)
        
        self.worker = Worker(self.UpdatePlot,10)
        self.refreshButton.clicked.connect(self.worker.start)
   

    def SetChart(self,chart,series,chartLayout,xAxisRange,title):

        chart.legend().hide()
        chart.addSeries(series)

        axisX = QValueAxis()
        
        self.axisY.setTitleText("Signal[V]") 
        self.axisY.setRange(0,10000)
        
        axisX.setRange(0.0,self.maximumXValue) # setting the range for x axis
        axisX.setTitleText("Time[s]") # setting title to x axis
        
        chart.setAxisX(axisX,series) # connecting axis propertis to series 
        chart.setAxisY(self.axisY,series)
           
        chartView = QChartView(chart) #adding chart to chartView
        chartView.setRenderHint(QPainter.Antialiasing)
        
        chartLayout.addWidget(chartView) # adding chartView to layout
        size = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        size.setHorizontalStretch(1)
        chartView.setSizePolicy(size)
        

    def UpdatePlot(self):    
        if self.i == 1:
            self.i = self.i +1
            #print(self.i)
        self.time = self.time + self.timeStep
        getdata.getData(self.currentComport)
        self.seriesHeartBeat.append(self.time, getdata.ir)
        self.redArray.append(getdata.red)
        self.hrRateArray.append(getdata.ir)
        if  np.size(self.hrRateArray) == 400:

            smoother = ConvolutionSmoother(window_len=15,window_type="ones")
            smoother.smooth(self.hrRateArray)
            newIR = smoother.smooth_data[0]
        
            smoother2 = ConvolutionSmoother(window_len=15,window_type="ones")
            smoother2.smooth(self.redArray)
            newRED = smoother2.smooth_data[0]

            self.CalculateHeartBeat(newIR,5,10)
            self.CalculateSpoRate(newIR,newRED)

            self.hrRateArray.clear()
            self.redArray.clear()
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
    

    def CalculateSpoRate(self,irReadArray,redReadArray):
        irMinVal = np.min(irReadArray)
        irMaxVal = np.max(irReadArray)
        self.axisY.setRange(irMinVal-1000,irMaxVal+1000)
        redMinVal = np.min(redReadArray)
        redMaxVal = np.max(redReadArray)

        redDC = np.mean(redReadArray)
        irDC = np.mean(irReadArray)
        redAC = redMaxVal-redMinVal
        irAC = irMaxVal-irMinVal
        r = (redAC/redDC)/(irAC/irDC)
        spoRate = 110-r*25
        self.spoLabel.setText("spo2: " + str(np.round(spoRate,3)) + " %" )


    def CalculateHeartBeat(self,irReadingArray,number1,number2, i = 0):

        #print(irReadingArray)
        #plt.plot(irReadingArray)
        #plt.show()
        hbDetected = 0
        hbDetectedIndex = [] 
        while i < len(irReadingArray) - 2:
            if int(irReadingArray[i + 1]) < int(irReadingArray[i]) - number1 and int(irReadingArray[i + 2]) < int(irReadingArray[i]) - number2:
                hbDetected = i                
                hbDetectedIndex.append(hbDetected)
                i += 30             
            else:
                i += 1
        self.CalculateBeatFromIndex(hbDetectedIndex)


    def CalculateBeatFromIndex(self,indexarray):

        i = 0
        bpmArray = []
        match len(indexarray):
            case 0:
                print('error: too few samples')
            case default:
                while i < len(indexarray) - 1:
                    bpm = 60/((indexarray[i+1] - indexarray[i])/100)
                    if bpm > 50 and bpm < 250:
                        bpmArray.append(bpm)
                    i+=1
                self.BPMlabel.setText("BPM: " + str(np.round(np.average(bpmArray),2)))              


if __name__ == "__main__": 
    app = QApplication(sys.argv)

    window = MainWindow()
    window.setWindowTitle("Heart Monitoring")
    window.setStyleSheet("background-color: #A2A2A1;")
    window.show()
    window.resize(1200, 900)
    #window.showFullScreen()
    sys.exit(app.exec())