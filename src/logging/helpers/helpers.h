#pragma once 

void printHelperText(bool terminalOnly);
void logTime(char* buf);
void logIMU(char* buf);
void printBufToFile(char* buf, FsFile &file);
