
/* 
 * File:   BLE.ino
 * Author: Nehal
 *
 * Created on December 2, 2024, 3:40 PM
 */

#include <BLE.H>


void setup() {
    
    Serial.begin(115200);   
    while (!Serial);
    delay(1000); 
    Serial.print("\033[2J");  // Clear the screen
    Serial.print("\033[H");   // Move the cursor to the top-left corner
   /* Initialize BLE */
    BLE_voidInit();
    canInit();
    
}


void loop() {
    /*  Restarts BLE scan periodically */ 
   BLE_voidRestartScan();
   /* Checks available memory and restarts the ESP32 if low */
   ESP32_voidCheckMemory();
  
}


