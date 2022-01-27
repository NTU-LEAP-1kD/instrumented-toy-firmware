#include <ArduinoBLE.h> 
#include "config/config.h"
#include "BLE.h"

//Custom serial logging service
BLEService serialLogService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//Battery Service follows GATT docs, see https://developer.apple.com/forums/thread/77866
BLEService batteryService("180F");
BLEByteCharacteristic battPercentageCharacteristic("2A19", BLERead);

void initBLE(){
  if (!BLE.begin()) {
    printDebugMessage("BLE Init Failed", D_FATAL);
  }
 // set the local name peripheral advertises
  BLE.setLocalName(BLE_PERIPHERAL_NAME);
  
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(serialLogService);
  BLE.setAdvertisedService(batteryService);

  // add the characteristics to services
  serialLogService.addCharacteristic(switchCharacteristic);
  batteryService.addCharacteristic(battPercentageCharacteristic);

  // add services
  BLE.addService(serialLogService);
  BLE.addService(batteryService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);

  // set an initial value for the characteristics
  switchCharacteristic.setValue(0);
  battPercentageCharacteristic.setValue(current.battery.percentage);

  // start advertising
  BLE.advertise();

  printDebugMessage("initBLE Success", D_DEBUG);
}

void loopTaskPollBle(){
  BLE.poll();
  battPercentageCharacteristic.setValue(current.battery.percentage);
}

void blePeripheralConnectHandler(BLEDevice central) {
  printDebugMessage("BLE Connected", D_INFO);

  BLE.setConnectionInterval(CONNECTION_INTERVAL_MIN,CONNECTION_INTERVAL_MAX);

  if(!central.discoverAttributes() || !central.discoverService(CTS_SERVICE)){
    central.disconnect();
  }
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  printDebugMessage("BLE Disconnected", D_INFO);
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  settings.outputSerial = switchCharacteristic.value();
}
