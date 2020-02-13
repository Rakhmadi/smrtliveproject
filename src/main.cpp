//////////HEADER////////////
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DoubleResetDetect.h>
#include <ArduinoJson.h> //version 5.13.4 
#include <DHT.h> 
#include <Adafruit_Sensor.h>
#include <stdio.h>
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
/////////////DEFINE///////////////
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
float t;
float h;
BlynkTimer timer;
//////// dht blynk function
void sendSensor()
{
  h = dht.getHumidity();
  t = dht.getTemperature(); 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}
//FUNCTION DHT11
void get_l(){
    if (false)
        http_rest_server.send(204); // :)
    else {
       long rsii= WiFi.RSSI();
       //MENGAMBIL TEMPERATUR& HUMIDTY
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
       http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",JSONmessageBuffer );
    }
}
//FUNCTION RELAY
void relay(){
      String condition =http_rest_server.arg("condition");
     if (condition == "on") {
       digitalWrite(led,0x000000000000);
     } else if (condition == "off") {
        digitalWrite(led,0x000000000001);
     } else {
          
     }
       StaticJsonBuffer<200> jsonBuffer;
       JsonObject& jsonObj = jsonBuffer.createObject();
       char msbubb[200];
       jsonObj["condition"] = condition;
       jsonObj.prettyPrintTo(msbubb, sizeof(msbubb));
       http_rest_server.sendHeader("Access-Control-Allow-Methods", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",msbubb );
}
//FUNCTION HOME
void qwer(){
       String q =WiFi.SSID();
       String w =WiFi.psk();
       char rt=digitalRead(led);
       StaticJsonBuffer<200> jsonBuffer;
       JsonObject& jsonObj = jsonBuffer.createObject();
       char msbubb[200];
       jsonObj["status"] = WiFi.status();
       jsonObj["rely"] =rt;
       jsonObj["ssid"] =q;
       jsonObj["pass"] =w;
       jsonObj.prettyPrintTo(msbubb, sizeof(msbubb));
       http_rest_server.sendHeader("Access-Control-Allow-Methods", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",msbubb );
}
      //////////////  ROUTING  ///////////////////
void  sys_route_http(){
       http_rest_server.on("/dht", HTTP_GET, get_l);
       http_rest_server.on("/relay",relay);
       http_rest_server.on("/",qwer);
}
      ////////////////////////////////////////////
void setup() {
       ////////////////////Setup Wifimanager
       WiFiManager wifimanager;
       /////// blynk token
       char auth[] ="Your Auth. Key";
       ////
       wifimanager.autoConnect("IoT_Dinamika","qwerty123");
       pinMode(led, OUTPUT); //relay mode output
       ///// Button reset setup
       DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);
   if (drd.detect()) 
    {
        WiFi.disconnect();// Disconect wifi goto back to wifi configuration WiFimanager
        Serial.print("");
    }else{
      /////////// kosongkan saja
    }
  //mendefinisikan routing
  sys_route_http();
  //webserver begin
  http_rest_server.begin();
  //DHTSETUP
  dht.setup(SYS_DHT_OUT_PIN);
  //SERVER BEGIN
  server.begin();
  //blynk setup
   char ssid[50];
   char pass[50];
   String a= WiFi.SSID();
    a.toCharArray(ssid, 50);
   String b=WiFi.psk();
    b.toCharArray(pass, 50);
   Blynk.begin(auth, ssid, pass);
  /////blynk interval
  timer.setInterval(1000L, sendSensor);
  ///////
}
void loop() {
  //handeled web server client
  http_rest_server.handleClient();
  delay(3000);
   Blynk.run();
  timer.run();
}
///////////////////////////////////////////////
///////////////////////////////////////////////
// BY RAKHMADI FULLSTACKDEV & Choirul//////////
///////////////////////////////////////////////
///////////////////////////////////////////////