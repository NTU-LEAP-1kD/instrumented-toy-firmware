#include <ArduinoBLE.h>

#include "config/config.h"
#include "comms/drivers/BLE/BLE.h"
#include "utils/time/rtcSync/rtcSync.h"
#include "utils/time/rtc/rtc.h"
#include "utils/time/time.h"
#include "cts.h"

cts_time_t received_cts; 
uint64_t received_cts_timestamp_millis; 
uint32_t rtt; 

void loopTaskReadCts(){ 
    static uint32_t prev_read_ms; 
    if(current_ms - prev_read_ms > CTS_READ_INTERVAL){
        BLEDevice central = BLE.central();
        if(readCurrentTimeService(central.service(CTS_SERVICE))){
            prev_read_ms = current_ms + rtt;
            received_cts_timestamp_millis = current_ms + (rtt/2);
        }
        current_ms = millis();
    }
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
    rtt = (ms2 - ms1);

    if(rtt >= MAX_CTS_RTT) return 0;
    if(cts_char.valueSize() != sizeof(cts_time_t)) return 0;

    memcpy(received_cts.buf,cts_char.value(),sizeof(cts_time_t));
    return 1;
}


