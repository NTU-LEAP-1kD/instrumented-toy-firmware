#pragma once

#include <SimpleCLI.h>

typedef struct {
    const char* cmd_str;
    void (*callback)(cmd*);
}cmd_callback;

void pingCallback(cmd*);

const cmd_callback CMD_CALLBACKS[] = {
    {"ping", pingCallback}
};

const uint8_t NUM_OF_CMD_CALLBACKS = sizeof(CMD_CALLBACKS)/sizeof(CMD_CALLBACKS[0]);

void errorCallback(cmd_error* e);
