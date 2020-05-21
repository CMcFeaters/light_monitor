/*
	test.h - library for testing libraries
*/

#ifndef array_wifi_h
	#define array_wifi_h
	#include <WiFiClient.h>
	#include "Arduino.h"
	class my_wifi
	{
		public:	//publically accessible stuff
			my_wifi(char* ssid,char* pw, char* host,int port);	//initiate, assigns all wifi data to private variables

			void setup_wifi(); 						//configures the wifi and 
			void connect_to_server();				//connect to the wifi server
			void static_IP(IPAddress my_IP,IPAddress my_gateway);	//setup the static IP
			void send_data(uint16_t* data,int size,int sleep_time);	//sends the pointed to data to the location
			bool send_packet(WiFiClient client, String data);//sends an individual packet
			String pack_data(uint16_t* data, int size,int sleep_time);//returns a string that details the packet and contains the data
			
		private:
			char* _my_ssid; 
			char* _my_pw;
			char* _host;
			int _port;
			IPAddress _my_IP;
			IPAddress _my_gateway;

			
	};

#endif

