#ifndef JCDECAUX_API_H
#define JCDECAUX_API_H
#include <Arduino.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "secrets.h"

void call_jcdecaux(String* nb_bikes_available, String* total_nb_bikes);

#endif