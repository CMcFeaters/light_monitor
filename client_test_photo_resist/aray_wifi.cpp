#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <string.h>
#include "array_wifi.h"


my_wifi::my_wifi(char* ssid,char* pw, char* host,int port)	//[stuff]::[thing]  defines thing as part of teh stuff class
{
	/*
	//configure the data needed for wifi
	*/
	_my_ssid=ssid;
	_my_pw=pw;
	_host=host;
	_port=port;
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
	
	WiFi.mode(WIFI_STA);
	WiFi.begin(_my_ssid,_my_pw);
	
	//attempt connection
	while(WiFi.status()!=WL_CONNECTED && attempt<100000){
		delay(10);
		attempt++;
		//Serial.print(".");
	}
	
	//Serial.print("Connected in: ");
	//Serial.println(attempt);
	//Serial.print("IP Address: ");
	//Serial.println(WiFi.localIP());

}

void my_wifi::send_data(uint16_t* data,int size,int sleep_time){
	String packet;
	//connect to host
	//Serial.print("connecting to ");
	//Serial.println(_host);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	
	if (!client.connect(_host,_port)) {
		//Serial.println("connection failed");
		return;
	}
	//Serial.println("connection success!");

	packet=pack_data(data,size,sleep_time);
	//Serial.print("Sending packet: ");
	//Serial.println(packet);
	
	if (not send_packet(client,packet)){
		//Serial.println("Failed on packet");
		client.stop();
		return;
	}

	client.stop();
}

bool my_wifi::send_packet(WiFiClient client, String data){
	
	/*
		sends the "data" over the wificlient
	*/
	//Serial.println("Sending!");
	client.print(data);

	//wait for response
	//if we timeout, exit
	/*unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 50000) {
			//Serial.println(">>> Client Timeout !");
			client.stop();
			return false;
		}
	}*/
	//read response
	/*while(client.available()){
		String line = client.readStringUntil('\r');
		//Serial.print("Received: ");
		//Serial.println(line);
	}*/
	//Serial.print("Send success: ");
	//Serial.println(data);
	
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
