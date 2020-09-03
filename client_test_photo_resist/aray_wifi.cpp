#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <string.h>
#include "array_wifi.h"


my_wifi::my_wifi(char* ssid,char* pw, char* host,int port, IPAddress my_ip,IPAddress  my_gateway)	//[stuff]::[thing]  defines thing as part of teh stuff class
{
	/*
	//configure the data needed for wifi
	*/
	_my_ssid=ssid;
	_my_pw=pw;
	_host=host;
	_port=port;
	_my_gateway=my_gateway;
	_my_IP=my_ip;
	Serial.println(_my_IP);
	Serial.println(_my_gateway);
}

void my_wifi::static_IP(IPAddress my_IP, IPAddress my_gateway)
{
	/*
	setup the wifi equipment to connect as needed.
	*/
	_my_IP=my_IP;
	_my_gateway=my_gateway;
	IPAddress _my_subnet(255,255,255,0); 
	WiFi.config(_my_IP, _my_gateway, _my_subnet);
}


void my_wifi::connect_to_server(){
	int attempt=0;
	
	/*
		save the flash memory!
	*/
	IPAddress _my_subnet(255,255,255,0);
	Serial.println(_my_IP);
	Serial.println(_my_gateway);
	Serial.println(_my_subnet);
	WiFi.forceSleepWake();
	delay(1);
	WiFi.persistent( false );
	WiFi.mode(WIFI_STA);
	WiFi.config(_my_IP, _my_gateway, _my_subnet);
	WiFi.begin(_my_ssid,_my_pw);
	
	//attempt connection
	while(WiFi.status()!=WL_CONNECTED && attempt<100000){
		delay(500);
		attempt++;
		Serial.print(".");
	}
	
	Serial.print("Connected in: ");
	Serial.println(attempt);
	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());

}

void my_wifi::send_data(uint16_t* data,int size,int sleep_time, bool night_mode){
	String packet;
	//connect to host
	Serial.print("connecting to ");
	Serial.println(_host);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	
	if (!client.connect(_host,_port)) {
		Serial.println("connection failed");
		return;
	}
	Serial.println("connection success!");

	packet=pack_data(data,size,sleep_time);
	Serial.print("Sending packet: ");
	Serial.println(packet);
	
	if (not send_packet(client,packet,night_mode)){
		Serial.println("Failed on packet");
		client.stop();
		return;
	}

	client.stop();
}

bool my_wifi::send_packet(WiFiClient client, String data, bool night_mode){
	
	/*
		sends the "data" over the wificlient
		if night_mode is enabled, will return true/false on if the device should go to sleep
	*/
	Serial.println("Sending!");
	client.print(data);

	//wait for response
	//if we timeout, exit
	/*unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 50000) {
			Serial.println(">>> Client Timeout !");
			client.stop();
			return false;
		}
	}*/
	//read response
	if (night_mode){
    String line;
		while(client.available()){
			line = client.readStringUntil('\r');
		Serial.print("Received: ");
		Serial.println(line);
		}
		
		if (line.toInt()<WAKE_TIME || line.toInt()>BED_TIME){
			return true;
		}
		//we shoudl recieve "hourminute"
		//convert stoi(hourminute)
		//and tehn we return true if hm>bed_time or hm<wake_time
	}
	Serial.print("Send success: ");
	Serial.println(data);
	
	return true;
}

String my_wifi::pack_data(uint16_t* data, int size, int sleep_time){
	/*
		Packs the data, size and ssleep time into a single string to be sent
		over wifi.  includes building header
	*/
	
	/*
		header contains "s,[number],[time]" 
		[number] is the number of data blocks being sent
		[time] is the number of seconds between each block
	*/
	String output="s,"+String(size)+","+sleep_time;
	for (int i=0;i<size;i++){
		output+=","+String(data[i]);
	}
	return output;	
}
