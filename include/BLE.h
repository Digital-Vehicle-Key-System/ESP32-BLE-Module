#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include "BLE_CAN.h"


// Define your service and characteristic UUIDs.
#define SERVICE_UUID         "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID  "abcdef01-1234-5678-1234-56789abcdef0"

void ESP32_voidSendUserID(int* copy_userID);
void ESP32_voidSendRSSIArray(signed char copy_RSSI_Array[8]);

void initBLEServer() ;
#endif // BLE_H