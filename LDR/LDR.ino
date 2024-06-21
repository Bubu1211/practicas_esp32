
const int PIN_LDR 34;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("LDR init");
}

void loop() {
  // put your main code here, to run repeatedly:
  int lectura = analogRead(PIN_LDR);
  Serial.println(lectura);
}
