#pragma once

typedef enum{
    D_FATAL,
    D_ERROR,
    D_WARN,
    D_INFO,
    D_DEBUG,
} debug_code_t;

int olaftoa(float fValue, char *pcBuf, int iPrecision, int bufSize);
