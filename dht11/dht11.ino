#include <DHTesp.h>
#define pinDht 15

DHTesp dht;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.setup(pinDht, DHTesp::DHT11);
}

void loop() {
  // put your main code here, to run repeatedly:
  TempAndHumidity datos = dht.getTempAndHumidity();
  Serial.print("Temperatura: " + String(datos.temperature, 2) + "°C");
  Serial.print(" Humedad: " + String(datos.humidity, 1) + "%");
  Serial.println("-----");
  delay(2050);  //la velocidad es importante ya que no hay respuestas cada segundo
}
