#pragma once 
#include "ICM_20948.h"

const uint8_t MAX_IMU_SETUP_ATTEMPTS = 10; 

extern ICM_20948_SPI myICM;
struct Vector3D { double x, y, z; };

Vector3D toEuler( double w, double x, double y, double z);
void imuPowerOn();
void imuPowerOff();
bool enableCIPOpullUp();
void configureImuPins();
