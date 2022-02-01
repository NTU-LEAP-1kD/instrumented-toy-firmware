#pragma once 

void initBLE(); 
void initUSB();
void initQwiic();
void initCliCmds();

void initTaskSyncRtc();
void loopTaskSyncRtc();
void loopTaskPollBle();
void loopTaskReadCts();
void loopTaskParseCmds();
