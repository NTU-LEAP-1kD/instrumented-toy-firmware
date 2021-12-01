#include "Arduino.h"
#include "config/config.h"
#include "sdcard.h"

#if SD_FAT_TYPE == 1
SdFat32 sd;
File32 sensorDataFile; //File that all sensor data is written to
File32 serialDataFile; //File that all incoming serial data is written to
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile sensorDataFile; //File that all sensor data is written to
ExFile serialDataFile; //File that all incoming serial data is written to
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile sensorDataFile; //File that all sensor data is written to
FsFile serialDataFile; //File that all incoming serial data is written to
#else // SD_FAT_TYPE == 0
SdFat sd;
File sensorDataFile; //File that all sensor data is written to
File serialDataFile; //File that all incoming serial data is written to
#endif  // SD_FAT_TYPE

//#define PRINT_LAST_WRITE_TIME // Uncomment this line to enable the 'measure the time between writes' diagnostic

char sensorDataFileName[30] = ""; //We keep a record of this file name so that we can re-open it upon wakeup from sleep
char serialDataFileName[30] = ""; //We keep a record of this file name so that we can re-open it upon wakeup from sleep

void initSD()
{
  SPI.begin();
  pinMode(PIN_MICROSD_POWER, OUTPUT);
  pin_config(PinName(PIN_MICROSD_POWER), g_AM_HAL_GPIO_OUTPUT); // Make sure the pin does actually get re-configured
  pinMode(PIN_MICROSD_CHIP_SELECT, OUTPUT);
  pin_config(PinName(PIN_MICROSD_CHIP_SELECT), g_AM_HAL_GPIO_OUTPUT); // Make sure the pin does actually get re-configured
  digitalWrite(PIN_MICROSD_CHIP_SELECT, HIGH); //Be sure SD is deselected

  if (settings.enableSD == true)
  {
    // For reasons I don't understand, we seem to have to wait for at least 1ms after SPI.begin before we call microSDPowerOn.
    // If you comment the next line, the Artemis resets at microSDPowerOn when beginSD is called from wakeFromSleep...
    // But only on one of my V10 red boards. The second one I have doesn't seem to need the delay!?
    delay(5);

    microSDPowerOn();

    //Max power up time is 250ms: https://www.kingston.com/datasheets/SDCIT-specsheet-64gb_en.pdf
    //Max current is 200mA average across 1s, peak 300mA
    for (int i = 0; i < 10; i++) //Wait
    {
      //checkBattery();
      delay(1);
    }

    if (sd.begin(SD_CONFIG) == false) // Try to begin the SD card using the correct chip select
    {
      Serial.println(F("SD init failed (first attempt). Trying again...\r\n"));
      for (int i = 0; i < 250; i++) //Give SD more time to power up, then try again
      {
        //checkBattery();
        delay(1);
      }
      if (sd.begin(SD_CONFIG) == false) // Try to begin the SD card using the correct chip select
      {
        Serial.println(F("SD init failed (second attempt). Is card present? Formatted?"));
        Serial.println(F("Please ensure the SD card is formatted correctly using https://www.sdcard.org/downloads/formatter/"));
        digitalWrite(PIN_MICROSD_CHIP_SELECT, HIGH); //Be sure SD is deselected
        online.microSD = false;
        return;
      }
    }

    //Change to root directory. All new file creation will be in root.
    if (sd.chdir() == false)
    {
      Serial.println(F("SD change directory failed"));
      online.microSD = false;
      return;
    }

    online.microSD = true;
  }
  else
  {
    microSDPowerOff();
    online.microSD = false;
  }
}

void microSDPowerOn()
{
  pinMode(PIN_MICROSD_POWER, OUTPUT);
  pin_config(PinName(PIN_MICROSD_POWER), g_AM_HAL_GPIO_OUTPUT); // Make sure the pin does actually get re-configured
  digitalWrite(PIN_MICROSD_POWER, LOW);
}
void microSDPowerOff()
{
  pinMode(PIN_MICROSD_POWER, OUTPUT);
  pin_config(PinName(PIN_MICROSD_POWER), g_AM_HAL_GPIO_OUTPUT); // Make sure the pin does actually get re-configured
  digitalWrite(PIN_MICROSD_POWER, HIGH);
}
