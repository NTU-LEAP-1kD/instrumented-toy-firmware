#include "Arduino.h"
#include "config/config.h"
#include "sensors/IMU/imu.h"
#include "sensors/pressure/pressure.h"
#include "logging/sdcard/sdcard.h"
#include "logging/sdfile/sdfile.h"
#include "utils/time/rtc/rtc.h"
#include "utils/utils.h"

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
  if (online.barometer && settings.logBarometer){
    strcat(helperText, "atm,");
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

void logIMU(char* buf){
  char tempData[50];
  char tempData1[16];
  char tempData2[16];
  char tempData3[16];
  icm_20948_DMP_data_t dmpData;
  
  if (online.IMU)
  {
    //printDebug("getData: online.IMU = " + (String)online.IMU + "\r\n");

    if (settings.imuUseDMP == false)
    {
      if (myICM.dataReady())
      {
        //printDebug("getData: myICM.dataReady = " + (String)myICM.dataReady() + "\r\n");

        myICM.getAGMT(); //Update values

        if (settings.logIMUAccel)
        {
          olaftoa(myICM.accX(), tempData1, 2, sizeof(tempData1) / sizeof(char));
          olaftoa(myICM.accY(), tempData2, 2, sizeof(tempData2) / sizeof(char));
          olaftoa(myICM.accZ(), tempData3, 2, sizeof(tempData3) / sizeof(char));
          sprintf(tempData, "%s,%s,%s,", tempData1, tempData2, tempData3);
          strcat(buf, tempData);
        }
        if (settings.logIMUGyro)
        {
          olaftoa(myICM.gyrX(), tempData1, 2, sizeof(tempData1) / sizeof(char));
          olaftoa(myICM.gyrY(), tempData2, 2, sizeof(tempData2) / sizeof(char));
          olaftoa(myICM.gyrZ(), tempData3, 2, sizeof(tempData3) / sizeof(char));
          sprintf(tempData, "%s,%s,%s,", tempData1, tempData2, tempData3);
          strcat(buf, tempData);
        }
        if (settings.logIMUMag)
        {
          olaftoa(myICM.magX(), tempData1, 2, sizeof(tempData1) / sizeof(char));
          olaftoa(myICM.magY(), tempData2, 2, sizeof(tempData2) / sizeof(char));
          olaftoa(myICM.magZ(), tempData3, 2, sizeof(tempData3) / sizeof(char));
          sprintf(tempData, "%s,%s,%s,", tempData1, tempData2, tempData3);
          strcat(buf, tempData);
        }
        if (settings.logIMUTemp)
        {
          olaftoa(myICM.temp(), tempData1, 2, sizeof(tempData1) / sizeof(char));
          sprintf(tempData, "%s,", tempData1);
          strcat(buf, tempData);
        }
      }
      //else
      //{
      //  printDebug("getData: myICM.dataReady = " + (String)myICM.dataReady() + "\r\n");
      //}
    }
    else
    {
      myICM.readDMPdataFromFIFO(&dmpData);
      while (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)
      {
        myICM.readDMPdataFromFIFO(&dmpData); // Empty the FIFO - make sure data contains the most recent data
      }
      if (settings.imuLogDMPQuat6)
      {
        olaftoa(((double)dmpData.Quat6.Data.Q1) / 1073741824.0, tempData1, 5, sizeof(tempData1) / sizeof(char));
        olaftoa(((double)dmpData.Quat6.Data.Q2) / 1073741824.0, tempData2, 5, sizeof(tempData2) / sizeof(char));
        olaftoa(((double)dmpData.Quat6.Data.Q3) / 1073741824.0, tempData3, 5, sizeof(tempData3) / sizeof(char));
        sprintf(tempData, "%s,%s,%s,", tempData1, tempData2, tempData3);
        strcat(buf, tempData);
      }
      if (settings.imuLogDMPQuat9)
      {
        olaftoa(((double)dmpData.Quat9.Data.Q1) / 1073741824.0, tempData1, 5, sizeof(tempData1) / sizeof(char));
        olaftoa(((double)dmpData.Quat9.Data.Q2) / 1073741824.0, tempData2, 5, sizeof(tempData2) / sizeof(char));
        olaftoa(((double)dmpData.Quat9.Data.Q3) / 1073741824.0, tempData3, 5, sizeof(tempData3) / sizeof(char));
        sprintf(tempData, "%s,%s,%s,%d,", tempData1, tempData2, tempData3, dmpData.Quat9.Data.Accuracy);
        strcat(buf, tempData);
      }
      if (settings.imuLogDMPAccel)
      {
        sprintf(tempData, "%d,%d,%d,", dmpData.Raw_Accel.Data.X, dmpData.Raw_Accel.Data.Y, dmpData.Raw_Accel.Data.Z);
        strcat(buf, tempData);
      }
      if (settings.imuLogDMPGyro)
      {
        sprintf(tempData, "%d,%d,%d,", dmpData.Raw_Gyro.Data.X, dmpData.Raw_Gyro.Data.Y, dmpData.Raw_Gyro.Data.Z);
        strcat(buf, tempData);
      }
      if (settings.imuLogDMPCpass)
      {
        sprintf(tempData, "%d,%d,%d,", dmpData.Compass.Data.X, dmpData.Compass.Data.Y, dmpData.Compass.Data.Z);
        strcat(buf, tempData);
      }
    }
  }
}

void logBarometer(char* buf){
  if(online.barometer && settings.logBarometer){
    char atm_buf[15];
    float atm = mpr.readPressure(ATM) - 1.0f;
    olaftoa(atm, atm_buf, 10, sizeof(atm_buf)/sizeof(char)); 
    strcat(buf,atm_buf);
  }
}

void printBufToFile(char* buf, FsFile &file){
    strcat(buf, "\r\n");
    if(settings.enableTerminalOutput == true){
        Serial.print(buf);
    }
    if (settings.enableSD && online.microSD)
    {
        uint32_t recordLength = file.write(buf, strlen(buf));
        if (recordLength != strlen(buf)) //Record the buffer to the card
        {
          if (settings.printDebugMessages == true)
          {
            Serial.println("*** sensorDataFile.write data length mismatch! ***");
          }
        }
        file.sync();
    }
}