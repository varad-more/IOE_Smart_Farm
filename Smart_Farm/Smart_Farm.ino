/*
 * Author: Varad More
 * Compiled on: 21/04/2021
 *  
 */

#include <Wire.h>
#include <ESP8266WiFi.h>

#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11// DHT 11

#define dht_dpin 0

#include <Servo.h>

Servo servo;

//---------------------Channel Settings--------------
String apiKey = "9MPJEMI5ICQD1NY3"; //  Enter your Write API key from ThingSpeak
const char * server = "api.thingspeak.com";

//---------------------WIFI Settings--------------
const char * ssid = "More"; // replace with your wifi ssid and wpa2 key
const char * pass = "99700981";
WiFiClient client;

int relayInput = 2; // the input to the relay pin


const int AirValue = 790; //you need to replace this value with Value_1
const int WaterValue = 390; //you need to replace this value with Value_2
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent = 0;


DHT dht(dht_dpin, DHTTYPE); 


void setup() {
  Serial.begin(9600);

  dht.begin();
  
  pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT

  Serial.println("Connecting to ");
  Serial.println(ssid);

  servo.attach(5); //D1

  servo.write(0);

         WiFi.begin(ssid, pass);
   
        while (WiFi.status() != WL_CONNECTED) 
       {
              delay(500);
              Serial.print(".");
       }
        Serial.println("");
        Serial.println("WiFi connected");

  delay(1000);
}

void loop() {

  // DHT 11 Sensor
       
  float h = dht.readHumidity();
  float t = dht.readTemperature();         
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t); 
  Serial.println("C  ");

  // LDR Sensor Modules

  // Analog mode
//  int ldrStatus = analogRead(ldrPin);
//  Serial.print("LDR:");
//  Serial.println(ldrStatus);
  Serial.print("LDR:");  
  // Digital Mode
  int ldrstatus =0;
  if (digitalRead(D7) ==HIGH){
    ldrstatus = 1;
    Serial.println("High Light Intensity");
  }
  else {
    ldrstatus =0;
    Serial.println("Low Light Intensity");

  }

//  Serial.println (ldrstatus);



  // Relay turned on for switching input
//  digitalWrite(relayInput, HIGH); // turn relay on
  
  // Soil Moisture Sensor
  int soilmoisturepercent = soil_moisture_sensor();
  Serial.print("Soil Sensor: ");
  Serial.println (soilmoisturepercent);
  delay(2000);

  // Turned off on receiving readings
//  digitalWrite(relayInput, LOW); // turn relay off


  
   // Servo functions
  // open
  int motor =1;
  if  (soilmoisturepercent<35){
    servo.write(0);
    motor =1;
     Serial.println("Motor On");
  }
  // close
  else{
   servo.write(90);
   motor = 0;
     Serial.println("Motor Off");

  }


  // Send data to thingspeak  
  send_thingspeak(ldrstatus, h, t, soilmoisturepercent, motor);


  delay(2000);
}

int soil_moisture_sensor() {
  float sensorValue = analogRead(SensorPin);
  int soilmoisturepercent = 100 -((sensorValue/1023) *100);

//  soilMoistureValue = analogRead(SensorPin); //put Sensor insert into soil
//  //  soilmoisturepercent = 100 - ((soilMoistureValue /1023) *100);
//  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.println(soilmoisturepercent);
  return soilmoisturepercent;
}

void send_thingspeak(int ldr, float h, float temp, int soilmoisturepercent, int motor) {

  if (client.connect(server, 80)) //   "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(ldr);
    postStr +="&field2=";
    postStr += String(h);
    postStr +="&field3=";
    postStr += String(temp);
    postStr +="&field4=";
    postStr += String(soilmoisturepercent);
    postStr +="&field5=";
    postStr += String(motor);
    
    Serial.println(postStr);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
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
