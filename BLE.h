/* 
 * File:   BLE.h
 * Author: Nehal
 *
 * Created on December 2, 2024, 3:40 PM
 */

#ifndef BLE_H
#define	BLE_H

/* Section : Includes*/
#include <STD_Types.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLE_config.h>
#include <BLEScan.h>
/* Section : Data Type Declarations */


/* Section : Macro Declarations */


/* Section : Macro Function Declarations */


/* Section : Function Declarations */

void BLE_voidInit();
sint8 BLE_sint8GetRSSI(BLEAdvertisedDevice advertisedDevice);
bool BLE_boolCheckForServiceUUID(BLEAdvertisedDevice advertisedDevice);
const uint8* BLE_pconstuint8GetUserID(BLEAdvertisedDevice advertisedDevice);
void BLE_voidRestartScan() ;
void ESP32_voidCheckMemory();
void ESP32_voidSendRSSI(sint8 copy_RSSI);
void ESP32_voidSendUserID(const uint8* copy_userID);

#endif	/* BLE_H */
