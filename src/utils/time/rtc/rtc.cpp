#include <RTC.h>
#include "utils/time/cts/cts.h"
#include "utils/time/time.h"
#include "rtc.h"

Apollo3RTC myRTC;

/*
Since the RTC runs on 100Hz clock, it returns time in increments of 10ms
ms_offset holds the 1ms decimal place
*/
static uint8_t ms_offset; 

void setRtc(current_time_t time){
  myRTC.getTime();
  int dd = time.day, mm = time.month, yy = time.year + CTS_YEAR_OFFSET, h = time.hours, m = time.minutes, s = time.seconds;
  int ms = ((int)time.frac_seconds * 1000) / 256 ;
  int ms_10 = ms / 10; 
  ms_offset = ms % 10; 
  myRTC.setTime(ms_10, s, m, h, dd, mm, yy);
}

void setRtcToCompilerTime(){
  myRTC.setToCompilerTime();
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
  millisToday += ((uint64_t)ms_offset); 

  return (millisToday);
}
