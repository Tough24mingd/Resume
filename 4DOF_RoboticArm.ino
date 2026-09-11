#include <Servo.h>

Servo base, arm1, arm2, arm3;

void setup() {
  base.attach(6);
  arm1.attach(5);
  arm2.attach(11);
  arm3.attach(10);

  Serial.begin(9600);
  Serial.println("Enter: <joint letter><angle> e.g. b90, a45, c120, d30");
  Serial.println("b=base, a=arm1(shoulder), c=arm2(elbow), d=arm3(final)");
}

void loop() {
  if (Serial.available() > 0) {
    char joint = Serial.read();
    int angle = Serial.parseInt();

    if (angle >= 0 && angle <= 180) {
      switch (joint) {
        case 'b': base.write(angle); break;
        case 'a': arm1.write(angle); break;
        case 'c': arm2.write(angle); break;
        case 'd': arm3.write(angle); break;
      }
    }
  }
}