#include <RTC.h>
#include "config/config.h"
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

//Query the RTC and put the appropriately formatted (according to settings) 
//string into the passed buffer. timeStringBuffer should be at least 37 chars long
//Code modified by @DennisMelamed in PR #70
void getTimeString(char timeStringBuffer[])
{
  //reset the buffer
  timeStringBuffer[0] = '\0';

  myRTC.getTime();

  if (settings.logDate)
  {
    char rtcDate[12]; // 10/12/2019,
    char rtcDay[3];
    char rtcMonth[3];
    char rtcYear[5];
    if (myRTC.dayOfMonth < 10)
      sprintf(rtcDay, "0%d", myRTC.dayOfMonth);
    else
      sprintf(rtcDay, "%d", myRTC.dayOfMonth);
    if (myRTC.month < 10)
      sprintf(rtcMonth, "0%d", myRTC.month);
    else
      sprintf(rtcMonth, "%d", myRTC.month);
    if (myRTC.year < 10)
      sprintf(rtcYear, "200%d", myRTC.year);
    else
      sprintf(rtcYear, "20%d", myRTC.year);
    if (settings.americanDateStyle == true)
      sprintf(rtcDate, "%s/%s/%s,", rtcMonth, rtcDay, rtcYear);
    else
      sprintf(rtcDate, "%s/%s/%s,", rtcDay, rtcMonth, rtcYear);
    strcat(timeStringBuffer, rtcDate);
  }

  if (settings.logTime)
  {
    char rtcTime[13]; //09:14:37.41,
    int adjustedHour = myRTC.hour;
    if (settings.hour24Style == false)
    {
      if (adjustedHour > 12) adjustedHour -= 12;
    }
    char rtcHour[3];
    char rtcMin[3];
    char rtcSec[3];
    char rtcHundredths[3];
    if (adjustedHour < 10)
      sprintf(rtcHour, "0%d", adjustedHour);
    else
      sprintf(rtcHour, "%d", adjustedHour);
    if (myRTC.minute < 10)
      sprintf(rtcMin, "0%d", myRTC.minute);
    else
      sprintf(rtcMin, "%d", myRTC.minute);
    if (myRTC.seconds < 10)
      sprintf(rtcSec, "0%d", myRTC.seconds);
    else
      sprintf(rtcSec, "%d", myRTC.seconds);
    if (myRTC.hundredths < 10)
      sprintf(rtcHundredths, "0%d", myRTC.hundredths);
    else
      sprintf(rtcHundredths, "%d", myRTC.hundredths);
    sprintf(rtcTime, "%s:%s:%s.%s,", rtcHour, rtcMin, rtcSec, rtcHundredths);
    strcat(timeStringBuffer, rtcTime);
  }
  
  if (settings.logMicroseconds)
  {
    // Convert uint64_t to string
    // Based on printLLNumber by robtillaart
    // https://forum.arduino.cc/index.php?topic=143584.msg1519824#msg1519824
    char microsecondsRev[20]; // Char array to hold to microseconds (reversed order)
    char microseconds[20]; // Char array to hold to microseconds (correct order)
    uint64_t microsNow = micros();
    unsigned int i = 0;
    
    if (microsNow == 0ULL) // if usBetweenReadings is zero, set tempTime to "0"
    {
      microseconds[0] = '0';
      microseconds[1] = ',';
      microseconds[2] = 0;
    }
    
    else
    {
      while (microsNow > 0)
      {
        microsecondsRev[i++] = (microsNow % 10) + '0'; // divide by 10, convert the remainder to char
        microsNow /= 10; // divide by 10
      }
      unsigned int j = 0;
      while (i > 0)
      {
        microseconds[j++] = microsecondsRev[--i]; // reverse the order
        microseconds[j] = ',';
        microseconds[j+1] = 0; // mark the end with a NULL
      }
    }
    
    strcat(timeStringBuffer, microseconds);
  }
}
