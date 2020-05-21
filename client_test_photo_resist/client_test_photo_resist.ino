//program monitors the lightsource and posts it to a web address as a server
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include <string>
#include "rtc_mem.h"
#include "array_wifi.h"

//constants
const int sensorPin=A0; //this is our analog input sensor
const int sleep_time=1; //sleep time in seconds 
const int buttonPin=D3; //this is the button pin

//global variables
const int test_var=1; //1 puts it in test mode, anything else puts it in regular mode
int sleep_time_ns=sleep_time*1e6; //sleep time in ns


/*
 * IP assigning function, right now this is just used to determine if we 
 * are testing or running
 */
my_wifi set_ip(){
 
  char* MY_SSID;
  char* MY_PW;
  char* host;
  int port;
  if (test_var==1){
    //internal network
     MY_SSID = "FiOS-UJYY9";
     MY_PW = "oily233glum9532gap";
     host="192.168.1.237";
     port = 10000;
      WiFi.begin(MY_SSID,MY_PW);
      IPAddress ip(192,168,1,225);   
      IPAddress gateway(192,168,1,11);   
      IPAddress subnet(255,255,255,0);   
    WiFi.config(ip, gateway, subnet);
    
  }else{
     //external network
    MY_SSID = "sun_server";
    MY_PW = "chuck5621temp279word";
    host="192.168.0.10";
    port = 10000;
    IPAddress ip(192,168,0,14);   
    IPAddress gateway(192,168,0,10);   
    IPAddress subnet(255,255,255,0);   
    WiFi.config(ip, gateway, subnet);
  }
  return my_wifi(MY_SSID,MY_PW, host,port);
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Serial.println(ESP.getFullVersion());
  RTC_MEM rtcMem;
  if ( rtcMem.write_to_RTC_MEM((uint16_t)(analogRead(sensorPin)))){
    //Serial.println("Time To Broadcast");
    //rtcMem.read_from_RTC_MEM();
    
    my_wifi temp_wifi=set_ip();
    temp_wifi.connect_to_server();
    temp_wifi.send_data((uint16_t*)&rtcMem.rtcData.data,rtcMem.countLimit,sleep_time);
    rtcMem.print_RTC_MEM();  
    
  }
  
 
  

  //go to sleep
 /* if (test_var==1){
    Serial.begin(9600);
      while (!Serial) {
       ; // wait for serial port to connect. Needed for native USB port only
      }
      Serial.println("TEST"); //clear the trash from the serial line

  }
  */
 //Serial.println(rtcMem.one_away);

 if (rtcMem.one_away){
    //If we are going to wake up and connect, allow wifi
    Serial.println("SENDING NEXT TIME");
    ESP.deepSleep(sleep_time_ns);
  }else{
    //if we are going to wakeup and just record, disable wifi
    Serial.println("We are not sending next time");
     //ESP.deepSleep(sleep_time_ns);
      ESP.deepSleep(sleep_time_ns,RF_DISABLED);
  }
  
}



void loop() {
 
}
