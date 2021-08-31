#pragma once 

const uint8_t MAX_IMU_SETUP_ATTEMPTS = 10; 

struct Vector3D { double x, y, z; };

Vector3D toEuler( double w, double x, double y, double z);
void imuPowerOn();
void imuPowerOff();
bool enableCIPOpullUp();
void configureImuPins();
