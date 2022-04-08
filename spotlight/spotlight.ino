#include <WiFi.h>
#include "PubSubClient.h"
#include <M5Atom.h>
const char* ssid = "ArduinoWifiShield";
const char* password = "woufwouf06";
static const char* connectionString = "";
const char* mqtt_server = "192.168.43.254";
const char* alert_topic = "/spotlight";
const char* clientID = "spotlight";
const int spk_pin = 26;
int freq = 30;
int channel = 0;
int resolution = 15;
float sinVal;       
int toneVal; 
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);
int alarmOn = false;

void callback(char* topic, byte *payload, unsigned int length){ 
  Serial.println("-------Nouveau message du broker mqtt-------");
  if ((char)payload[0] == '1') {
     Serial.println("RECEIVED SPOTLIGHT ON");
     M5.dis.fillpix(0xffffff);
   } else {
     Serial.println("RECEIVED SPOTLIGHT OFF");
     M5.dis.fillpix(0x000000);
   }
}
void setup(){
  Serial.begin(115200);
  M5.begin(true, false, true);
  delay(50);
  M5.dis.fillpix(0x000000);
  Serial.println("Starting connection WiFi.");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
  client.setCallback(callback);
  if(client.connect(clientID)){
    Serial.println("Connected to MQTT Broker!");  
  }else{
    Serial.print("Connection to MQTT Broker failed...");
  }
  client.subscribe(alert_topic);
}



void loop(){
  client.loop();
}
