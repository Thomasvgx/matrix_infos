#ifndef WEATHER_API_H
#define WEATHER_API_H
#include <Arduino.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "secrets.h"

void call_openweather(float* current_temp);

#endif