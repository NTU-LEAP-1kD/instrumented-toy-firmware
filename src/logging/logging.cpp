#include "Arduino.h"
#include "config/config.h"
#include "logging/sdcard/sdcard.h"
#include "logging/sdfile/sdfile.h"

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
    sensorDataFile.sync();
  }
  else
    online.dataLogging = false;
}

//Headers for the CSV file
void printHelperText(bool terminalOnly)
{
  char helperText[1000];
  helperText[0] = '\0';

  if (settings.logRTC)
  {
    if (settings.logDate)
      strcat(helperText, "rtcDate,");
    if (settings.logTime)
      strcat(helperText, "rtcTime,");
    if (settings.logMicroseconds)
      strcat(helperText, "micros,");
  }
  if (settings.logVIN)
    strcat(helperText, "VIN,");

  if (online.IMU)
  {
    if (settings.imuUseDMP == false)
    {
      if (settings.logIMUAccel)
        strcat(helperText, "aX,aY,aZ,");
      if (settings.logIMUGyro)
        strcat(helperText, "gX,gY,gZ,");
      if (settings.logIMUMag)
        strcat(helperText, "mX,mY,mZ,");
      if (settings.logIMUTemp)
        strcat(helperText, "imu_degC,");
    }
    else
    {
      if (settings.imuLogDMPQuat6)
        strcat(helperText, "Q6_1,Q6_2,Q6_3,");
      if (settings.imuLogDMPQuat9)
        strcat(helperText, "Q9_1,Q9_2,Q9_3,HeadAcc,");
      if (settings.imuLogDMPAccel)
        strcat(helperText, "RawAX,RawAY,RawAZ,");
      if (settings.imuLogDMPGyro)
        strcat(helperText, "RawGX,RawGY,RawGZ,");
      if (settings.imuLogDMPCpass)
        strcat(helperText, "RawMX,RawMY,RawMZ,");
    }
  }
  strcat(helperText, "\r\n");

  Serial.print(helperText);
  if ((terminalOnly == false) && (settings.logData == true) && (online.microSD) && (settings.enableSD && online.microSD))
    sensorDataFile.print(helperText);
}
