#include <Servo.h>

int Shootbutton = 0;

int motorCW1 = 9; // pwm
int motorCW2 = 10;
int motorCCW1 = 13; // pwm
int motorCCW2 = 12;

Servo teste;

void setup() {
  teste.attach(7);
  teste.write(90);

  pinMode(motorCW1, OUTPUT);
  pinMode(motorCCW1, OUTPUT);
  pinMode(motorCW2, OUTPUT);
  pinMode(motorCCW2, OUTPUT);

  pinMode(Shootbutton, INPUT_PULLUP);

}

void loop() {

  if (digitalRead(Shootbutton) == 0) {

  digitalWrite(motorCW1, 0);
  digitalWrite(motorCW2, 1);
  digitalWrite(motorCCW1, 1);
  digitalWrite(motorCCW2, 0);

  delay(4000);

  teste.write(120);

  delay(300);

  teste.write(62);

  delay(300);

  digitalWrite(motorCW1, 1);
  digitalWrite(motorCW2, 1);
  digitalWrite(motorCCW1, 1);
  digitalWrite(motorCCW2, 1);

  teste.write(90);

  }
}
