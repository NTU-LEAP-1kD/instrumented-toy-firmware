#include "Arduino.h"
#include "config/config.h"
#include "power/tasks.h"

void loopTaskReadBattery(){
    uint16_t reading = analogRead(PIN_VIN_MONITOR);
    Serial.println(reading);
}
