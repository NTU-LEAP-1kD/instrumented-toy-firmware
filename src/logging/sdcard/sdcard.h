#pragma once

//microSD Interface
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <SPI.h>
#include <SdFat.h> //SdFat v2.0.7 by Bill Greiman: http://librarymanager/All#SdFat_exFAT

#define SD_FAT_TYPE 3 // SD_FAT_TYPE = 0 for SdFat/File, 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_CONFIG SdSpiConfig(PIN_MICROSD_CHIP_SELECT, SHARED_SPI, SD_SCK_MHZ(24)) // 24MHz

const int sdPowerDownDelay = 100; //Delay for this many ms before turning off the SD card power

void initSD();
void microSDPowerOn();
void microSDPowerOff();