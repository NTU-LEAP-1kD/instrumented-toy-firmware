#include "Arduino.h"
#include "config/config.h"
#include "power/tasks.h"
#include "utils/filters/EMA.cpp"
#include "battery.h"

void loopTaskReadBattery(){
    static uint64_t prev_millis = 0;
    if(current.ms - prev_millis > BATTERY_READ_INTERVAL_MS){
        prev_millis = current.ms; 
        debugLogBattery(smartFilterMv(readBatteryMv()));
    }
}

void debugLogBattery(uint16_t mv){
    char buf[30];
    buf[0] = '\0';

    sprintf(buf,"Batt %d\%/%dmV",calculateBatteryCapacity(mv),mv);
    printDebugMessage(buf, D_DEBUG);
}

// If the mv reading is outside of LIPO_DISCHARGE_CURVE, don't pass it into the filter
uint16_t smartFilterMv(uint16_t mv){
    bool isValidLipoReading = 
        (mv < LIPO_DISCHARGE_CURVE[0]) &&
        (mv > LIPO_DISCHARGE_CURVE[LIPO_DISCHARGE_CURVE_RESOLUTION]);

    if(isValidLipoReading) return filterMv(mv);
    else return mv; 
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

uint8_t calculateBatteryCapacity(uint16_t voltage){
    for(uint8_t i = 1; i <= LIPO_DISCHARGE_CURVE_RESOLUTION; i++){
        if (voltage > LIPO_DISCHARGE_CURVE[i]){
            //Take a linear interpolation of points [i-1] and [i]
            const uint8_t scale = BATTERY_CAPACITY_MAX_VALUE/
                                  LIPO_DISCHARGE_CURVE_RESOLUTION;
            uint8_t offset = (scale * (voltage - LIPO_DISCHARGE_CURVE[i])) / 
                            (LIPO_DISCHARGE_CURVE[i-1] - LIPO_DISCHARGE_CURVE[i]);
            uint8_t capacity = BATTERY_CAPACITY_MAX_VALUE - (i * scale) + offset;
            return min(BATTERY_CAPACITY_MAX_VALUE, capacity);
        } 
    }
    return 0;
}
