#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"
#include "logging/tasks.h"
#include "power/tasks.h"

void setup() {
  initPins();
  
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
  loopTaskReadBattery();
  loopTaskPollBle();
  loopTaskReadCts();
  loopTaskSyncRtc(); 
  loopTaskLogData();
}
