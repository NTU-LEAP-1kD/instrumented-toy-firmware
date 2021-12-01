#pragma once 
#include "utils/time/cts/cts.h"

void setRtc(current_time_t time);
void setRtcToCompilerTime();
uint64_t rtcMillis();
void getTimeString(char timeStringBuffer[]);
