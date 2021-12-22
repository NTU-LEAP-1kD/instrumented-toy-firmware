#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"
#include "logging/tasks.h"

void setup() {
  digitalWrite(PIN_STAT_LED, HIGH);
  initUSB();
  initSD();
  initDebugging();

  initQwiic();
  initBLE();

  initIMU();
  initTaskSyncRtc();
  initPressureSensor();
  initDataLogging();
}

void loop() {
  current_ms = millis();
  //loopTaskReadPressureSensor();
  //loopTaskLogImu();
  loopTaskPollBle();
  loopTaskReadCts();
  loopTaskSyncRtc(); 
  loopTaskLogData();
}
