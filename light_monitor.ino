//program monitors the lightsource and posts it to a web address as a server

#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

//constants
const char* MY_SSID = "FiOS-UJYY9";
const char* MY_PW = "oily233glum9532gap";


SFE_TSL2561 light; //the light object
ESP8266WebServer server(80); //our server/port


//globals
boolean gain;
unsigned int ms;

void handleRoot();
void handleNotFound();
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
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else
  {
    byte error = light.getError();
    printError(error);
  }

  gain=0;

  unsigned char time=2;

  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
  
  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.

}
/*
 * This function sets up teh wifi portion of our project
 */
void wifi_setup(){
  IPAddress ip;
  int attempt=0;

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(MY_SSID,MY_PW);

  //attempt connection
  while(WiFi.status()!=WL_CONNECTED && attempt<10){
    delay(1000);
    attempt++;
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("HTTP server started");  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  light_setup();
  wifi_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
/*
  unsigned int data0,data1;

  if (light.getData(data0,data1))
  {
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1); 
    double lux;
    boolean good;
  
    good=light.getLux(gain,ms,data0,data1,lux);
  
    Serial.print(" lux : ");
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else
    {
      // getData() returned false because of an I2C error, inform the user.
  
      byte error = light.getError();
      printError(error);
    }
    */
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

void handleRoot(){
  //this is the function that is accessed when we go to "/" at the server
  unsigned int data0, data1;
  double lux;
  boolean good;
  String test;
  
  light.getData(data0,data1);
  good=light.getLux(gain,ms,data0,data1,lux);
  test=String(lux);
  server.send(200,"text/plain",test);
  Serial.print(" lux : ");
  Serial.print(lux);

  
}


void handleNotFound() {
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
 // digitalWrite(led, 0);
}

