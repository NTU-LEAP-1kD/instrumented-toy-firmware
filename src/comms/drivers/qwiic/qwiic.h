#pragma once 
#include <Wire.h>

extern TwoWire qwiic;

void initQwiic();
void qwiicPowerOn();
void qwiicPowerOff();
void setQwiicPullups(uint32_t qwiicBusPullUps);
