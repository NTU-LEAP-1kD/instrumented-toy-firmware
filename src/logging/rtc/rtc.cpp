#include <RTC.h>
#include "comms/timesync/cts/cts.h"
#include "utils/utils.h"
#include "rtc.h"

Apollo3RTC myRTC;

void setRtc(current_time_t time){
    myRTC.getTime();
    int dd = time.day, mm = time.month, yy = time.year + CTS_YEAR_OFFSET, h = time.hours, m = time.minutes, s = time.seconds;
    int ms_10 = ((int)time.frac_seconds * 100) / 256; //millis divided by 10;
    myRTC.setTime(ms_10, s, m, h, dd, mm, yy);
}

//Returns the number of milliseconds according to the RTC
//(In increments of 10ms)
//Watch out for the year roll-over!
uint64_t rtcMillis()
{
  myRTC.getTime();
  uint64_t millisToday = 0;
  int dayOfYear = calculateDayOfYear(myRTC.dayOfMonth, myRTC.month, myRTC.year + 2000);
  millisToday += ((uint64_t)dayOfYear * 86400000ULL);
  millisToday += ((uint64_t)myRTC.hour * 3600000ULL);
  millisToday += ((uint64_t)myRTC.minute * 60000ULL);
  millisToday += ((uint64_t)myRTC.seconds * 1000ULL);
  millisToday += ((uint64_t)myRTC.hundredths * 10ULL);

  return (millisToday);
}
