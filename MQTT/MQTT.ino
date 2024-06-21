#include <PubSubClient.h>   //Protocolo MQTT
#include <WiFi.h>           //conectarse a internet
#include <DHTesp.h>         //DHT11 que es el sensor de temperatura

#define PIN_DHT 15
#define TIEMPO_MUESTRA 10000 //milisegundos

const char* MQTT_SERVER = "broker.hivemq.com"; //
const char* TOPIC = "crlv/lcc/python_clima";
const char* USER = "CRLV";
const char* RED = "TP-LINK_2.4GHz_57E50C";//"";Redmi Note Bubu
const char* PASS = "07228964";//"07228964";bubu0105

WiFiClient cliente;
PubSubClient mqtt(cliente);
DHTesp dht;
TempAndHumidity lectura_dht;

String lectura;
bool conectado;
int longitud;
char *m;

void setup() {
  // put your setup code here, to run once:
  dht.setup(PIN_DHT, DHTesp::DHT11); 
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  WiFi.begin(RED, PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(10);
  }
  //conectado = WiFi.status() == WL_CONNECTED;
  conectado = true;
  mqtt.setServer(MQTT_SERVER, 1883);
  delay(2000);
}

void loop() {
  Serial.print("Red: ");
  Serial.println(WiFi.localIP());
  delay(TIEMPO_MUESTRA);
}
