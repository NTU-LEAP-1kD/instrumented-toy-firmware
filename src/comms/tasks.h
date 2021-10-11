#pragma once 

void initBLE(); 
void initUSB();

void initTaskSyncRtc();
void loopTaskSyncRtc();
void loopTaskPollBle();
void loopTaskReadCts();
