#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"

void setup() {
  // put your setup code here, to run once:
  initUSB();
  initBLE();
  initIMU();
  Serial.println("x\ty\tz");
}

void loop() {
  // put your main code here, to run repeatedly:
  loopTaskLogImu();
  loopTaskPollBle();
}
