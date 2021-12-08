#pragma once 

extern char sensorDataFileName[30]; //We keep a record of this file name so that we can re-open it upon wakeup from sleep
extern char debugDataFileName[30]; //We keep a record of this file name so that we can re-open it upon wakeup from sleep

extern FsFile sensorDataFile; //File that all sensor data is written to
extern FsFile debugDataFile; //File that all incoming serial data is written to

char* findNextAvailableLog(int &newFileNumber, const char *fileLeader);
