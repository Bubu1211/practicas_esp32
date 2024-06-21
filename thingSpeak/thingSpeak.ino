#include <HTTPClient.h>
#include <DHTesp.h>  // Including library for dht
#include <WiFi.h> 

String apiKey = "Z0HTDFPI1COED74L";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "TP-LINK_2.4GHz_57E50C";     // replace with your wifi ssid and wpa2 key
const char *pass =  "07228964";

#define DHTPIN 15          //pin where the dht11 is connected

DHTesp dht;

WiFiClient client;
HTTPClient http;
void setup()
{
  Serial.begin(115200);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);


  dht.setup(DHTPIN, DHTesp::DHT11);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(50);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop()
{

  TempAndHumidity datos = dht.getTempAndHumidity();
  float h = datos.humidity;
  float t = datos.temperature;

  //el protocolo https requiere config adicional por lo quue se debe usar http
  String url = "http://api.thingspeak.com/update";
  String queryParams = "api_key=" + apiKey+"&field1=" + String(t) + "&field2=" + String(h); //+"&field1=0";
  
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (http.begin(client, url)) { //conectarse a la url
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.println("URL: " + url);
    Serial.println("Params: "+ queryParams);
    int httpCode = http.POST(queryParams); //realizar petición
    Serial.println("httpCode: " + httpCode);
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();  // Obtener respuesta
        Serial.println(payload);  // Mostrar respuesta por serial
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else {
    Serial.printf("[HTTP} Unable to connect\n");
  }

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(20000);
}
