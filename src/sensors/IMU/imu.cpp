#include <Arduino.h>
#include "ICM_20948.h"
#include "config/config.h"
#include "utils/utils.h"
#include "imu.h"

ICM_20948_SPI myICM;

void initIMU()
{
  configureImuPins();
  
  if (settings.printDebugMessages) myICM.enableDebugging();
  uint8_t count = 1; 
  while(count){
    myICM.begin(PIN_IMU_CHIP_SELECT, SPI, 4000000);
    if(myICM.status == ICM_20948_Stat_Ok){
      break;
    }
    if(count++ > MAX_IMU_SETUP_ATTEMPTS){
      online.IMU = false;
      return;
    }
  }

  //Give the IMU extra time to get its act together. This seems to fix the IMU-not-starting-up-cleanly-after-sleep problem...
  //Seems to need a full 25ms. 10ms is not enough.
  delay(25); //Allow ICM to come online.

  bool success = true;

  //Check if we are using the DMP
  if (settings.imuUseDMP == false)
  {
    //Perform a full startup (not minimal) for non-DMP mode
    ICM_20948_Status_e retval = myICM.startupDefault(false);
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not startup the IMU in non-DMP mode!"));
      success = false;
    }
    //Update the full scale and DLPF settings
    retval = myICM.enableDLPF(ICM_20948_Internal_Acc, settings.imuAccDLPF);
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not configure the IMU Accelerometer DLPF!"));
      success = false;
    }
    retval = myICM.enableDLPF(ICM_20948_Internal_Gyr, settings.imuGyroDLPF);
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not configure the IMU Gyro DLPF!"));
      success = false;
    }
    ICM_20948_dlpcfg_t dlpcfg;
    dlpcfg.a = settings.imuAccDLPFBW;
    dlpcfg.g = settings.imuGyroDLPFBW;
    retval = myICM.setDLPFcfg((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), dlpcfg);
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not configure the IMU DLPF BW!"));
      success = false;
    }
    ICM_20948_fss_t FSS;
    FSS.a = settings.imuAccFSS;
    FSS.g = settings.imuGyroFSS;
    retval = myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), FSS);
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not configure the IMU Full Scale!"));
      success = false;
    }
  }
  else
  {
    // Initialize the DMP
    ICM_20948_Status_e retval = myICM.initializeDMP();
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not startup the IMU in DMP mode!"));
      success = false;
    }
    if (settings.imuLogDMPQuat6)
    {
      retval = myICM.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR);
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not enable the Game Rotation Vector (Quat6)!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Quat6, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Quat6 ODR!"));
        success = false;
      }
    }
    if (settings.imuLogDMPQuat9)
    {
      retval = myICM.enableDMPSensor(INV_ICM20948_SENSOR_ROTATION_VECTOR);
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not enable the Rotation Vector (Quat9)!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Quat9, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Quat9 ODR!"));
        success = false;
      }
    }
    if (settings.imuLogDMPAccel)
    {
      retval = myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER);
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not enable the DMP Accelerometer!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Accel, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Accel ODR!"));
        success = false;
      }
    }
    if (settings.imuLogDMPGyro)
    {
      retval = myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_GYROSCOPE);
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not enable the DMP Gyroscope!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Gyro, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Gyro ODR!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Gyro_Calibr, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Gyro Calibr ODR!"));
        success = false;
      }
    }
    if (settings.imuLogDMPCpass)
    {
      retval = myICM.enableDMPSensor(INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED);
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not enable the DMP Compass!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Cpass, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Compass ODR!"));
        success = false;
      }
      retval = myICM.setDMPODRrate(DMP_ODR_Reg_Cpass_Calibr, 0); // Set ODR to 55Hz
      if (retval != ICM_20948_Stat_Ok)
      {
        Serial.println(F("Error: Could not set the Compass Calibr ODR!"));
        success = false;
      }
    }
    retval = myICM.enableFIFO(); // Enable the FIFO
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not enable the FIFO!"));
      success = false;
    }
    retval = myICM.enableDMP(); // Enable the DMP
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not enable the DMP!"));
      success = false;
    }
    retval = myICM.resetDMP(); // Reset the DMP
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not reset the DMP!"));
      success = false;
    }
    retval = myICM.resetFIFO(); // Reset the FIFO
    if (retval != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Error: Could not reset the FIFO!"));
      success = false;
    }
  }

  if (success)
  {
    online.IMU = true;
    delay(50); // Give the IMU time to get its first measurement ready
  }
  else
  {
    //Power down IMU
    imuPowerOff();
    online.IMU = false;
  }
}

void loopTaskLogImu(){
  icm_20948_DMP_data_t dmpData; 
  struct Vector3D vector_3d;

  myICM.readDMPdataFromFIFO(&dmpData);
  
  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
  {
    if ((dmpData.header & DMP_header_bitmap_Quat9) > 0) // We have asked for orientation data so we should receive Quat9
    {
    double q1 = ((double)dmpData.Quat9.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
    double q2 = ((double)dmpData.Quat9.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
    double q3 = ((double)dmpData.Quat9.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
    double q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));
    
    vector_3d = toEuler(q0,q1,q2,q3);

    if(settings.outputSerial){
      csvPrint(vector_3d.x);
      csvPrint(vector_3d.y);
      csvPrint(vector_3d.z);
      //csvPrint(dmpData.Quat9.Data.Accuracy);
      Serial.write('\n');
    }
    }
  }
}
