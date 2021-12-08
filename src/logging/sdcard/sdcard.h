#pragma once

//microSD Interface
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <SPI.h>
#include <SdFat.h> //SdFat v2.0.7 by Bill Greiman: http://librarymanager/All#SdFat_exFAT

#define SD_CONFIG SdSpiConfig(PIN_MICROSD_CHIP_SELECT, SHARED_SPI, SD_SCK_MHZ(24)) // 24MHz

const int sdPowerDownDelay = 100; //Delay for this many ms before turning off the SD card power

extern char sensorDataFileName[30]; //We keep a record of this file name so that we can re-open it upon wakeup from sleep
extern char debugDataFileName[30]; //We keep a record of this file name so that we can re-open it upon wakeup from sleep

extern SdFs sd;
extern FsFile sensorDataFile; //File that all sensor data is written to
extern FsFile debugDataFile; //File that all incoming serial data is written to

void initSD();
void microSDPowerOn();
void microSDPowerOff();