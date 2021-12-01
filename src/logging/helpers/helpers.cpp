#include "Arduino.h"
#include "config/config.h"
#include "logging/sdcard/sdcard.h"
#include "logging/sdfile/sdfile.h"
#include "utils/time/rtc/rtc.h"

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

void logTime(char* buf){
  if (settings.logRTC)
  {
    char timeString[37];
    getTimeString(timeString); //TODO: Correct for drift
    strcat(buf, timeString);
  }
}

void printBufToFile(char* buf){
    strcat(buf, "\r\n");
    if(settings.enableTerminalOutput == true){
        Serial.print(buf);
    }
    if (settings.enableSD && online.microSD)
    {
        digitalWrite(PIN_STAT_LED, HIGH);
        uint32_t recordLength = sensorDataFile.write(buf, strlen(buf));
        if (recordLength != strlen(buf)) //Record the buffer to the card
        {
          if (settings.printDebugMessages == true)
          {
            Serial.println("*** sensorDataFile.write data length mismatch! ***");
          }
        }
        sensorDataFile.sync();
    }
}