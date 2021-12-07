#include "config/config.h"
#include "comms/drivers/qwiic/qwiic.h"
#include "pressure.h"

void initPressureSensor(){
    if(mpr.begin(0x18, qwiic)){
        online.barometer = true; 
    }
    else{
        Serial.println(F("Cannot connect to MicroPressure sensor."));
    }
}

void loopTaskReadPressureSensor(){
    float pressure = mpr.readPressure(ATM);
    Serial.print((pressure - 1.0)*1000.0,6);
    Serial.println(" atm");
}
