#include "Arduino.h"
#include "config/config.h"
#include "power/tasks.h"
#include "utils/filters/EMA.cpp"
#include "battery.h"

void loopTaskReadBattery(){
    static uint64_t prev_millis = 0;
    if(current_ms - prev_millis > BATTERY_READ_INTERVAL_MS){
        prev_millis = current_ms; 
        debugLogMv(filterMv(readBatteryMv()));
    }
}

void debugLogMv(uint16_t mv){
    char buf[20];
    buf[0] = '\0';

    sprintf(buf,"Batt: %dmV",mv);
    printDebugMessage(buf, D_DEBUG);
}

uint16_t filterMv(uint16_t mv){
    static EMA<BATTERY_FILTER_FACTOR,uint16_t,uint32_t> batteryFilter(mv);
    return batteryFilter(mv);
}

// From OpenLog Artemis Sensors.ino
uint16_t readBatteryMv(){
    int div3 = analogRead(PIN_VIN_MONITOR); //Read VIN across a 1/3 resistor divider
    uint16_t mv = float(div3) * DIV3_TO_MV; 
    return mv;
}
