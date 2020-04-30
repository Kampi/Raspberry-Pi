import sys
import json
import requests
import datetime

from PyQt5 import QtWidgets, uic
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from datetime import datetime

from GetImage import GetImage

class Main(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.__Location = str()
        self.__APIKey = str()
        self.__Timer = QTimer(self)
        self.__Timer.timeout.connect(self.__loadData)

        # Load the UI
        uic.loadUi("app/ui/Main.ui", self)

        # Connect the sockets
        self.pushButton_LoadData.clicked.connect(self.__loadData)
        self.lineEdit_Location.textChanged.connect(self.locationChangedEvent)
        self.lineEdit_APIKey.textChanged.connect(self.apiChangedEvent)
        self.actionAbout_Qt.triggered.connect(self.aboutQtEvent)

        self.tableWidget_Data.horizontalHeader().setStretchLastSection(True) 
        self.tableWidget_Data.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.tableWidget_Data.verticalHeader().setStretchLastSection(True) 
        self.tableWidget_Data.verticalHeader().setSectionResizeMode(QHeaderView.Stretch)

        # Clear the status bar and fix the size of the main window
        self.statusBar().showMessage("")
        self.setFixedSize(560, 480)
        self.show()

    def Close(self):
        self.__Timer.stop()
        self.close()

    def aboutQtEvent(self):
        QMessageBox.aboutQt(self, "About Qt")

    def keyPressEvent(self, event):
        if(event.key() == Qt.Key_Escape):
            self.Close()

    def closeEvent(self, event):
        self.Close()

    def locationChangedEvent(self, text):
        self.__Location = text

    def apiChangedEvent(self, text):
        self.__APIKey = text

    def __loadData(self):
        if(not(self.__Timer.isActive())):
            self.__Timer.start(10000)

        Response = requests.get("http://api.openweathermap.org/data/2.5/weather?q={}&APPID={}".format(self.__Location, self.__APIKey))

        if(Response.ok):
            self.statusBar().showMessage("[{}] Data fetch successfull".format(datetime.now().strftime("%H:%M:%S")))
            self.__WeatherData = Response.json()
            
            self.__refreshData()
        else:
            self.statusBar().showMessage("[{}] Error during data fetch!".format(datetime.now().strftime("%H:%M:%S")))

    def __refreshData(self):
        self.tableWidget_Data.setItem(0, 0, QTableWidgetItem("Humidity"))
        self.tableWidget_Data.setItem(0, 1, QTableWidgetItem("{} %".format(self.__WeatherData["main"]["humidity"])))
        self.tableWidget_Data.setItem(1, 0, QTableWidgetItem("Pressure"))
        self.tableWidget_Data.setItem(1, 1, QTableWidgetItem("{} hPa".format(self.__WeatherData["main"]["pressure"])))
        self.tableWidget_Data.setItem(2, 0, QTableWidgetItem("Temperature"))
        self.tableWidget_Data.setItem(2, 1, QTableWidgetItem("{:.2f} °C".format(self.__WeatherData["main"]["temp"] - 273.0)))
        self.tableWidget_Data.setItem(3, 0, QTableWidgetItem("Temperature max."))
        self.tableWidget_Data.setItem(3, 1, QTableWidgetItem("{:.2f} °C".format(self.__WeatherData["main"]["temp_max"] - 273.0)))
        self.tableWidget_Data.setItem(4, 0, QTableWidgetItem("Temperature min."))
        self.tableWidget_Data.setItem(4, 1, QTableWidgetItem("{:.2f} °C".format(self.__WeatherData["main"]["temp_min"] - 273.0)))
        self.tableWidget_Data.setItem(5, 0, QTableWidgetItem("Speed of wind"))
        self.tableWidget_Data.setItem(5, 1, QTableWidgetItem("{} m/s".format(self.__WeatherData["wind"]["speed"])))

        if("deg" in self.__WeatherData["wind"]):
            self.tableWidget_Data.setItem(6, 0, QTableWidgetItem("Wind angle"))
            self.tableWidget_Data.setItem(6, 1, QTableWidgetItem("{} °".format(self.__WeatherData["wind"]["deg"])))
        else:
            self.tableWidget_Data.setItem(6, 0, QTableWidgetItem("Wind angle"))
            self.tableWidget_Data.setItem(6, 1, QTableWidgetItem("{} °".format(0)))

        self.lineEdit_LocationOut.setText("{}, {}".format(self.__WeatherData["name"], self.__WeatherData["sys"]["country"]))
        self.label_WeatherImage.setPixmap(QPixmap.fromImage(QImage(GetImage(self.__WeatherData["weather"][0]["icon"]))) )

if(__name__ == "__main__"):
    app = QApplication(sys.argv)
    App = Main()
    sys.exit(app.exec_())