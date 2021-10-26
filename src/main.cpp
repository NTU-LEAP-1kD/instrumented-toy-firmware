#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"

void setup() {
  // put your setup code here, to run once:
  initQwiic();
  initUSB();
  initBLE();


  initIMU();
  initTaskSyncRtc();
  pinMode(PIN_STAT_LED, OUTPUT);
  Serial.println("Reading\tEMA");
}

void loop() {
  // put your main code here, to run repeatedly:
  current_ms = millis();
  loopTaskLogImu();
  loopTaskPollBle();
  loopTaskReadCts();
  loopTaskSyncRtc(); 
}
