/*
 * Author: Varad More
 * Compiled on: 21/04/2021
 *  
 */

#include <Wire.h>
#include <ESP8266WiFi.h>


//---------------------Channel Settings--------------
String apiKey = "9MPJEMI5ICQD1NY3";     //  Enter your Write API key from ThingSpeak
const char* server = "api.thingspeak.com";

//---------------------WIFI Settings--------------
const char *ssid =  "More";     // replace with your wifi ssid and wpa2 key
const char *pass =  "99700981";
WiFiClient client;


const int ledPin = 5;
const int ldrPin = A0;

const int AirValue = 790;   //you need to replace this value with Value_1
const int WaterValue = 390;  //you need to replace this value with Value_2
const int SensorPin = D0;
int soilMoistureValue = 0;
int soilmoisturepercent=0;


void setup() {

Serial.begin(9600);
  Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

delay(500);
}

void loop() {

// Soil Moisture Sensor

//soilmoisturepercent = soil_moisture_sensor();
//Serial.print("Final Value: ");
//Serial.println (soilmoisturepercent);

int ldrStatus = analogRead(ldrPin);
Serial.println(ldrStatus);
send_thingspeak(ldrStatus);


delay(1000);
}


int soil_moisture_sensor (){
  soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
  //  soilmoisturepercent = 100 - ((soilMoistureValue /1023) *100);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.println(soilmoisturepercent);
  return soilmoisturepercent;
}


void send_thingspeak(int ldr){

    if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(ldr);
//                             postStr +="&field2=";
//                             postStr += String(soil);
//                             postStr +="&field3=";
//                             postStr += String(Az);
                             Serial.println (postStr);
                             postStr += "\r\n\r\n";
                 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
         client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
         client.print("Content-Type: application/x-www-form-urlencoded\n");
         client.print("Content-Length: ");
         client.print(postStr.length());
         client.print("\n\n");
         client.print(postStr);
 
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 

delay(1000);
}
