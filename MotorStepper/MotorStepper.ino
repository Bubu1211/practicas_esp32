#include <Stepper.h>

#define IN1_MOTOR 12
#define IN2_MOTOR 14
#define IN1_MOTOR 26
#define IN1_MOTOR 27

const int STEPS_PER_REVOLUTION = 2048;
Stepper motorPasos(STEPS_PER_REVOLUTION, IN1, IN2, IN3, IN4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  motorPasos.setSpeed(5); //velocidad a 5 rpm
}

void loop() {

  Serial.println("clockwise");
  motorPasos.step(STEPS_PER_REVOLUTION);
  delay(2300);

  Serial.println("counterclockwise");
  motorPasos.step(-STEPS_PER_REVOLUTION);
  delay(2300);
}
