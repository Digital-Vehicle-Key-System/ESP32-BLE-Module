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
#include "BLE_CAN.h"
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
/**CAN Communication**/
void ESP32_voidSendRSSI(sint8 copy_RSSI);
void ESP32_voidSendRSSIArray(sint8 copy_RSSI_Array[8]);/* for 8 RSSI values*/
void ESP32_voidSendUserID( uint8* copy_userID);

#endif	/* BLE_H */
