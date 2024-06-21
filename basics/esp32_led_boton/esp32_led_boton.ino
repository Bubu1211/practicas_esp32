#define pinBoton 4 //pin donde se lee el boton
#define pinLed 2 //pin donde está el led
#include "Arduino.h"

int ledState = LOW;  //para manejar el estado del LED
int lastState = 0;   //registra el estado anterior del botón
int state;           //lee el estado actual 
void setup() {
  // put your setup code here, to run once:
  pinMode(pinBoton, INPUT_PULLUP);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, ledState); //empieza en un estado predefinidio = LOW
  Serial.begin(9600);
}

void loop() {
  state = digitalRead(pinBoton);  //se lee el estado del boton
  //si el boton se presiono y el estado anterior es que no estaba presionado
  if(state == LOW && lastState==HIGH){ 
    Serial.println("Boton presionado ");
    ledState = !ledState; //se cambia el estado del led
    digitalWrite(pinLed, ledState); //se modifica el led
  }
  lastState = state; //se guarda el estado 
  delay(100); //debounce por tiempo
}
