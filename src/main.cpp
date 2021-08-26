#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"

void setup() {
  // put your setup code here, to run once:
  initUSB();
  initBLE();
  initIMU();
}

void loop() {
  // put your main code here, to run repeatedly:
  static int64_t i;
  Serial.println(i++);
}