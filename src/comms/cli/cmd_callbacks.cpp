#include <SimpleCLI.h>
#include "cmd_callbacks.h"
#include "../fileTransfer/fileTransfer.h"

/* Callback template from README
void myCallback(cmd* c) {
  Command cmd(c); // Create wrapper class instance for the pointer
  // ..
}
*/ 


void pingCallback(cmd* c) {
  Command cmd(c); // Create wrapper class instance for the pointer
  // ..
  Serial.println("Ping");
}

void xferCallback(cmd* c) {
  Command cmd(c); // Create wrapper class instance for the pointer
  // ..
  sdCardMenu();
}

void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    // Print error
    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    // Print command usage
    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}