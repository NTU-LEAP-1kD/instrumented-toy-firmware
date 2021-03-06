#include <Arduino.h>
#include <RTC.h>
#include "config/config.h"
#include "utils/time/cts/cts.h"
#include "utils/time/rtc/rtc.h"
#include "utils/time/time.h"
#include "utils/utils.h"
#include "utils/filters/EMA.cpp"
#include "rtcSync.h"

int64_t timesync_rtc_offset;

void initTaskSyncRtc(){
    if(rtcMillis() < millis()){
        setRtcToCompilerTime();
    }
    char time_string_buf[37];
    getTimeString(time_string_buf);
    printDebugMessage("initTaskSyncRtc success", D_DEBUG);
}

void loopTaskSyncRtc(){ 
    static uint64_t prev_received_cts_timestamp_millis;
    static uint64_t sync_count; 
    if(prev_received_cts_timestamp_millis != received_cts_timestamp_millis){
        prev_received_cts_timestamp_millis = received_cts_timestamp_millis;
        if(sync_count++){
            setSyncedMillis();
        }
        else{
            setRtc(received_cts.time);
        }
    }
}

void setSyncedMillis(){
    int64_t new_offset;
    new_offset = calcAdjustedCtsMillis() - rtcMillis();
    updateTimesyncRtcOffset(new_offset);
}

void updateTimesyncRtcOffset(int64_t new_offset){
    if(timesync_rtc_offset == 0){
        timesync_rtc_offset = new_offset; 
    } 
    else{ //EMA filter
        static EMA<timesync_update_filter_factor,int64_t, uint64_t> timeSyncFilter(new_offset);
        timesync_rtc_offset = timeSyncFilter(new_offset); 
    }
}

uint64_t getSyncedMillis(){
    return timesync_rtc_offset + rtcMillis();
}

uint64_t calcAdjustedCtsMillis(){
    uint64_t cts_ms;
    cts_ms = ctsMillis(received_cts.time);
    cts_ms += (current.ms - received_cts_timestamp_millis);
    return cts_ms; 
}

uint64_t ctsMillis(current_time_t ct){
    uint64_t ms = 0;
    int dayOfYear = calculateDayOfYear(ct.day, ct.month, ct.year + CTS_YEAR_OFFSET);
    ms += ((uint64_t)dayOfYear * 86400000ULL);
    ms += ((uint64_t)ct.hours * 3600000ULL);
    ms += ((uint64_t)ct.minutes * 60000ULL);
    ms += ((uint64_t)ct.seconds * 1000ULL);
    ms += ((uint64_t)(ct.frac_seconds * 1000)/256);

    return (ms);
}