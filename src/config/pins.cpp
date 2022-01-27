#include <Arduino.h>
#include "config.h"

void initPins(){
    pinMode(PIN_STAT_LED, OUTPUT);
    digitalWrite(PIN_STAT_LED, HIGH);
    analogReadResolution(14);
}
