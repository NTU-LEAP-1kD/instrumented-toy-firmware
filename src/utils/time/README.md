# Different clocks used

| Name | Device | Clock Source | Resolution | Description | 
|--|--|--|--|--|
|millis()|Onboard clock|Crystal Oscillator|1ms| High drift (~0.8%)
|RTC|Real-time Clock|100Hz clock|10ms| Powered by onboard battery
|CTS|Bluetooth|GPS/NTC Time|1/256 s| Accuracy dependent on RTT

# Synchronisation Method

BLE synchronisation (cts)
- RTT of CTS request is recorded using millis()
- If RTT is > MAX_CTS_RTT, CTS data is discarded
- Otherwise, `received_cts` is updated, along with `received_cts_timestamp_millis` (set to millis + RTT/2)

RTC synchronistion (rtcSync)
- RTC is set at start of program
- The offset between the RTC time and the actual time is stored in `timesync_rtc_offset`
- If new CTS data is received (by comparing `received_cts_timestamp_millis`), this offset is updated
- Synced millis can be obtained in `getSyncedMillis()`
  
