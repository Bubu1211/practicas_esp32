#include "WiFi.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //comunicación serial a 9600 baudios
  Serial.println("Iniciando programa");
  WiFi.mode(WIFI_STA);
  Serial.println("Modo wifi: STA");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Escaneando ");

  int n = WiFi.scanNetworks(); //escanea las redes cercanas al esp32, n es el n´mero de redes
  Serial.print("Escaneo realizado, redes detectadas: ");
  Serial.println(n);
  if(n != 0){
    for(int i = 0; i<n; i++){
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "T " : "F");
      delay(10);
    }
  }
  delay(5000);
}
