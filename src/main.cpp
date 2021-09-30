#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"

void setup() {
  // put your setup code here, to run once:
  initUSB();
  initBLE();
  initIMU();
  pinMode(PIN_STAT_LED, OUTPUT);
  Serial.println("RTT\tTime_Diff");
}

void loop() {
  // put your main code here, to run repeatedly:
  loopTaskLogImu();
  loopTaskPollBle();
  loopTaskReadCts();
}
