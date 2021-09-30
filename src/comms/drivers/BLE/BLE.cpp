#include <ArduinoBLE.h> 
#include "config/config.h"
#include "BLE.h"

BLEService serialLogService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = PIN_STAT_LED; // pin to use for the LED

void initBLE(){
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
 // set the local name peripheral advertises
  BLE.setLocalName(BLE_PERIPHERAL_NAME);
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(serialLogService);

  // add the characteristic to the service
  serialLogService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(serialLogService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  // set an initial value for the characteristic
  switchCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  //Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loopTaskPollBle(){
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  /*
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  */

  BLE.setConnectionInterval(CONNECTION_INTERVAL_MIN,CONNECTION_INTERVAL_MAX);

  if(!central.discoverAttributes() || !central.discoverService(CTS_SERVICE)){
    central.disconnect();
  }
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  Serial.print(switchCharacteristic.value());
  
  settings.outputSerial = switchCharacteristic.value();
}
