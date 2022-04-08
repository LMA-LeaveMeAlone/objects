#include <WiFi.h>
#include "PubSubClient.h"

const char* ssid = "ArduinoWifiShield";
const char* password = "woufwouf06";

static const char* connectionString = "";

const char* mqtt_server = "192.168.43.254";
const char* topic = "/sensor/movement";

const char* clientID = "sensor_movement";

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

float t;

void setup(){
  Serial.begin(115200);
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

  if(client.connect(clientID)){
    Serial.println("Connected to MQTT Broker!");
    sendAlert();
  }else{
    Serial.print("Connection to MQTT Broker failed...");
  }
}

void sendAlert(){
  if(client.publish(topic, "alert")){
    Serial.println("alert sent!");
  }
}

void loop(){
  Serial.println("start sending events.");
  delay(5000);
}
