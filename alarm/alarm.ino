#include <WiFi.h>
#include "PubSubClient.h"
#include <M5StickC.h>
const char* ssid = "ArduinoWifiShield";
const char* password = "woufwouf06";
static const char* connectionString = "";
const char* mqtt_server = "192.168.43.254";
const char* alert_topic = "/alarm";
const char* clientID = "alarm";
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
     Serial.println("RECEIVED ALARM ON");
     alarmOn = true;
   } else {
     Serial.println("RECEIVED ALARM OFF");
     alarmOn = false;
   }

}
void setup(){
  Serial.begin(115200);
  M5.begin();
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(spk_pin, channel);
  ledcWrite(channel, 256);
  ledcWriteTone(channel, 0);
  Serial.println("Starting connection WiFi.");
  delay(10);
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
  if(alarmOn == true){
    for (int x = 0; x < 360; x += 20) {
      sinVal = sin(x * (PI / 180));
      toneVal = 2000 + sinVal * 500;
      ledcWriteTone(channel, toneVal);
      delay(15);
      ledcWriteTone(channel, 0);
    }
  }
  if(alarmOn == false){
    ledcWriteTone(channel, 0);
  }
  
}
