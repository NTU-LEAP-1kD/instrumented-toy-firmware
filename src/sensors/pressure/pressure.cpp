#include "config/config.h"
#include "comms/drivers/qwiic/qwiic.h"
#include "pressure.h"

SparkFun_MicroPressure mpr; 

void initPressureSensor(){
    if(mpr.begin(BAROMETER_I2C_ADD, qwiic)){
        online.barometer = true; 
        printDebugMessage("initPressureSensor success", D_DEBUG);
    }
    else{
        printDebugMessage("initPressureSensor fail", D_FATAL);
    }
}

void loopTaskReadPressureSensor(){
    float pressure = mpr.readPressure(ATM);
    Serial.print((pressure - 1.0)*1000.0,6);
    Serial.println(" atm");
}
