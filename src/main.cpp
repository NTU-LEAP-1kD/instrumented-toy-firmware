#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"
#include "logging/tasks.h"

void setup() {
  // put your setup code here, to run once:
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
  // put your main code here, to run repeatedly:
  current_ms = millis();
  //loopTaskReadPressureSensor();
  //loopTaskLogImu();
  loopTaskPollBle();
  loopTaskReadCts();
  loopTaskSyncRtc(); 
  loopTaskLogData();
}
