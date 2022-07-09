#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor2(2);

Servo leftServo;
Servo rightServo;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  leftServo.attach(9);
  rightServo.attach(10);
  stopMotor(motor2);
}

void loop() {
  Serial.println("Loop. Starting motors forwards...");

  //startMotorForwards(motor2);

  leftServo.write(90);
  rightServo.write(90);

  delay(2000);

  Serial.println("Loop. Stopping motors...");

  //stopMotor(motor2);

  leftServo.write(0);
  rightServo.write(0);

  delay(1000);

  Serial.println("Loop. Stopping motors...");

  //stopMotor(motor2);

  Serial.println("Loop. Starting motors backwards...");


  //startMotorBackwards(motor2);

 // stopMotor(motor2);

  //delay(1000);
}
void startMotorForwards(AF_DCMotor motor) {
  motor.run(FORWARD);
  motor.setSpeed(254);
}

void startMotorBackwards(AF_DCMotor motor) {
  motor.run(BACKWARD);
  motor.setSpeed(254);
}

void stopMotor(AF_DCMotor motor) {
  motor.run(RELEASE);
}
