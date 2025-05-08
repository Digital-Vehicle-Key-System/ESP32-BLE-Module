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
 #include <string>
 /* Section : Data Type Declarations */
 
 extern unsigned long lastRSSIUpdate;
 extern uint8_t rssiIndex ;
 /* Section : Macro Declarations */
 
 
 /* Section : Macro Function Declarations */
 
 
 /* Section : Function Declarations */
 
 void BLE_voidInit();
 sint8 BLE_sint8GetRSSI(BLEAdvertisedDevice advertisedDevice);
 bool BLE_boolCheckForServiceUUID(BLEAdvertisedDevice advertisedDevice);
 const char* BLE_pconstuint8GetDeviceID(BLEAdvertisedDevice advertisedDevice);
 void BLE_voidRestartScan() ;
 void ESP32_voidCheckMemory();
 /**CAN Communication**/
 void ESP32_voidSendRSSIArray(signed char copy_RSSI_Array[8]);
 
 void ESP32_voidSendDeviceID(const char* copy_userID);
 #endif	/* BLE_H */
 