#include <Arduino.h>
#include "config/config.h"

void initChipId(){
    am_hal_mcuctrl_device_t sDevice;
    am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &sDevice);
    settings.olaChipId = ((uint64_t)sDevice.ui32ChipID1 << 32) + (uint64_t)sDevice.ui32ChipID0; 
}
