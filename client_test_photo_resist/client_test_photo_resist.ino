  //program monitors the lightsource and posts it to a web address as a server
#include <Wire.h>

#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include <string>
#include "rtc_mem.h"
#include "array_wifi.h"

//constants
const int sensorPin=A0; //this is our analog input sensor
const int sleep_time=120; //sleep time in seconds 
const int buttonPin=13 ; //this is the button pin
const int button_on=0;  //1 this board has a button, 0 this board does not have a button
const int test_var=0; //1 puts it in test mode, anything else puts it in regular mode
int sleep_time_ns=sleep_time*1e6; //sleep time in ns

/*
 * If buttonpin is low, the initial cycle the device will make it's reading, set the rtcData.button value to high and go to sleep
 * for a brief period of time.  upon wakeup and rtcData.button high the device will send it's data, reset count to 0, and go to sleep with wifi-off for a brief period 
 * Whiel rtcData.button is high, and blink an LED until the button is pulledhigh
 * When the button pin is pulled high and rtcData.button is high, rtcData.button is set to 0, and the device sleeps into no-wifi mode for the normal delay cycle 
 * this brings us back to normal.
 */


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

  
  bool state_1=false;
  /*
   * For state_1, we will do our last data recording, we will send our data next time
   * We need to update the get_ready_to_send bit, allow the device to capture data as usual
   * if we send data this round, move straight to state_2.
   */
  bool state_2=false;
  /*
   * For state_2, wifi is now enabled.  send the data, update the "sent" bit, sleep with wifi off
   * remain in 
   * 
   */

   bool state_3=false;
   /*
    * for state_3 we have sent the data and are waiting for the user to undo the pin
    */
  //Serial.begin(115200);
  //Serial.println();
  //delay(1000);
  //Serial.println(ESP.getFullVersion());
  RTC_MEM rtcMem;
  //if the button is depressed and we haven't had it recorded as such
  if (button_on){
    pinMode(buttonPin,INPUT);
    bool buttonState=(bool)digitalRead(buttonPin);
    if ((!buttonState)&&(!rtcMem.rtcData.get_ready_to_send)){
        state_1=true;
        rtcMem.rtcData.get_ready_to_send=true;
    }else if ((!buttonState)&&(!rtcMem.rtcData.sent)){
      state_2=true;
      rtcMem.rtcData.sent=true;
    }else if((!buttonState)&&(rtcMem.rtcData.sent)){
      state_3=true;
    }else if (buttonState){
      rtcMem.rtcData.sent=false;
      rtcMem.rtcData.get_ready_to_send=false;
    }
  }
  
  
  if ((!state_3) && ((rtcMem.write_to_RTC_MEM((uint16_t)(analogRead(sensorPin))))||(state_2))){
    //Serial.println("Time To Broadcast");
    my_wifi temp_wifi=set_ip();
    temp_wifi.connect_to_server();
    temp_wifi.send_data((uint16_t*)&rtcMem.rtcData.data,rtcMem.rtcData.count,sleep_time);
    rtcMem.rtcData.count=0;
    rtcMem.one_away=false;
    rtcMem.write_to_RTC_MEM_no_data();
    rtcMem.print_RTC_MEM();  

  }
  
  if (state_1){ 
    //Serial.println("We are in state_1, the button was just pressed this round");
    ESP.deepSleep(1E6); //sleep for 1 second and awaken with RF enabled to broadcast
  }else if (state_2){
    //Serial.println("We are in state_2, the data was sent");
    ESP.deepSleep(1E6,RF_DISABLED);
  }else if (state_3){
    //Serial.println("We are in state_3, the user needs to disable the button");
    ESP.deepSleep(1E6,RF_DISABLED);
  }else if (rtcMem.one_away){
    //If we are going to wake up and connect, allow wifi
    //Serial.println("SENDING NEXT TIME");
    ESP.deepSleep(sleep_time_ns);
  }else{
    //if we are going to wakeup and just record, disable wifi
    //Serial.println("We are not sending next time");
     //ESP.deepSleep(sleep_time_ns);
      ESP.deepSleep(sleep_time_ns,RF_DISABLED);
  }
  //ESP.deepSleep(sleep_time_ns);
  
}



void loop() {
 
}
