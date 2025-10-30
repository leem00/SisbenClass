#include <ESP32Servo.h>

static const int servoPin = 13;

Servo servo1;

void setup() {

  Serial.begin(115200);
  servo1.attach(servoPin);
}

void loop() {
  servo1.write(90);
  delay(500);
  servo1.write(45);
  delay(500);
}