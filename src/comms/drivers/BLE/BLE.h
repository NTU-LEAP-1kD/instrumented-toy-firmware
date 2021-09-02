#pragma once

#define BLE_PERIPHERAL_NAME "Victor's OLA"
#define CTS_SERVICE "1805"
#define CTS_CHAR_ID "2a2b"

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);
void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);
void readCurrentTimeService(BLEService currentTimeService);

void exploreService(BLEService service);
void exploreCharacteristic(BLECharacteristic characteristic);
void exploreDescriptor(BLEDescriptor descriptor);
void printData(const unsigned char data[], int length);
