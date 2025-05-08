
/* 
 * File:   BLE.ino
 * Author: Nehal
 *
 * Created on December 2, 2024, 3:40 PM
 */

 #include <BLE.H>
#include <CAN.h>

 void setup() {
     
     Serial.begin(115200);   
     while (!Serial);
     delay(1000); 

    /* Initialize CAN*/
     canInit();
	 /* Initialize BLE */
     BLE_voidInit();
    
     
 }
 
 
 void loop() {
     /*  Restarts BLE scan periodically */ 
    BLE_voidRestartScan();
    /* Checks available memory and restarts the ESP32 if low */
    ESP32_voidCheckMemory();
    

    // If no new advertisement for 2 seconds, reset rssiIndex.
    if ((millis() - lastRSSIUpdate) > 2000 && rssiIndex != 0) {
      rssiIndex = 0;
      Serial.println("No update in 2 seconds, resetting RSSI index.");
    }
   
 }

 