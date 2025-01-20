
/* 
 * File:   BLE.ino
 * Author: Nehal
 *
 * Created on December 2, 2024, 3:40 PM
 */
#include <BLE.H>

void setup() {
    
    Serial.begin(115200);
    delay(1000); 
   /* Initialize BLE */
    BLE_voidInit();
    
}


void loop() {
    /*  Restarts BLE scan periodically */ 
   BLE_voidRestartScan();
   /* Checks available memory and restarts the ESP32 if low */
   ESP32_voidCheckMemory();
 
 
}
