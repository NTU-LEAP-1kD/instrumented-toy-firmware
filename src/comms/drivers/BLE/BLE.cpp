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

  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loopTaskPollBle(){
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());

  if(!central.discoverAttributes() || !central.discoverService(CTS_SERVICE)) return;
  readCurrentTimeService(central.service(CTS_SERVICE));
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

void readCurrentTimeService(BLEService currentTimeService){
  if(!currentTimeService || !currentTimeService.hasCharacteristic(CTS_CHAR_ID)) return;

  Serial.print("Current Time:");
  uint8_t time_buf[CTS_TIME_BUF_LEN];
  currentTimeService.characteristic(CTS_CHAR_ID).readValue(time_buf,CTS_TIME_BUF_LEN);
  printData(time_buf, CTS_TIME_BUF_LEN);
  Serial.write('\t');
  Serial.print(millis());
  Serial.write('\n');
}
