#pragma once 
#include "comms/timesync/cts/cts.h"

const uint16_t timesync_update_filter_factor = 999;

void initTaskSyncRtc();
void loopTaskSyncRtc();
void setSyncedMillis();
void updateTimesyncRtcOffset(int64_t new_offset);
uint64_t getSyncedMillis();
uint64_t calcAdjustedCtsMillis();
uint64_t ctsMillis(current_time_t ct);
