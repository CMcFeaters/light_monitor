//program monitors the lightsource and posts it to a web address as a server

#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <string>

//constants
int sensorPin=A0; //this is our analog input sensor
int sensorValue=0;  //this is where we'll store the sensor value


//this is our connection address inforamtion
const char* MY_SSID = "sun_server";
const char* MY_PW = "chuck5621temp279word";
const char* host="192.168.0.10";
const int port = 10000;


//constants for testing on laptop
/*
 * 
 const char* MY_SSID = "FiOS-UJYY9";
const char* MY_PW = "oily233glum9532gap";
const char* host="192.168.1.237";
const int port = 10000;
*/

//globals
boolean gain;
unsigned int ms;


/*
 * This function sets up teh wifi portion of our project
 */
void wifi_setup(){
 // IPAddress ip;
  int attempt=0;

  //WiFi.disconnect();
  WiFi.mode(WIFI_STA);
 
 //external network
 WiFi.begin(MY_SSID,MY_PW);
    IPAddress ip(192,168,0,14);   
    IPAddress gateway(192,168,0,10);   
    IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  
/*fozr internal test
  WiFi.begin(MY_SSID,MY_PW);
    IPAddress ip(192,168,1,225);   
    IPAddress gateway(192,168,1,11);   
    IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet); 
*/
  //attempt connection
  while(WiFi.status()!=WL_CONNECTED && attempt<100000){
    delay(1);
    attempt++;
    //Serial.print(".");
  }
  //Serial.print("Connected in: ");
  //Serial.println(attempt);
  //Serial.print("IP Address: ");
  //Serial.println(WiFi.localIP());

 
}

void work(){
  //read data
  sensorValue=analogRead(sensorPin);
  
  //print data
  //Serial.println("Sensor value: ");
  //Serial.println(sensorValue);
  
  //connect to host
  //Serial.print("connecting to ");
  //Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  //Serial.println("connection success!");
  

  
  //send data
  client.print(String(sensorValue));
    
  unsigned long timeout = millis();
  //wait for response
  while (client.available() == 0) {
    if (millis() - timeout > 50000) {
      //Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  //read response
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);
  }

  client.stop();

 
}

/*
 * This function setsup the light object
 */


void setup() {
  // put your setup code here, to run once:

  /*WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay( 1 );
  
  WiFi.forceSleepWake();
  delay(1);*/
  wifi_setup();

  //read, print and send the analog value
  work();
  /*WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay( 1 );*/
  ESP.deepSleep(300e6);
}



void loop() {
 
}

