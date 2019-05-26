import socket
'''
HOST='192.168.1.206'
PORT=80
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((HOST, PORT))
	s.sendall(b'Hello, world')
	data = s.recv(1024)

print('Received', repr(data))
'''
import urllib.request
url = "http://192.168.1.206/"  # ESP's url, ex: https://192.168.102/ (Esp serial prints it when connected to wifi)

def get_data():
	global data

	n = urllib.request.urlopen(url).read() # get the raw html data in bytes (sends request and warn our esp8266)
	n = n.decode("utf-8") # convert raw html bytes format to string :3
	
	data = n
#	data = n.split() 			#<optional> split datas we got. (if you programmed it to send more than one value) It splits them into seperate list elements.
#	data = list(map(int, data)) #<optional> turn datas to integers, now all list elements are integers.

# Example usage
while True:
	get_data()
	print("Your data(s) which we received from arduino: "+data)
	input("To test it again press enter.")