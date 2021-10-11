#include <Arduino.h>
#include <RTC.h>
#include "config/config.h"
#include "comms/timesync/cts/cts.h"
#include "utils/time/rtc/rtc.h"
#include "utils/time/time.h"
#include "utils/utils.h"
#include "rtcSync.h"

int64_t timesync_rtc_offset;

void initTaskSyncRtc(){
    if(rtcMillis() < millis()){
        setRtcToCompilerTime();
    }
}

void loopTaskSyncRtc(){ 
    static uint64_t prev_received_cts_timestamp_millis;
    static uint64_t sync_count; 
    if(prev_received_cts_timestamp_millis != received_cts_timestamp_millis){
        prev_received_cts_timestamp_millis = received_cts_timestamp_millis;
        if(sync_count++ > 1){
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
    csvPrint(new_offset);
    updateTimesyncRtcOffset(new_offset);
    Serial.println(timesync_rtc_offset);
}

void updateTimesyncRtcOffset(int64_t new_offset){
    static int64_t remainder;
    if(timesync_rtc_offset == 0){
        timesync_rtc_offset = new_offset; 
    } 
    else{ //EMA filter
        timesync_rtc_offset *= timesync_update_filter_factor;
        timesync_rtc_offset += new_offset;
        timesync_rtc_offset += remainder;
        remainder = timesync_rtc_offset % (timesync_update_filter_factor + 1);
        timesync_rtc_offset /= timesync_update_filter_factor + 1; 
    }
}

uint64_t getSyncedMillis(){
    return timesync_rtc_offset + rtcMillis();
}

uint64_t calcAdjustedCtsMillis(){
    uint64_t cts_ms;
    cts_ms = ctsMillis(received_cts.time);
    cts_ms += (current_ms - received_cts_timestamp_millis);
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