#include <Arduino.h>
#include "ICM_20948.h"
#include "config/config.h"
#include "imu.h"

ICM_20948_SPI myICM;
icm_20948_DMP_data_t dmpData; // Global storage for the DMP data - extracted from the FIFO

void initIMU()
{
  SPI.begin();
  pinMode(PIN_IMU_POWER, OUTPUT);
  pin_config(PinName(PIN_IMU_POWER), g_AM_HAL_GPIO_OUTPUT); // Make sure the pin does actually get re-configured
  pinMode(PIN_IMU_CHIP_SELECT, OUTPUT);
  pin_config(PinName(PIN_IMU_CHIP_SELECT), g_AM_HAL_GPIO_OUTPUT); // Make sure the pin does actually get re-configured
  digitalWrite(PIN_IMU_CHIP_SELECT, HIGH); //Be sure IMU is deselected

  enableCIPOpullUp(); // Enable CIPO pull-up on the OLA

  //There is a quirk in v2.1 of the Apollo3 mbed core which means that the first SPI transaction will
  //disable the pull-up on CIPO. We need to do a fake transaction and then re-enable the pull-up
  //to work around this...
  #if defined(ARDUINO_ARCH_MBED)
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // Do a fake transaction
  SPI.endTransaction();
  enableCIPOpullUp(); // Re-enable the CIPO pull-up
  #endif

  //Reset ICM by power cycling it
  imuPowerOff();
  delay(100);
  imuPowerOn();
  delay(100); //Allow ICM to come online. Typical is 11ms. Max is 100ms. https://cdn.sparkfun.com/assets/7/f/e/c/d/DS-000189-ICM-20948-v1.3.pdf
  
  //if (settings.printDebugMessages) myICM.enableDebugging();
  
  uint8_t count = 1; 
  while(count){
    myICM.begin(PIN_IMU_CHIP_SELECT, SPI, 4000000);
    Serial.println(myICM.statusString(myICM.status));
    if(myICM.status == ICM_20948_Stat_Ok){
      Serial.println("Success!");
      break;
    }
    if(count++ > 100){
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