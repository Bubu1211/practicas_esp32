#include <SPI.h> //librería comunicación SPI
#include <MFRC522.h> //librería para el sensor Rc522
//Librería para acceder al WiFi
#include <WiFi.h>    //libreria para conectarse a internet 
#include <PubSubClient.h>  //libreria para conectarse al protocolo MQTT

#define SS_PIN 5 //pin SDA transferencia de datos
#define RST_PIN 22 //pin de reset para sincronización

//Constantes de la conexión a internet
const char* ssid = "UAEH_CC"; // Nombre de red
const char* password = "07228964"; // Contraseña de la red

//Conexión del broker MQTT
const char* mqtt_server = "broker.hivemq.com"; //servidor del protocolo MQTT
const char* topic_mqtt = "cesarin/522/inputs"; //topico que se usará en este programa

MFRC522 rfid(SS_PIN, RST_PIN); // instanciamos la clase con los pines
MFRC522::MIFARE_Key key; //generamos una llave de lectura

//objetos para realizar las conexiones como cliente
WiFiClient espClient; //cliente de internet
PubSubClient client(espClient); //cliente en el protocolo MQTT

// guarfamos los nuid en arrays de 4 bytes
byte nuidPICC[4];

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  //se inicia la llave: FF FF FF FF
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  //conectarse a internet
  Serial.print("Conectandose a la red: ");
  Serial.println(ssid);
  WiFi.begin(ssid,"" ); //Inicia conexión a internet
  //Mientras no se logre la conexión a internet se mantiene en stand by
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(10);
  }
  //una vez que se logra la conexión
  Serial.println();
  Serial.print("Conectado IP: ");
  Serial.println(WiFi.localIP()); //muestra la IP asignada a la esp32
  delay(1000);

  //conexión al broker MQTT
  //se realiza la conexión a un servidor mediante la url y el puerto 1883
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback); //se activa la función que maneja las respuestas del servidor
  delay(1500);
}

void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println("\nEmpezando envio de datos");
    if (!client.connected()) {
      //método definido para reintentar la conexión
      while (!client.connected()) { //mientras no se este conectado a MQTT
        Serial.print("Intentando conectar con MQTT");
        if (client.connect("IoTClient")) { //intenta la conexión
          Serial.println("Conexión exitosa");
          //client.subscribe(topic_mqtt);    //Se suscribe al topico
          Serial.println("Enviando clave");
          String m1 = makeHex(rfid.uid.uidByte, rfid.uid.size);
          int longitud = m1.length();
          char m[longitud + 1];
          m1.toCharArray(m, longitud+1);
          Serial.print("m: ");
          client.publish(topic_mqtt, m);
        } else {
          //en caso de tener un error se informa
          Serial.print("Falló la conexión, error rc = ");
          Serial.print(client.state());
          Serial.println("Intentar de nuevo en 2.5 segundos");
          delay(2500);
          Serial.println(client.connected());
        }
      }
    } else {
      Serial.println("Enviando clave");
      String m1 = makeHex(rfid.uid.uidByte, rfid.uid.size);
      int longitud = m1.length();
      char m[longitud + 1];
      m1.toCharArray(m, longitud+1);
      Serial.print("m: ");
      client.publish(topic_mqtt, m);
    }
  }
  else Serial.println(F("Card read previously."));

  rfid.PICC_HaltA();

  rfid.PCD_StopCrypto1();
}


void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

String makeHex(byte *buffer, byte bufferSize)
{
  String cad = "", cad1 = "";
  for (byte i = 0; i < bufferSize; i++) {
    cad1 = (buffer[i] < 0x10) ? String("0" + String(buffer[i], HEX)) : String(buffer[i], HEX);
    cad = String(cad + cad1);
  }
  Serial.print("Cdena en makeHex: ");
  Serial.println(cad);
  return cad;
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}
