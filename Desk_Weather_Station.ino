/*

  I2C Scanner - http://www.esp8266learning.com/i2c-scanner.php

  PIN Mapping - They dont have to be in this order you can change them and update the below const to new pins.
  NodeMCU - Device - PIN
  D0  - 5110  - LIGHT
  D1  - 5110  - CE
  D2  - 5110  - RST
  D3  - BME   - SCL
  D4  - BME   - SDA
  D5  - 5110  - DC
  D6  - 5110  - DIN
  D7  - 5110  - CLK
  
  Connect display and BME to available 3.3v and GND pins

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Ticker.h>

const byte I2C_SDA = D4;
const byte I2C_SCL = D3;

const byte RST_PIN = D2;
const byte CE_PIN = D1;
const byte DC_PIN = D5;
const byte BL_PIN = D0;
const byte DIN_PIN = D6;
const byte CLK_PIN = D7;

const byte displayContrast = 40;

// thingspeak requires 3 fields, temp humidity pressure
const char* thingspeakAPI = "xxx";
const char* ssid = "xxx";
const char* password = "xxx";

Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);
Adafruit_BME280 bme;

Ticker displayTicker;

void setup() {

  // light up the LED on display
  pinMode(BL_PIN,OUTPUT);
  digitalWrite(BL_PIN,LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.println("starting up....");

  Wire.begin(I2C_SDA,I2C_SCL);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  display.begin();
  display.setContrast(displayContrast);
  display.clearDisplay();
  
  bool status;
  status = bme.begin(0x76);
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  if (!status) {
      display.println("BME280 not    detected");
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  } else {
    display.println("BME280        detected");
    Serial.println("found BME280 sensor");
  }
  display.display();
  delay(1000);


  displayTicker.attach(1,updateDisplay);
}

  void updateThingspeak(){
    if (WiFi.status() == WL_CONNECTED) {
      
      // build the get request 
      String req;
      float h = bme.readHumidity();
      if (isnan(h)) {
        Serial.println("nan detected");
        delay(200);
        h = bme.readHumidity();
      }
      float t = bme.readTemperature();
      float p = bme.readPressure();
    
      // submit to thingspeak
      HTTPClient httpts;
      req = "/update?api_key=";
      req = req + thingspeakAPI;
      req = req + "&field1=" + t;
      req = req + "&field2=" + h;
      req = req + "&field3=" + p/100;
      Serial.println(req);
      
      // convert String to char array, .begin function does not accept Strings
      char urlChar[300];
      req.toCharArray(urlChar,req.length()+1);
      httpts.begin("api.thingspeak.com", 80, urlChar);
      httpts.GET();
    }
  }


void loop() {
  // using basic delay here instead of ticker, for some reason sending http requets from a ticker crashes the esp.
  delay(60000); // 60000 ms = 60 seconds, adjust accordingly
  updateThingspeak();
}

void updateDisplay(){
  Serial.println("update dispay");
  display.clearDisplay();
  
  // write temp
  display.setRotation(2);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Temp");
  display.setTextSize(2);
  String tempReading;
  tempReading = bme.readTemperature();
  tempReading = tempReading.substring(0,tempReading.indexOf(".")+2);

  // write humidity
  display.println(tempReading);
  display.setTextSize(1);
  display.setCursor(0,25);
  display.println("Humidity");
  display.setTextSize(2);
  String humidityReading;
  humidityReading = bme.readHumidity();
  humidityReading = humidityReading.substring(0,humidityReading.indexOf("."));
  display.print(humidityReading);
  display.println("%");

  // write pressure down the side
  display.setCursor(0,0);
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  String pressureReading;
  pressureReading = bme.readPressure() / 100;
  pressureReading = pressureReading.substring(0,pressureReading.indexOf("."));
  Serial.println(pressureReading);
  display.print("hPa");
  display.setCursor(0,10);
  display.setTextSize(2);
  display.print(pressureReading);

  // update bottom left of screen with X or triangle to indicate if wifi is connnected
  // triangle for wifi connected, x for no connection
  if (WiFi.status() == WL_CONNECTED) {
    display.setRotation(0);
    display.drawTriangle(3,6, 0,0, 6,0,BLACK);
  }
  else {
    display.setRotation(2);
    display.setTextSize(0);
    display.setCursor(78,40);
    display.print("x");
  }

  // update the display
  display.display();
}
