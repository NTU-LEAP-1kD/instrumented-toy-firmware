#include <ArduinoBLE.h>

#include "config/config.h"
#include "comms/drivers/BLE/BLE.h"
#include "logging/rtc/rtc.h"
#include "utils/utils.h"
#include "cts.h"

cts_time_t received_cts_time; 
uint32_t rtt; 

void loopTaskReadCts(){ 
    static uint32_t prev_read_ms; 
    if(current_ms - prev_read_ms > CTS_READ_INTERVAL){
        BLEDevice central = BLE.central();
        if(readCurrentTimeService(central.service(CTS_SERVICE))){
            prev_read_ms = current_ms + rtt;
        }
        current_ms += rtt;
    }
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

void subscribeToCurrentTimeService(BLEService cts_svc){
    if(!cts_svc || !cts_svc.hasCharacteristic(CTS_CHAR_ID)) return;
    BLECharacteristic cts_char = cts_svc.characteristic(CTS_CHAR_ID);
    if(!cts_char.canSubscribe() || !cts_char.subscribe()) return;
}

bool readCurrentTimeService(BLEService cts_svc){
    if(!cts_svc || !cts_svc.hasCharacteristic(CTS_CHAR_ID)) return 0;
    static BLECharacteristic cts_char = cts_svc.characteristic(CTS_CHAR_ID);
    uint32_t ms1,ms2;

    ms1 = millis(); 
    cts_char.read();
    ms2 = millis();
    if(cts_char.valueSize() != sizeof(cts_time_t)) return 0;
    memcpy(received_cts_time.buf,cts_char.value(),sizeof(cts_time_t));
    rtt = (ms2 - ms1);

    digitalWrite(PIN_STAT_LED, !digitalRead(PIN_STAT_LED));
    if(rtt >= MAX_CTS_RTT) return 0;
    return 1;
}


