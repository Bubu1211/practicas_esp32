//Librería para acceder al WiFi
#include <WiFi.h>
#define led 2

//Constantes de la conexión
const char* ssid = "TP-LINK_2.4GHz_57E50C"; // Nombre de red
const char* password = "07228964"; // Contraseña de la red

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  delay(1500);
  digitalWrite(led, LOW);
  delay(1000);
  WiFi.begin(ssid, password); //Inicia conexión a internet
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(10);
  }
  digitalWrite(led, HIGH);
  Serial.println();
  Serial.print("Conectado IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Conectado a internet");
  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(500);
  }else{
    digitalWrite(led, LOW);
    Serial.println("Desconectado");
  }
}
