#!/usr/bin/python3			 # This is server.py file
import socket										  
import datetime
import os
import sys
from pathvalidate import sanitize_filename

#setup folder
def setup_folders(folder_name):
	f_name=sanitize_filename(folder_name)
	x=os.listdir(os.getcwd())
	if x.count(f_name)<1:
		fpath="./"+f_name
		os.makedirs(fpath)
	fpath="./"+f_name+"/"
	return fpath

def begin_sockets(fpath):
	serversocket = socket.socket(
				socket.AF_INET, socket.SOCK_STREAM) 
	# get local machine name
	host = socket.gethostname()							  
	port = 10000										   

	# bind to the port
	serversocket.bind(('', port))								   
	#serversocket.settimeout(5)	#300s timeout
	serversocket.settimeout(600)
	# queue up to 5 requests
	serversocket.listen(5)											 
	print("Serving at: ",host,":",port)


	count=0
	while True:
	   # establish a connection		
		print('Waiting for client to connect to ',host,":",port)
		clientsocket,addr = serversocket.accept()	 
		try:

			print('connection from', addr)

			# Receive the data in small chunks and retransmit it
			#while True:
			print("Waiting for client to transmit")
			data = clientsocket.recv(16)
			print('received {!r}'.format(data))
			if data:
				time=datetime.datetime.now()
				f_name="%s%s_%s"%(fpath,time.strftime("%Y%m%d"),"data_file.txt")
				f=open(f_name,"a+")

				data_string="%d,%s,%d\n"%(count,time.strftime("%Y-%m-%d,%H:%M:%S"),int(data))
				print("Writing: ",data_string)
				f.write(data_string)
				f.close()
				print("Write Complete")
				clientsocket.sendall(data)
				clientsocket.close()
				count+=1

			else:
				print('no data from', addr)
				break

		
		finally:
			print('Socket_closed!')
			clientsocket.close()
			
if (len(sys.argv)<2):
	begin_sockets("./")
else:
	begin_sockets(setup_folders(sys.argv[1]))
	