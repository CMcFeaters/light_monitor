#!/usr/bin/python3			 # This is server4.py file
#this file receives the data in bursts
import socket										  
import datetime
import os
import sys
from pathvalidate import sanitize_filename

#setup folder
#this funciton setsup the folder to store the data in
def setup_folders(folder_name):
	f_name=sanitize_filename(folder_name)
	x=os.listdir(os.getcwd())
	if x.count(f_name)<1:
		fpath="./"+f_name
		os.makedirs(fpath)
	fpath="./"+f_name+"/"
	return fpath


#this function establishes a connection with the client using sockets
#default port is 1000
def begin_sockets(fpath):
	'''
	This section establishes the sockets, sets any timeout settings, etc
	'''
	serversocket = socket.socket(
				socket.AF_INET, socket.SOCK_STREAM) 
	# get local machine name
	host = socket.gethostname()							  
	port = 10000										   

	# bind to the port
	serversocket.bind(('', port))								   
	#serversocket.settimeout(5)	#300s timeout
	#serversocket.settimeout(6000)
	# queue up to 5 requests
	serversocket.listen(5)											 
	print("Serving at: ",host,":",port)

	'''
	this section sets the server to operator in recieve mode
	waits for a connection and recieves client data
	writes data to designated file and displays diagnostic information
	then closes socket
	'''
	count=0

	while True:
	   # establish a connection	
		data_array=[]
		done=False
		print('Waiting for client to connect to ',host,":",port)
		clientsocket,addr = serversocket.accept()	
		#clientsocket.settimeout(20)
		
		try:

			print('connection from', addr)
			print("Waiting for client to transmit")
			rec_string="";
			#recieved the header
			while not done:
				data= clientsocket.recv(16)
				print('received {!r}'.format(data))
				if data:
					data1=data.decode("utf-8") 
					temp=str(data1).split(",")
					if temp[0]=="s":
						#we have a header
						[check,num_packets,time_size]=[temp[0],int(temp[1]),int(temp[2])]
					rec_string+=data1
					parts=rec_string.split(",")
					print(len(parts))
					print(num_packets+3)
					if len(parts)==num_packets+3:
						done=True
						print("Recieved it all!")
						print(parts)

					

					#clientsocket.sendall(data)
				else:
					print('no data from', addr)
					break
			
			'''
			if len(data_array)==int(num_packets):
				# we have recieved this full submission of packets, write to file
				
				clientsocket.close()
				done=True
			'''
		

	
		finally:
		#release any used resources
			print('Socket_closed!')
			clientsocket.close()
			data_array=[parts[i] for i in range(3,num_packets+3)]
			print(data_array)
			count=write_data(data_array,int(time_size),fpath,count)
			print('Data Written!')

#read command line, expect data to inlcude file path			

def write_data(data,time_size,fpath,count):
	'''
	Recieves a data array, a time size and a filepath
	writes each element in the data array to a filepath
	uses time_size as a timedelta to calculate when the 
	measurement was taken.
	
	data is in reverse order (most recent last)
	'''
	delta=datetime.timedelta(seconds=time_size);
	cur_time=datetime.datetime.now()	#the current time
	num_elements=len(data)
	
	for i in range(num_elements):
		
		time=cur_time-(num_elements-i-1)*delta
		file_name="%s%s_%s"%(fpath,time.strftime("%Y%m%d"),"data_file.txt")
		f=open(file_name,"a+")

		#need to receive as an array, then close the socket and write
		data_string="%d,%s,%d\n"%(count,time.strftime("%Y-%m-%d,%H:%M:%S"),int(data[i]))
		print("Writing: ",data_string)
		f.write(data_string)
		f.close()
		print("Write Complete")
		count+=1
		
	return count
if (len(sys.argv)<2):
	begin_sockets("./")
else:
	begin_sockets(setup_folders(sys.argv[1]))
	