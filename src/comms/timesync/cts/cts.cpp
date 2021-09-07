#include <ArduinoBLE.h>

#include "config/config.h"
#include "comms/drivers/BLE/BLE.h"
#include "cts.h"

void loopTaskReadCts(){ 
    if(millis()%200 == 0){
        BLEDevice central = BLE.central();
        readCurrentTimeService(central.service(CTS_SERVICE));
    }
}

uint32_t ctsMillis(current_time_t ct){
    return((uint32_t)ct.frac_seconds * 1000)/256;
}

void subscribeToCurrentTimeService(BLEService cts_svc){
    if(!cts_svc || !cts_svc.hasCharacteristic(CTS_CHAR_ID)) return;
    BLECharacteristic cts_char = cts_svc.characteristic(CTS_CHAR_ID);
    if(!cts_char.canSubscribe() || !cts_char.subscribe()) return;
}

void readCurrentTimeService(BLEService cts_svc){
    if(!cts_svc || !cts_svc.hasCharacteristic(CTS_CHAR_ID)) return;
    static BLECharacteristic cts_char = cts_svc.characteristic(CTS_CHAR_ID);
    static int32_t min_rtt = 1000;
    static int32_t max_rtt, sum_rtt, count;
    cts_time_t rcvd_cts;
    uint32_t ms1,ms2,rtt;

    ms1 = millis(); 
    cts_char.read();
    ms2 = millis();
    if(cts_char.valueSize() != sizeof(cts_time_t)) return;
    memcpy(rcvd_cts.buf,cts_char.value(),sizeof(cts_time_t));
    rtt = (ms2 - ms1);

    min_rtt = min(rtt, min_rtt); 
    max_rtt = max(rtt, max_rtt);
    sum_rtt += rtt;
    ++count;

    Serial.print(rtt);
    Serial.write('\t');
    Serial.print(min_rtt);
    Serial.write('\t');
    Serial.print(max_rtt);
    Serial.write('\t');
    Serial.print((float)sum_rtt/count);
    Serial.write('\t');
    Serial.print(ctsMillis(rcvd_cts.time));
    Serial.write('\n');

    digitalWrite(PIN_STAT_LED, !digitalRead(PIN_STAT_LED));
}

