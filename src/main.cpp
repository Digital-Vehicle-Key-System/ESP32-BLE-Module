//==================================================================================//
//  PlatformIO Project: CAN Sender Example (Arduino on ESP32)
//==================================================================================//

#include <Arduino.h>
#include <CAN.h>  
#include "BLE_CAN.h"
#include "BLE.h"



// Example data for transmission (for instance, an RSSI array)
signed char rssi[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

void setup() {
    Serial.begin(115200);
    while (!Serial);   // Wait for Serial Monitor to open
    delay(1000);

    Serial.println("CAN Transmitter Example");

    // Set the CAN pins
    CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);

    // Start the CAN bus at 500 kbps
    if (!CAN.begin(500E3)) {
        Serial.println("Starting CAN failed!");
        while (1); // Hang here if CAN initialization fails
    } else {
        Serial.println("CAN Initialized");
    }

      // Initialize the BLE server.
      initBLEServer();


}

void loop() {
    // Transmit an example packet every cycle
   // canSender(0x08, (char *)rssi, 8);
    
    // Uncomment below if you want to also receive packets
    // canReceiver();
}



