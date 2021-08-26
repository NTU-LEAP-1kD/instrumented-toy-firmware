#include <Arduino.h>
#include "USB.h"

void initUSB(){
    Serial.begin(USB_BAUDRATE);
}
