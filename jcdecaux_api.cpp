#include "jcdecaux_api.h"

void call_jcdecaux(String* nb_bikes_available, String* total_nb_bikes)
{
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();
    
    // Create an HTTPClient instance
    HTTPClient https;
    const char* jcdecaux_api_key = JCDECAUX_API_KEY;
    const char* api_url = "https://api.jcdecaux.com/vls/v3/stations/102?contract=toulouse&apiKey=";
    // Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] BEGIN CONNECTION TO JC DECAUX API ...\n");
    if (https.begin(*client, strcat((char *)api_url, (char *)jcdecaux_api_key))) {  // HTTPS
      Serial.print("[HTTPS] GET CONNECTION TO JC DECAUX API ...\n");
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
          Serial.println(my_obj["totalStands"]["availabilities"]["bikes"]);

          // String nb_bikes_available;
          // String total_nb_bikes;

          *nb_bikes_available = JSON.stringify(my_obj["totalStands"]["availabilities"]["bikes"]);
          *total_nb_bikes = JSON.stringify(my_obj["totalStands"]["capacity"]);
          Serial.println(*nb_bikes_available);

          // Display.setTextAlignment(PA_CENTER);
          // nb_bikes_available.concat(" / ");
          // nb_bikes_available.concat(*total_nb_bikes);
          // Display.print(*nb_bikes_available);

        }
      } else {
        Serial.printf("[HTTPS] GET CONNECTION TO JC DECAUX API failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect to JC DECAUX API\n");
    }
}