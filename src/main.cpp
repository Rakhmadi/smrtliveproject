
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DoubleResetDetect.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <stdio.h>

///////////////////////////////////
#define DRD_TIMEOUT 2.0
#define DRD_ADDRESS 0x00
#define PORT_REST_HTTP 80
#define SYS_DHT_OUT_PIN 0
#define SYS_DHT_TYPE DHT11
//////////////////////////////////
DHT dht;

WiFiServer server(80);
byte led = 16;
byte ds = 2;
ESP8266WebServer http_rest_server(PORT_REST_HTTP);
void get_l(){
    if (false)
        http_rest_server.send(204);
    else {
      long rsii= WiFi.RSSI();
      float tmp =dht.getTemperature();
      float hmdt=dht.getHumidity();
       StaticJsonBuffer<200> jsonBuffer;
    JsonObject& jsonObj = jsonBuffer.createObject();
    char JSONmessageBuffer[200];
       jsonObj["id"] = 1;
       jsonObj["temperatur"] =tmp;
       jsonObj["kelembapan"] =hmdt;
       jsonObj["signalstr"] = rsii;
       jsonObj["status"] = 200;
       jsonObj.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
       http_rest_server.sendHeader("Access-Control-Allow-Methods", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",JSONmessageBuffer );
    }
}
void relay(){
      String condition =http_rest_server.arg("condition");
     if (condition == "on") {
       digitalWrite(led,0x000000000000);
     } else if (condition == "off") {
        digitalWrite(led,0x000000000001);
     } else {
      char err[] = "{[\"msg\":\"err_params\"]}"; 
      http_rest_server.send(400, "application/json",err );
     }
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& jsonObj = jsonBuffer.createObject();
    char msbubb[200];
       jsonObj["condition"] = condition;
       jsonObj.prettyPrintTo(msbubb, sizeof(msbubb));
       http_rest_server.sendHeader("Access-Control-Allow-Methods", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",msbubb );
}
//////////////  ROUTING  ///////////////////
void  sys_route_http(){
       http_rest_server.on("/dht", HTTP_GET, get_l);
       http_rest_server.on("/relay",relay);
}
////////////////////////////////////////////

void setup() {
  WiFiManager wifimanager;
  wifimanager.autoConnect("IoT_CoNfT_","nulllablestc");
  pinMode(led, OUTPUT);
   DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);
   if (drd.detect())
    {
        WiFi.disconnect();
        Serial.print("");
    }else{

    }
  sys_route_http();
  http_rest_server.begin();
  dht.setup(SYS_DHT_OUT_PIN);
  server.begin();
}
void loop() {
  http_rest_server.handleClient();
}