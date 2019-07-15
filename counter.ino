//For Pinging
#include <ESP8266Ping.h>

//Adafruit MQQT
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266WiFi.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "YOURUSERNAMEHERE"
#define AIO_KEY  "YOURKEYHERE"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

IPAddress staticIP980_10(172,16,11,200);
IPAddress gateway980_10(172,16,11,1);
IPAddress subnet980_10(255,255,255,0);

boolean MQTT_connect();

boolean MQTT_connect() {  int8_t ret; if (mqtt.connected()) {    return true; }  uint8_t retries = 3;  while ((ret = mqtt.connect()) != 0) { mqtt.disconnect(); delay(2000);  retries--;if (retries == 0) { return false; }} return true;}

Adafruit_MQTT_Publish Connections = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/quiet");

#define SSID "YOURSSID"
#define SSID_PASSWORD "YOUPASSWORD"

int connections = 0;
unsigned long startTime, endTime;

void setup()
{
  Serial.begin(9600);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin(SSID,SSID_PASSWORD);
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");
  }
  Serial.println("Connected");
  WiFi.config(staticIP980_10, gateway980_10, subnet980_10);
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));
  startTime = millis();
}


void loop()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Network Not Available");
    delay(60000);
    Serial.println("Retrying to connect to WiFi Network");
    Serial.println("START");
    WiFi.begin(SSID,SSID_PASSWORD);
    while ((!(WiFi.status() == WL_CONNECTED))){
      delay(300);
      Serial.print("..");
    }
    Serial.println("Connected");
    WiFi.config(staticIP980_10, gateway980_10, subnet980_10);
    Serial.println("Your IP is");
    Serial.println((WiFi.localIP().toString()));
    startTime = millis();
  }
  
  for(int i=2;i<255;i++)
  {
    IPAddress remote_ip(172, 16, 11, i);
    Serial.print("Pinging ip ");
    Serial.println(remote_ip); 

    if(Ping.ping(remote_ip,5)) 
    {
      Serial.println("Ping YES");
      connections++;
    }
    else 
    {
      Serial.println("no Ping");
    } 
  }

  
  
  
  Serial.println("Waiting to send data......!!!");
  endTime = millis();
  while(endTime<(startTime+(3000)))
  {
    endTime=millis();
    delay(100);
  }
  
  if (MQTT_connect()) {
    if (Connections.publish(connections)) {
      Serial.println("Feed Sent. No of connections: ");
      Serial.print(connections);
    } else {
      Serial.print("The Feed was not Sent");
    }
  }
  connections = 0;
  startTime = millis();
}
