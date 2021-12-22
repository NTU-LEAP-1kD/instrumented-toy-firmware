#pragma once

void debugLogMv(uint16_t mv);
uint16_t readBatteryMv();
uint16_t filterMv(uint16_t mv);

const uint64_t BATTERY_READ_INTERVAL_MS = 5000; 
const uint8_t BATTERY_FILTER_FACTOR = 3;
const float DIV3_TO_MV = settings.vinCorrectionFactor * 1000.0 * 3.0 * 2.0 / 16384.0; //Convert 1/3 VIN to VIN (14-bit resolution) and Correct for divider impedance (determined experimentally)
