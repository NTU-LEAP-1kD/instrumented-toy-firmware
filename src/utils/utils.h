#pragma once

#define csvPrint(X) Serial.print(X);\
                    Serial.write('\t')

int calculateDayOfYear(int day, int month, int year);
