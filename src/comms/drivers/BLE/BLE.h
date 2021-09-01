#pragma once

const char BLE_PERIPHERAL_NAME[] = "Victor's OLA";

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);
void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);
