'''
Monitors the output of the tiny-va-meter on com3.
Files is output as .csv 
format: "currentMillis,busvoltage,shuntvoltage,loadvoltage,current_ma,power_mw,mAh"
intended to be viewed in "display" notebook

the code basically waits for the port to open
logs the data until it hits an error or the user hits ctrl+c

'''

#initial funciton to monitor and print serial line data
import serial
import os
import sys

#setup file
out_file="power.csv"
f=open(os.path.join(sys.path[0],out_file),'w')

#setup serial port
_port='COM3'
ser=serial.Serial()
ser.baudrate=115200
ser.port=_port
ser.timeout=5

#open serial port
_op=False
print("Opening "+_port+" not open")	
while (not _op):
	try:
		ser.open()
		_op=True
	except:
		pass	
i=0

#read from serial, write to file
_exit=False
while(not _exit):
	try:
		f.write(ser.readline().decode("utf-8").replace('\r\n','\n'))
		
		i+=1
	except (KeyboardInterrupt, SystemExit):
		print("User Done!")
		_exit=True
	except:
		print("System Done!")
		raise
		_exit=True

f.close()
