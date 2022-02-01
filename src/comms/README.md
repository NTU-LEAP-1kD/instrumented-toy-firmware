# comms

Communication can get really complicated. We try to keep things simple.

## cli
This is the command line interface, added in #47 in order to allow receiving files over USB. 

To add/modify commands, change `cmd_callbacks.h cmd_callbacks.cpp`. 

## drivers

### BLE
We are using ArduinoBLE, check out their documentation for more information.

### qwiic
Qwiic is based on I2C. The only driver code needed was to manage the pullup resistors, and is taken from the original Sparkfun codebase

### USB
Very simple driver code here, Arduino abstracts everything away :) 