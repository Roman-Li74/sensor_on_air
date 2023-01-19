#! /usr/bin/python3

import serial
import time
import glob
import mysql.connector

'''
class color:
	PURPLE = '\033[95m'
	CYAN = '\033[96m'
	DARKCYAN = '\033[36m'
	BLUE = '\033[94m'
	GREEN = '\033[92m'
	YELLOW = '\033[93m'
	RED = '\033[91m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'
	END = '\033[0m'
'''

def serial_ports():
	ports = glob.glob('/dev/ttyUSB[0-9]*')
	result = []
	for port in ports:
		try:
			s = serial.Serial(port)
			result.append(s.name)
			s.close()
		except(OSError, serial.SerialException):
			pass
	return result[0]

#print("Found: ", serial_ports())

print (serial_ports())

#comn = input("Enter COM-port number: ")
#comn = "/dev/ttyUSB"+comn

comn = serial_ports()

#print (comn)
#comn = "/dev/ttyUSB0"

#btrt = int(input("Enter COM-port bitrate 2400, 4800, "+color.RED+"*9600"+color.END+", 19200, 38400, 57600, 115200: ") or 9600)
#print (btrt)
btrt = 9600
arduino = serial.Serial(port=comn, baudrate=btrt, timeout=.1)
arduino.write(bytes('1', 'utf-8'))
#sensors = []

def write_read():
	arduino.write(bytes('1', 'utf-8'))
	data = arduino.readline()
	return data
	arduino.close()

time.sleep(10)

# MySQL connection
myconn = mysql.connector.connect(host = "localhost", user = "py", passwd = "py_test", database = "py")
cur = myconn.cursor()

while True:
	sensors = (write_read().decode().split(","))
	print(sensors)

	sql = "INSERT INTO `data` (`Temp1`, `Pressure`, `Humidity`, `Temp2`) VALUES (%s, %s, %s, %s);"
	val = (sensors[0],sensors[1],sensors[2],sensors[3])
	try:
		cur.execute(sql,val)
		myconn.commit()
	except:
		myconn.rollback()

	#print(cur.rowcount,"record inserted!")

	time.sleep(90)