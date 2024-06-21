#include <BH1750.h>
#include <Wire.h>

const int SCL 22
const int SDA 21

BH1750 sensorLuz;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(SDA, SCL);
  sensorLuz.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float lectura = sensorLuz.readLightLevel();
  Serial.print("Lectura: ");
  Serial.print(lectura);
  Serial.println("lux");
  delay(2000);
}
