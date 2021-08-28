#include <Arduino.h>
#include "config/config.h"

void initUSB(){
    Serial.begin(settings.serialTerminalBaudRate);
}
