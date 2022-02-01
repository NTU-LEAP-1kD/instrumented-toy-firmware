#include <SimpleCLI.h>
#include "config/config.h"
#include "cmd_callbacks.h"
#include "cli.h"

SimpleCLI cli;

void initCliCmds(){
    for(uint8_t i = 0; i < NUM_OF_CMD_CALLBACKS; i++){
        Command c = cli.addCmd(CMD_CALLBACKS[i].cmd_str, CMD_CALLBACKS[i].callback);
        if (!c){
            Serial.print("Error adding command: ");
            Serial.println(CMD_CALLBACKS[i].cmd_str);
        }
    }

    cli.setOnError(errorCallback);
}

void loopTaskParseCmds(){
    static char cli_buf[CLI_BUF_LEN];
    static uint8_t idx = 0;

    while(Serial.available()){
        char c = Serial.read();
        if(idx >= CLI_BUF_LEN-1){
            idx = 0;
        }
        else if(c == '\n'){
            cli_buf[idx] = '\0';
            Serial.print("Received cmd: ");
            Serial.println(cli_buf);
            cli.parse(cli_buf);
            idx = 0;
        }
        else{
            cli_buf[idx++] = c;
        }
    }
}
