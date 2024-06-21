
//Librería para acceder al WiFi
#include <WiFi.h>    //libreria para conectarse a internet 
#include <PubSubClient.h>  //libreria para conectarse al protocolo MQTT

//constantes del programa
#define led 2  //led indicador de conexión a internet
#define led_mqtt 21   //led indicador de conexión a MQTT
#define rele 23       //pin del rele al que se va  a conectar

//Constantes de la conexión a internet
const char* ssid = "TP-LINK_2.4GHz_57E50C"; // Nombre de red
const char* password = "07228964"; // Contraseña de la red

//Conexión del broker MQTT
const char* mqtt_server = "broker.hivemq.com"; //servidor del protocolo MQTT
const char* topic_mqtt = "cesarin/foco"; //topico que se usará en este programa

//cadenas para verificar el mensaje en el topico
const char* ON = "on";
const char* OFF = "off";

//objetos para realizar las conexiones como cliente
WiFiClient espClient; //cliente de internet
PubSubClient client(espClient); //cliente en el protocolo MQTT

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //estado inicial 
  pinMode(led, OUTPUT);
  pinMode(led_mqtt, OUTPUT);
  pinMode(rele, OUTPUT);
  //parpadeo del led indicador
  digitalWrite(led, HIGH);
  digitalWrite(led_mqtt, HIGH);
  delay(1500);
  digitalWrite(led, LOW);
  digitalWrite(led_mqtt, LOW);
  delay(1500);
  //conectarse a internet 
  Serial.print("Conectandose a la red: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //Inicia conexión a internet
  //Mientras no se logre la conexión a internet se mantiene en stand by
  while(WiFi.status() != WL_CONNECTED){ 
    Serial.print(".");
    delay(10);
  }
  //una vez que se logra la conexión
  digitalWrite(led, HIGH);
  Serial.println();
  Serial.print("Conectado IP: ");
  Serial.println(WiFi.localIP()); //muestra la IP asignada a la esp32
  delay(1000);

  //conexión al broker MQTT
    //se realiza la conexión a un servidor mediante la url y el puerto 1883
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback); //se activa la función que maneja las respuestas del servidor
  delay(1500);
  digitalWrite(led_mqtt, HIGH); //inficador de éxito de conexión
}

void loop() {
  //Comprobar la conexión con el broker MQTT
  if(!client.connected()){
    //método definido para reintentar la conexión
    reconnect();
  }
  client.loop(); //loop de este cliente MQTT (callback);
}

//función que reintenta la conexión con MQTT
void reconnect(){
  digitalWrite(led_mqtt, LOW); //apagar el indicador de conexión a MQTT
  while(!client.connected()){ //mientras no se este conectado a MQTT
    Serial.print("Intentando conectar con MQTT");
    if(client.connect("IoTClient")){   //intenta la conexión 
      Serial.println("Conexión exitosa");
      digitalWrite(led_mqtt, HIGH);
      client.subscribe(topic_mqtt);    //Se suscribe al topico 
    }else{
      //en caso de tener un error se informa
      Serial.print("Falló la conexión, error rc = ");
      Serial.print(client.state());
      Serial.println("Intentar de nuevo en 2.5 segundos");
      delay(2500);
      Serial.println(client.connected());
    }
  }
}

//función que reacciona a las respuestas dekl servidor
void callback(char* topic, byte* message, unsigned int length){ 
  Serial.print("Llego un mensaje al topico: ");
  Serial.print(topic);
  Serial.print(". El mensaje es: ");
  //imprimir mensaje recibido
  String messageTemp;
  for(int i = 0; i<length; i++){
    Serial.print((char)message[i]);  //muestra caracter por caracter el mensaje
    messageTemp += (char) message[i]; //se recupera el mensaje en un solo String
  }
  Serial.println();

  //manejamos el mensaje conforme a nuestras necesidades
  if(String(topic) == topic_mqtt){ //verifica que el topico sea el indicado
    Serial.println("Coincidencia del topico");
    if(messageTemp == ON){
      Serial.println("Encendiendo ON");
      digitalWrite(rele, HIGH);
      delay(10);
    }
    else if(messageTemp == OFF){
      Serial.println("Apagando OFF");
      digitalWrite(rele, LOW);
      delay(10);
    }
  }
}
