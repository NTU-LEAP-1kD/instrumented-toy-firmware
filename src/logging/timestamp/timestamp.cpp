#include <RTC.h>
#include <SdFat.h>
#include "timestamp.h"

extern Apollo3RTC myRTC; 

void updateDataFileCreate(FsFile *dataFile){
  myRTC.getTime();
  dataFile->timestamp(T_CREATE, (myRTC.year + 2000), myRTC.month, myRTC.dayOfMonth, myRTC.hour, myRTC.minute, myRTC.seconds);
}

void updateDataFileAccess(FsFile *dataFile){
  myRTC.getTime(); 
  dataFile->timestamp(T_ACCESS, (myRTC.year + 2000), myRTC.month, myRTC.dayOfMonth, myRTC.hour, myRTC.minute, myRTC.seconds);
  dataFile->timestamp(T_WRITE, (myRTC.year + 2000), myRTC.month, myRTC.dayOfMonth, myRTC.hour, myRTC.minute, myRTC.seconds);
}
