#pragma once

#define csvPrint(X) Serial.print(X);\
                    Serial.write('\t')

static uint64_t divu64_10(uint64_t ui64Val);
static int uint64_to_str(uint64_t ui64Val, char *pcBuf);
static int olaftoa(float fValue, char *pcBuf, int iPrecision, int bufSize);
