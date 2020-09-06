'''

This is a serial monitor program.  
it is intended to capture input on a python line and store it in a file


'''

#initial funciton to monitor and print serial line data
import serial
import os
import sys

#setup file
out_file="state.csv"
f=open(os.path.join(sys.path[0],out_file),'w')

#setup serial port
_port='COM3'
ser=serial.Serial()
ser.baudrate=115200
ser.port=_port
ser.timeout=10

#open serial port
_op=False
print("State monitoring")
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
		
		line=ser.readline()
		print(line)
		line=line.decode("utf-8")#.replace('\r\n','\n')
		state=line.split("State: ")
		if(len(state)>1):
			print(state[1])
			f.write(state[1])
		i+=1
	except (KeyboardInterrupt, SystemExit):
		print("User Done!")
		_exit=True
	except(UnicodeDecodeError):
		print("unicode error!")
	except:
		print("System Done!")
		raise
		_exit=True

f.close()
