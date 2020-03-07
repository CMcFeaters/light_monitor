//program monitors the lightsource and posts it to a web address as a server

#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <string>

//constants
const char* MY_SSID = "sun_server";
const char* MY_PW = "chuck5621temp279word";
const char* host="192.168.0.10";
const int port = 10000;

SFE_TSL2561 light; //the light object


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
  WiFi.begin(MY_SSID,MY_PW);
    IPAddress ip(192,168,0,14);   
    IPAddress gateway(192,168,0,10);   
    IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  
  //attempt connection
  while(WiFi.status()!=WL_CONNECTED && attempt<100000){
    delay(1);
    attempt++;
    //Serial.print(".");
  }
  Serial.print("Connected in: ");
  Serial.println(attempt);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

 
}

void work(int l_value){
    //connect to host
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  //send data
  Serial.println("connection success!");
  client.print(String(l_value));
    
  unsigned long timeout = millis();
  //wait for response
  while (client.available() == 0) {
    if (millis() - timeout > 50000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  //read response
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  client.stop();

 
}

/*
 * This function setsup the light object
 */
void light_setup(){
   
  light.begin();

  unsigned char ID;
  
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
  }
  else
  {
    byte error = light.getError();
    printError(error);
  }

  gain=0;

  unsigned char time=0;

  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
  delay(15);
  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.

}

void printError(byte error)
  // If there's an I2C error, this function will
  // print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

int print_light(){
  //this is the function that is accessed when we go to "/" at the server
  unsigned int data0, data1;
  double lux;
  boolean good;
  String test;
  
  //light.manualStart();
  //delay(130);
  //light.manualStop();

  light.getData(data0,data1);
  good=light.getLux(gain,ms,data0,data1,lux);
  //test=String(lux);
  //server.send(200,"text/plain",test);
  Serial.print(" lux : ");
  Serial.print(lux);
  
  return lux;

  
}


void setup() {
  // put your setup code here, to run once:

  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay( 1 );
  
  //Serial.begin(9600);
  //light sensor stuff
  light_setup();
  //delay(250);
  int l_value;
  l_value=print_light();
  //delay(1000);
  light.setPowerDown();//shutoff
  
  //Serial.println(l_value);
  WiFi.forceSleepWake();
  delay(1);
  wifi_setup();
  work(l_value);
  //Serial.println("SLEEPING");
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay( 1 );
  ESP.deepSleep(300e6);
}



void loop() {
 
}

