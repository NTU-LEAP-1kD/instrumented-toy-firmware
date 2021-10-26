#pragma once 

void initBLE(); 
void initUSB();
void initQwiic();

void initTaskSyncRtc();
void loopTaskSyncRtc();
void loopTaskPollBle();
void loopTaskReadCts();
