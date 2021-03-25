

# pyuic5 -x mainwindow.ui -o mainwindow.py
# python3.8 -m PyInstaller --onefile --add-binary='/System/Library/Frameworks/Tk.framework/Tk':'tk' --add-binary='/System/Library/Frameworks/Tcl.framework/Tcl':'tcl' fizgui.py


from mainwindow import *
from dialog import *
from PyQt5 import QtCore, QtGui, QtWidgets, QtSerialPort

import serial
import glob
#import serial.tools.list_ports

#from threading import Thread

import sys
import os
from time import sleep

app = QtWidgets.QApplication(sys.argv)
MainWindow = QtWidgets.QMainWindow()
ui = Ui_MainWindow()
ui.setupUi(MainWindow)

Dialog = QtWidgets.QDialog()
dia = Ui_Dialog()
dia.setupUi(Dialog)


serial_run = False
configstring = False



ui.pushButton.clicked.connect(lambda: connect(str(ui.comboBox.currentText())))
ui.pushButton_send.clicked.connect(lambda: write_config())
ui.pushButton_getConfig.clicked.connect(lambda: read_config())
ui.lineEdit.returnPressed.connect(lambda: send_command())



def connect(port):
    global serial_run
    print("--> connecting to " + port)
    try:
        ui.serial = serial.Serial(
            port=port,
            baudrate=9600,
            timeout=1,
            rtscts=1
        )
        ui.serial.isOpen() # try to open port, if possible print message and proceed with 'while True:'

    except IOError: # if port is already opened, close it and open it again and print message
        ui.serial.close()
        ui.serial.open()
        print ("--> port was already open, was closed and opened again!")

    if True:
        print ("--> port opened")
        serial_run = True
        ui.serial.flush()
        sleep(1)
        ui.serial.write(str.encode("debug=0\n"))
        ui.serial.write(str.encode("debug=1\n"))
        ui.serial.flush()

        read_config()


def receive():
    global configstring

    while ui.serial.readable():
        text = ui.serial.readline().decode()
        text = text.rstrip('\r\n')
        print(text)
        #ui.textEdit.append(text)

        if text == "# fiz-o-matic configuration":
            configstring = True
            ui.textEdit.clear
            #ui.textEdit.setText("# fiz-o-matic configuration")

        if ui.radioButton.isChecked():
            ui.textEdit.append(text)
        else:
            if configstring:
                if not text.startswith('['):
                    ui.textEdit.append(text)

        if "batt2_voltage_port" in text:
                    configstring = False
                    break



def read_config():

    print("--> start reading config")
    ui.textEdit.clear()
    ui.textEdit.setText("")

    #ser.write(str.encode('config\n'))
    print("--> waiting for device to get ready")
    ui.serial.write(str.encode("config\n"))
    ui.serial.flush()

    receive()
    
    print("--> end reading config")



def write_config():
    print ("--> writing config")
    if serial_run:
        #print(ui.textEdit.toPlainText())
        config = ui.textEdit.toPlainText()

        for line in config.splitlines():
            #print(line)
            line = line.rstrip()


            if not ( line.startswith('#') or line == "" ):
                print(line)
                ui.serial.write(str.encode(line + "\n"))
                ui.serial.flush()
            
        #checking if the last two messages are the same and breaks while loop if so
        text_previous = "aaa"
        while ui.serial.readable():
            text_new = ui.serial.read_until(b'\n').decode()
            text_new = text_new.rstrip('\r\n')
            print(text_new)
            if text_new == text_previous: 
                break
            else:
                text_previous = text_new

            

        ui.serial.write(str.encode("save\n"))
        ui.serial.flush()

        read_config()




def send_command():
    print (ui.lineEdit.text())
    ui.serial.write(str.encode(ui.lineEdit.text()))
    ui.serial.write(str.encode("\n"))
    ui.serial.flush()
    ui.lineEdit.setText("")

def scan_serial_ports():
    for serialport in QtSerialPort.QSerialPortInfo.availablePorts():

        if sys.platform.startswith('win'):
            print ("found port: " + serialport.portName())
            ui.textEdit.append("Found Port")
            ui.textEdit.append("Port Name: " + serialport.portName())
            ui.textEdit.append("Description: " + serialport.description())
            ui.textEdit.append("Manufacturer: " + serialport.manufacturer())
            ui.textEdit.append("")

            ui.comboBox.addItem(serialport.portName())

        else:

            if serialport.portName().startswith('tty'):
                print ("found port: " + serialport.portName())

                ui.textEdit.append("Found Port")
                ui.textEdit.append("Port Name: " + serialport.portName())
                ui.textEdit.append("Description: " + serialport.description())
                ui.textEdit.append("Manufacturer: " + serialport.manufacturer())
                ui.textEdit.append("System Location: " + serialport.systemLocation())
                ui.textEdit.append("")

                ui.comboBox.addItem(serialport.systemLocation())

                if serialport.description().startswith('Adafruit'):
                    ui.comboBox.setCurrentText(serialport.systemLocation())


def main():

    #print (sys.platform)

    scan_serial_ports()

    MainWindow.show()
    sys.exit(app.exec_())






if __name__== "__main__":
    try:
        main()
    finally:
        print ("--> goodbye")
