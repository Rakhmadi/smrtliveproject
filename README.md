# Hi There

Welcome my IoT project 

## Installation

use Extension PlatformIo in visual studio code
### location main code in src/main.cpp 
----
### Header required
``` 
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
```

## Api 

```python
  Get Method 
  Format json

 /dht                 >> get temperature and humidity
 /                    >> get status wifi and relay condition 1 = false 0=true
 /relay?condition=    >> activate relay value on/off 
```
### By Rakhmadi 
## master in https://github.com/Rakhmadi/smrtliveproject