#include "Arduino.h"
#include "config/config.h"
#include "logging/sdcard/sdcard.h"
#include "logging/sdfile/sdfile.h"
#include "logging/helpers/helpers.h"

void initDataLogging();
void printHelperText(bool terminalOnly);

void initDataLogging()
{
  if (online.microSD == true && settings.logData == true)
  {
    //If we don't have a file yet, create one. Otherwise, re-open the last used file
    if (strlen(sensorDataFileName) == 0)
      strcpy(sensorDataFileName, findNextAvailableLog(settings.nextDataLogNumber, "dataLog"));

    if (sensorDataFile.open(sensorDataFileName, O_CREAT | O_APPEND | O_WRITE) == false)
    {
      Serial.println(F("Failed to create sensor data file"));
      online.dataLogging = false;
      return;
    }
    //updateDataFileCreate(&sensorDataFile); // Update the file create time & date
    online.dataLogging = true;
    printHelperText(false);
  }
  else
    online.dataLogging = false;
}

void initDebugging(){
  if (online.microSD == true && settings.logDebug == true)
  {
    //If we don't have a file yet, create one. Otherwise, re-open the last used file
    if (strlen(debugDataFileName) == 0)
      strcpy(debugDataFileName, findNextAvailableLog(settings.nextDataLogNumber, "debugLog"));

    if (debugDataFile.open(debugDataFileName, O_CREAT | O_APPEND | O_WRITE) == false)
    {
      Serial.println(F("Failed to create debug data file"));
      return;
    }
    //updateDataFileCreate(&debugDataFile); // Update the file create time & date
  }
}

void loopTaskLogData(){
    static uint64_t prev_log_data_time;
    if(millis() - prev_log_data_time > settings.usBetweenReadings / 1000ULL){
        prev_log_data_time = millis(); 

        char log_buf[512];
        log_buf[0] = '\0';
        logTime(log_buf);
        logIMU(log_buf);
        logBarometer(log_buf);
        printBufToFile(log_buf, sensorDataFile);
    }
}