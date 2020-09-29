//////////HEADER////////////
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DoubleResetDetect.h>
#include <ArduinoJson.h> //version 5.13.4 
#include <DHT.h> 
#include <Adafruit_Sensor.h>

/////////////DEFINE///////////////
#define DRD_TIMEOUT 2.0
#define DRD_ADDRESS 0x00
#define PORT_REST_HTTP 80
#define SYS_DHT_OUT_PIN 0
#define SYS_DHT_TYPE DHT11

//////////////////////////////////
DHT dht;
WiFiServer server(80);
byte led  = 16;
byte led1 = 14;
byte led2 = 12;
byte led3 = 13;
byte ds   =  2;
ESP8266WebServer http_rest_server(PORT_REST_HTTP);
const byte DNS_PORT = 53; 
DNSServer dnsServer;    

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
       char strs[200];
          jsonObj["id"] = 1;
          jsonObj["temperatur"] =String(tmp);
          jsonObj["kelembapan"] =String(hmdt);
          jsonObj["signalstr"] = String(rsii);
          jsonObj["status"] = 200;
          jsonObj.prettyPrintTo(strs, sizeof(strs));
       http_rest_server.sendHeader("Access-Control-Allow-Methods", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",strs );
    }
}
//FUNCTION RELAY
void relay(){
      String condition =http_rest_server.arg("condition");
     if (condition == "on") {
       Serial.print(condition);
       digitalWrite(led,0x000000000000);
     } else if (condition == "off") {
        Serial.print(condition);
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
       char rt1=digitalRead(led1);
       char rt2=digitalRead(led2);
       char rt3=digitalRead(led3);
       StaticJsonBuffer<200> jsonBuffer;
       JsonObject& jsonObj = jsonBuffer.createObject();
       char msbubb[200];
       jsonObj["status"] = WiFi.status();
       jsonObj["relay1"] =rt;
       jsonObj["relay2"] =rt1;
       jsonObj["relay3"] =rt2;
       jsonObj["relay4"] =rt3;
       jsonObj["ssid"] =q;
       //Show pass if you need it
       
       //jsonObj["pass"] =w;
       jsonObj.prettyPrintTo(msbubb, sizeof(msbubb));
       http_rest_server.sendHeader("Access-Control-Allow-Methods", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Origin", "*");
       http_rest_server.sendHeader("Access-Control-Allow-Headers", "*");
       http_rest_server.send(200, "application/json",msbubb );
}
void rely1(){
        String condition =http_rest_server.arg("condition");
     if (condition == "on") {
       digitalWrite(led1,0x000000000000);
     } else if (condition == "off") {
        digitalWrite(led1,0x000000000001);
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
void rely2(){
        String condition =http_rest_server.arg("condition");
     if (condition == "on") {
       digitalWrite(led2,0x000000000000);
     } else if (condition == "off") {
        digitalWrite(led2,0x000000000001);
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
void rely3(){
        String condition =http_rest_server.arg("condition");
     if (condition == "on") {
       digitalWrite(led3,0x000000000000);
     } else if (condition == "off") {
        digitalWrite(led3,0x000000000001);
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
      //////////////  ROUTING  ///////////////////
void  sys_route_http(){
       http_rest_server.on("/dht", HTTP_GET, get_l);
       http_rest_server.on("/relay",relay);
       http_rest_server.on("/relay1",rely1);
       http_rest_server.on("/relay2",rely2);
       http_rest_server.on("/relay3",rely3);
       http_rest_server.on("/",qwer);
}
     ////////////////////////////////////////////
void setup() {
       ////////////////////Setup Wifimanager
       WiFiManager wifimanager;
       wifimanager.autoConnect("IoT_Dinamika","qwerty123");
       pinMode(led, OUTPUT);
       pinMode(led1, OUTPUT);
       pinMode(led2, OUTPUT);
       pinMode(led3, OUTPUT); //relay mode output
       //dns setup
       ///// Button reset setup
       DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);
   if (drd.detect()) 
    {
        WiFi.disconnect();// Disconect wifi goto back to wifi configuration WiFimanager
      
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

}
void loop() {
  //handeled web server client
  http_rest_server.handleClient();
}
///////////////////////////////////////////////
///////////////////////////////////////////////
// BY RAKHMADI FULLSTACKDEV & Choirul//////////
///////////////////////////////////////////////
///////////////////////////////////////////////
// INPUT   BOARD_LABEL  RAW_PIN_NUM          //
// IN1     D0           16                   //
// IN2     D5           14                   //
// IN3     D6           12                   //
// IN4     D7           13                   //
///////////////////////////////////////////////
//              ________    __________
//             |       |___|         |
//   reset >> _|            D1 mini  |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |                       |
//           |_______________________|