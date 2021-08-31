#pragma once 

const uint8_t MAX_IMU_SETUP_ATTEMPTS = 10; 

void imuPowerOn();
void imuPowerOff();
bool enableCIPOpullUp();
void configureImuPins();
