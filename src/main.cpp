#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"
#include "logging/tasks.h"

void setup() {
  digitalWrite(PIN_STAT_LED, HIGH);
  initSD();
  initDebugging();

  initQwiic();
  initUSB();
  initBLE();

  initIMU();
  initTaskSyncRtc();
  initPressureSensor();
  initDataLogging();
}

void loop() {
  // put your main code here, to run repeatedly:
  current_ms = millis();
  //loopTaskReadPressureSensor();
  //loopTaskLogImu();
  loopTaskPollBle();
  loopTaskReadCts();
  loopTaskSyncRtc(); 
  loopTaskLogData();
}
