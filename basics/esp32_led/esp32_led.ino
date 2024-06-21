#define pinLed 2
#define TIME 2000

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pinLed, HIGH);
  delay(TIME);
  digitalWrite(pinLed, LOW);
  delay(TIME);
}
