#include <ArduinoBLE.h>

#include "config/config.h"
#include "comms/drivers/BLE/BLE.h"
#include "utils/utils.h"
#include "cts.h"

cts_time_t received_cts_time; 

void loopTaskReadCts(){ 
    static uint64_t offset; 
    if(millis()%200 == 0){
        BLEDevice central = BLE.central();
        if (readCurrentTimeService(central.service(CTS_SERVICE))){;
            if(offset == 0){
                offset = ctsMillis(received_cts_time.time) - millis();
                Serial.print("Wrote offset");
                Serial.println(offset);
            }
            Serial.println((int64_t)ctsMillis(received_cts_time.time) - (offset + millis()));  
        }
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
    static int32_t min_rtt = 1000;
    static int32_t max_rtt, sum_rtt, count;
    uint32_t ms1,ms2,rtt;

    ms1 = millis(); 
    cts_char.read();
    ms2 = millis();
    if(cts_char.valueSize() != sizeof(cts_time_t)) return 0;
    memcpy(received_cts_time.buf,cts_char.value(),sizeof(cts_time_t));
    rtt = (ms2 - ms1);
    
    min_rtt = min(rtt, min_rtt); 
    max_rtt = max(rtt, max_rtt);
    sum_rtt += rtt;
    ++count;
    /*
    Serial.print(rtt);
    Serial.write('\t');
    Serial.print(min_rtt);
    Serial.write('\t');
    Serial.print(max_rtt);
    Serial.write('\t');
    Serial.print((float)sum_rtt/count);
    Serial.write('\t');
    Serial.print(ctsMillis(received_cts_time.time));
    Serial.write('\n');
    */

    digitalWrite(PIN_STAT_LED, !digitalRead(PIN_STAT_LED));
    return 1;
}

