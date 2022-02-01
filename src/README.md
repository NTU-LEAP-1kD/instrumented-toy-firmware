# src

## Code Structure
|Folder|Function|
|--|--|
|comms|Handle driver code for communications (BLE,USB,Qwiic) and also the command line interface (CLI)|
|config|Global definitions and variables that are included in every file|
|logging|Datalogging utils and SD card drivers|
|power|Battery monitoring and other functions to conserve power|
|sensors|IMU and Pressure sensor code|
|utils|Any algorithms/helper functions that are used in several sections of the code|

## Design Patterns

### Tasks
- Every function that needs to be run is organised into Tasks
- Why Tasks
  - Allows for consistent structuring of code
  - Enables us to refactor into a state machine or RTOS later if we require it
- How Tasks run
  - Tasks are called in `main.cpp`.
  - Tasks are either run once at the start (`initTaskXXXX`) or in the loop (`loopTaskXXXX`)
- How to add a Task
  - Determine which relevant folder it should be in
  - Decide if it is a initTask or a loopTask and give it a descriptive name e.g. `initTaskCheckBatteryVoltage`
  - Add it into these three places:
    - Define it in the `.cpp` file
    - Declare it in the `tasks.h` file (this should be in the src/XXX folder)
    - Call it in `main.cpp` 

### Abstraction
- Specifically, we want to try to abstract away the hardware from the software
- Hence we separate driver code and application code, even if the driver code is really simple
