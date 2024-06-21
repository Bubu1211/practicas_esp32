//Librería wifi para usar la capacidad del esp32 de conectarse a internet 
#include <WiFi.h>

void setup() {
  Serial.begin(9600); //comunicación serial a 9600 baudios
  Serial.println("Iniciando wifi...");
  WiFi.mode(WIFI_STA); //modo de internet 
}

void loop() {
  Serial.println("Escaneando redes cercanas...");
  //crear un array interno que almacena las redes escaneadas y retorna la cantidad de redes
  int n = WiFi.scanNetworks(); //Escanear las redes 
  if(n == 0){ //en caso de no encontrar redes
    Serial.println("No se detectaron redes..."); 
  }else{
    Serial.println();
    Serial.print(n);
    Serial.println(" redes encontradas");
    for(int i = 0; i<n; i++){
      Serial.print(i+1);
      Serial.print("= RSSI: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(", SSID: ");
      Serial.print(WiFi.SSID(i));
      Serial.print(", EncryptionType: ");
      Serial.println(WiFi.encryptionType(i));
      delay(10);
    }
    Serial.println("Fin de escaneo");
  }
  delay(2500);
}
