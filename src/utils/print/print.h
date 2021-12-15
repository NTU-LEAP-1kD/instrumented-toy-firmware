#pragma once

typedef enum{
    D_FATAL,
    D_ERROR,
    D_WARN,
    D_INFO,
    D_DEBUG,
} debug_code_t;

void printDebugMessage(const char* debug_msg, debug_code_t debug_code);
int olaftoa(float fValue, char *pcBuf, int iPrecision, int bufSize);
