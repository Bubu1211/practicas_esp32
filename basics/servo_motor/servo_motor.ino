#include <Servo.h>
#define pinServo 23

Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(pinServo);
}

void loop() {
  // put your main code here, to run repeatedly:
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(180);
  delay(1000);
  for(int i = 0; i<=180; i++){
    servo.write(i);
    delay(200);
  }
}
