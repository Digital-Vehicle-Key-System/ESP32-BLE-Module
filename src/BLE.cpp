
#include "BLE.h"

signed char rssiArray[8];   // Array to hold 8 RSSI values
uint8_t rssiIndex = 0; // Index to track how many values collected
int rssiValue =0;
int check=0;
// Global pointer to our BLE characteristic.
static NimBLECharacteristic *pCharacteristic = nullptr;

// Callback class for server connection events.
class MyServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) override {
      Serial.println("Client connected");
    }
  
    void onDisconnect(NimBLEServer* pServer) override {
      Serial.println("Client disconnected");
      // Restart advertising when the client disconnects.
      NimBLEDevice::startAdvertising();
    }
  };


// Callback class for handling characteristic write events.
class MyCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *pCharacteristic) override {
      // Get the incoming data from the client.
      std::string rxValue = pCharacteristic->getValue();
  
      if (!rxValue.empty()) {
         check++;
        // Print the received data.
        Serial.print("Received Data: ");
        Serial.println(rxValue.c_str());

        // Prepare the ACK message.
        std::string ackMessage = "ACK";

         // Store the received RSSI value in the global array if there's space.
         if ((rssiIndex < 8) && (check !=1)) {
          rssiValue = atoi(rxValue.c_str()); // Convert the received string to an integer using atoi.
          rssiArray[rssiIndex++] = rssiValue;
        }

        if(check==1 || (check==9)){

        // Set the value of the characteristic to the ACK message.
        pCharacteristic->setValue(ackMessage);
        
        // Notify the client with the ACK message.
        pCharacteristic->notify();
        Serial.print("Sent Data: ");
        Serial.println(ackMessage.c_str());
        if(check == 9){
          check=0;
        }
        // Once 8 RSSI values are stored, print the array.
        if (rssiIndex >= 8) {
            Serial.println("RSSI Array:");
            for (int i = 0; i < 8; i++) {
              Serial.print(rssiArray[i]);
              Serial.print(" ");
            }
            Serial.println();
            /**************CAN********************/
            ESP32_voidSendRSSIArray(rssiArray);
            // Optionally, reset index to store new readings.
            rssiIndex = 0;
          }
  
        }
 
      }
    }
  };  

void initBLEServer() {
    Serial.println("Starting BLE Server using NimBLE");
    NimBLEDevice::init("MyESP32-BLE");
    NimBLEDevice::setMTU(512);
    
    // Create the BLE server and assign callbacks.
    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
  
    // Create the BLE service.
    NimBLEService* pService = pServer->createService(SERVICE_UUID);
  
    // Create a BLE characteristic with READ, WRITE, and NOTIFY properties.
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
                      );
    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
    
    // Optionally, set an initial value.
    pCharacteristic->setValue("Hello");
  
    // Start the service.
    pService->start();
  
    // Configure advertising: add the service UUID and set scan response.
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->start();
  
    Serial.println("BLE advertising started");
  }

// Send User ID in chunks (each chunk is 8 bytes, except possibly the last one)
void ESP32_voidSendUserID(int* copy_userID) {
    int ID = 0x20;  // Start CAN IDs from 0x20
    // 27 bytes total to send (for example)
    for (int i = 0; i < 27; i += 8) {
        // Calculate the chunk size (8 bytes or the remainder if fewer than 8 left)
        char size = (i + 8 <= 27) ? 8 : (27 - i);
        // Cast to (char*) so that canSender sends byte-level data
        canSender(ID++, (char *)&copy_userID[i], size);  
    }
}

// Send the RSSI Array (8 signed RSSI values)
void ESP32_voidSendRSSIArray(signed char copy_RSSI_Array[8]) {
    char rssi_size = 8;  // Total 8 bytes for 8 RSSI values
    // Since our canSender expects a char pointer, we can cast our array
    canSender(0x30, (char *)copy_RSSI_Array, rssi_size);
}

