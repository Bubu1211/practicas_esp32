#include <ArduinoJson.h>

JsonDocument json;

void setup() {
  Serial.begin(9600);
  delay(5000);
  // put your setup code here, to run once:
  json["titulo"] = "Titulo";
  json["data"] = 2.2;
  json["data1"] = 2;
  String jsonString;
  serializeJson(json, jsonString);
  Serial.println(jsonString);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
