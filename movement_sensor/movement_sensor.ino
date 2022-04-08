#include <M5StickC.h>
#include <WiFi.h>
#include "PubSubClient.h"

const char* ssid = "ArduinoWifiShield";
const char* password = "woufwouf06";

static const char* connectionString = "";

const char* mqtt_server = "192.168.43.254";
const char* topic = "/sensor/movement";

const char* clientID = "sensor_movement";

boolean isSensorActive = true;

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
  }else{
    Serial.print("Connection to MQTT Broker failed...");
  }

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(GREEN);

  pinMode(36, INPUT_PULLUP);
}

void loop(){
  //debug();
  if(digitalRead(36)){
    Serial.print("MOVEMENT DETECTED - VALUE : ");
    Serial.println(digitalRead(36));
    
    client.publish(topic, "alert");
    
    M5.Lcd.fillScreen(RED); //Alerte visuelle (test de dev)
    Serial.println("movement detector going afk");
    delay(5000); //5 secondes
    M5.Lcd.fillScreen(GREEN);
    
    delay(5000);
    Serial.println("movement detector going active");
  }
}

void debug(){
  Serial.println("----- DEBUG -----");
  Serial.print("PIR state : ");
  Serial.println(digitalRead(36));
  Serial.println("-----------------");
  delay(500);
}
