#pragma once

const uint64_t BATTERY_READ_INTERVAL_MS = 5000; 
const uint8_t BATTERY_FILTER_FACTOR = 6;

//Convert 1/3 VIN to VIN (14-bit resolution) and
// Correct for divider impedance (determined experimentally)
const float DIV3_TO_MV = settings.vinCorrectionFactor * 1000.0 * 3.0 * 2.0 / 16384.0; 

//From https://blog.ampow.com/lipo-voltage-chart/
const uint16_t LIPO_DISCHARGE_CURVE[] = {
    4200,4150,4110,4080,4020,
    3980,3950,3910,3870,3850,
    3840,3820,3800,3790,3770,
    3750,3730,3710,3690,3610,
    3400
};
const uint8_t LIPO_DISCHARGE_CURVE_RESOLUTION = sizeof(LIPO_DISCHARGE_CURVE) / 
                                                sizeof(LIPO_DISCHARGE_CURVE[0]) - 1;

const uint8_t BATTERY_CAPACITY_MAX_VALUE = 100;

void loopTaskReadBattery();
void updateBatteryStatus();
void debugLogBattery();
uint16_t smartFilterMv(uint16_t mv);
uint16_t filterMv(uint16_t mv);
uint16_t readBatteryMv();
uint8_t calculateBatteryCapacity(uint16_t voltage);
