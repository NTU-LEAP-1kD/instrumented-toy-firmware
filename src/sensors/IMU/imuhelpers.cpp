#include <Arduino.h>
#include <SPI.h>
#include "config/config.h"
#include "imu.h"

void imuPowerOn()
{
  pinMode(PIN_IMU_POWER, OUTPUT);
  digitalWrite(PIN_IMU_POWER, HIGH);
}
void imuPowerOff()
{
  pinMode(PIN_IMU_POWER, OUTPUT);
  digitalWrite(PIN_IMU_POWER, LOW);
}

#if defined(ARDUINO_ARCH_MBED) // updated for v2.1.0 of the Apollo3 core
bool enableCIPOpullUp()
{
  //Add 1K5 pull-up on CIPO
  am_hal_gpio_pincfg_t cipoPinCfg = g_AM_BSP_GPIO_IOM0_MISO;
  cipoPinCfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_1_5K;
  pin_config(PinName(PIN_SPI_CIPO), cipoPinCfg);
  return (true);
}
#else
bool enableCIPOpullUp()
{
  //Add CIPO pull-up
  ap3_err_t retval = AP3_OK;
  am_hal_gpio_pincfg_t cipoPinCfg = AP3_GPIO_DEFAULT_PINCFG;
  cipoPinCfg.uFuncSel = AM_HAL_PIN_6_M0MISO;
  cipoPinCfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_1_5K;
  cipoPinCfg.eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA;
  cipoPinCfg.eGPOutcfg = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL;
  cipoPinCfg.uIOMnum = AP3_SPI_IOM;
  padMode(MISO, cipoPinCfg, &retval);
  return (retval == AP3_OK);
}
#endif

void configureImuPins(){
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
}
