#include <Arduino.h>
#include "config/config.h"
#include "comms/tasks.h"
#include "sensors/tasks.h"
#include "logging/tasks.h"
#include "power/tasks.h"

void setup() {
  initPins();
  initCliCmds();
  
  initUSB();
  initChipId();
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
  current.ms = millis();
  //loopTaskReadPressureSensor();
  //loopTaskLogImu();
  loopTaskParseCmds();
  loopTaskReadBattery();
  loopTaskPollBle();
  loopTaskReadCts();
  loopTaskSyncRtc(); 
  loopTaskLogData();
}
