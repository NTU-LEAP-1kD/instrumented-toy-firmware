#include "Arduino.h"
#include "config/config.h"
#include "logging/sdcard/sdcard.h"
#include "logging/sdfile/sdfile.h"
#include "logging/helpers/helpers.h"
#include "debug.h"

void initDebugging(){
  char debugLogHeader[32]; 
  if (online.microSD == true && settings.logDebug == true)
  {
    //If we don't have a file yet, create one. Otherwise, re-open the last used file
    if (strlen(debugDataFileName) == 0)
      sprintf(debugLogHeader,"dbg_%llx_",settings.olaChipId);
      strcpy(debugDataFileName, findNextAvailableLog(settings.nextDataLogNumber, debugLogHeader));

    if (debugDataFile.open(debugDataFileName, O_CREAT | O_APPEND | O_WRITE) == false)
    {
      Serial.println(F("Failed to create debug data file"));
      return;
    }
    printDebugMessage("File Creation", D_DEBUG);
    //updateDataFileCreate(&debugDataFile); // Update the file create time & date
  }
}

void printDebugMessage(const char* debug_msg, debug_code_t debug_code){
  char buf[512];
  buf[0] = '\0';
  logTime(buf);
  logDebugCode(buf, debug_code);
  strcat(buf, debug_msg);
  if(settings.printDebugMessages) Serial.println(buf);
  printBufToFile(buf, debugDataFile);
}

void logDebugCode(char* buf, debug_code_t debug_code){
  char temp[5];
  sprintf(temp, "%d,",(uint8_t)debug_code);
  strcat(buf, temp);
}
