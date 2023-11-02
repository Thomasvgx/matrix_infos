# include "weather_api.h"

void call_openweather(float* current_temp)
{
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  // Ignore SSL certificate validation
  client->setInsecure();
  
  //create an HTTPClient instance
  HTTPClient https;

  const char *weather_api_key = WEATHER_API_KEY;
  const char *api_url = "https://api.openweathermap.org/data/2.5/weather?lat=43.5959145&lon=1.4525005&appid=";

  //Initializing an HTTPS communication using the secure client
  Serial.print("[HTTPS] BEGIN CONNECTION TO OPEN WEATHER MAP API ... \n");
  if (https.begin(*client, strcat((char *)api_url, (char *)weather_api_key))) {  // HTTPS
    Serial.print("[HTTPS] GET CONNECTION TO OPEN WEATHER MAP API ...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);

        JSONVar my_obj = JSON.parse(payload);

        if (JSON.typeof(my_obj) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }

        Serial.print("JSON object = ");
        Serial.println(my_obj["main"]["feels_like"]);

        String current_temp_str;
        // float current_temp;

        current_temp_str = JSON.stringify(my_obj["main"]["feels_like"]);
        *current_temp = current_temp_str.toFloat() - 273.0;
        Serial.println(*current_temp);

        // Display.setTextAlignment(PA_CENTER);
        // nb_bikes_available.concat(" / ");
        // nb_bikes_available.concat(total_nb_bikes);
        // Display.print(nb_bikes_available);

      }
    } else {
      Serial.printf("[HTTPS] GET CONNECTION TO OPEN WEATHER MAP API failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect to OPEN WEATHER MAP API\n");
  }
}