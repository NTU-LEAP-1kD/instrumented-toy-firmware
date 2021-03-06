#pragma once

#define CTS_SERVICE "1805"
#define CTS_CHAR_ID "2a2b"

const uint16_t CONNECTION_INTERVAL_MIN = 6;
const uint16_t CONNECTION_INTERVAL_MAX = 6;

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);
void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);

void exploreService(BLEService service);
void exploreCharacteristic(BLECharacteristic characteristic);
void exploreDescriptor(BLEDescriptor descriptor);
void printData(const unsigned char data[], int length);
