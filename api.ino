#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino_JSON.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "weather_api.h"
#include "jcdecaux_api.h"
#include "secrets.h"

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 15

MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Replace with your network credentials
const char* ssid = "T";
const char* password = WIFI_PASSWORD;

void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  
  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();

  Serial.println();
  Serial.println();
  Serial.println();

  //Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    String nb_bikes_available;
    String total_nb_bikes;
    float current_temp;
    call_jcdecaux(&nb_bikes_available, &total_nb_bikes);
    call_openweather(&current_temp);
    String current_temp_str = String(current_temp);
    Display.setTextAlignment(PA_CENTER);
    nb_bikes_available.concat(" | ");
    nb_bikes_available.concat(total_nb_bikes);
    current_temp_str.concat("C");
    // nb_bikes_available.concat(" | ");
    // nb_bikes_available.concat(current_temp);
    // nb_bikes_available.concat("°C");
    // Display.displayText(nb_bikes_available.c_str(), PA_RIGHT, 2, 1, PA_SCROLL_LEFT, PA_SLICE);
    uint32_t period = 3 * 60000L;// 5 minutes
    for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
      Display.print(nb_bikes_available);
      delay(5000);
      Display.print(current_temp_str);
      delay(2000);
    }
    
  }
  Serial.println();
  Serial.println("Waiting 2min before the next round...");
  // delay(120000);
}


