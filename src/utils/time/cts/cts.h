#pragma once

#include <ArduinoBLE.h>

const uint16_t CTS_YEAR_OFFSET = 1744;

const uint32_t MAX_CTS_RTT = 100;
const uint32_t CTS_READ_INTERVAL = 30000; 

struct current_time_t{
  uint16_t year; //since 1794
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t day_of_week;
  uint8_t frac_seconds;
  uint8_t adjust_reason;
};

union cts_time_t{
    uint8_t buf[sizeof(current_time_t)];
    current_time_t time; 
};

extern cts_time_t received_cts; 
extern uint64_t received_cts_timestamp_millis;

uint64_t ctsMillis(current_time_t ct);
void subscribeToCurrentTimeService(BLEService cts_svc);
bool readCurrentTimeService(BLEService cts_svc);
void loopTaskReadCts();
