#pragma once 
#include "comms/timesync/cts/cts.h"

void setRtc(current_time_t time);
void setRtcToCompilerTime();
uint64_t rtcMillis();
