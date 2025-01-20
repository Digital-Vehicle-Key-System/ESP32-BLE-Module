/* 
 * File:   BLE.cpp
 * Author: Nehal
 *
 * Created on December 2, 2024, 3:40 PM
 */

/* Section : Includes*/
#include <BLE.h>


/*Create a BLE scanner*/
BLEScan* pBLEScan;



/* Callback class to handle BLE devices found during scanning
 * It processes the RSSI, service UUID, and local name of each advertised device.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        // Get RSSI value
        sint8 rssi = BLE_sint8GetRSSI(advertisedDevice);                  
        // Print the RSSI value to the serial
        Serial.print("RSSI: ");
        Serial.print(rssi);
        Serial.println(" dBm");
             
        // Check if the UUID matches
        if (BLE_boolCheckForServiceUUID(advertisedDevice)) {
            // Call the BLE_getUserID function to get the User ID
            const uint8* userID = BLE_pconstuint8GetUserID(advertisedDevice);
            // Print the User ID to the Serial monitor
            Serial.print("User ID: ");
            Serial.println((char*)userID);  // Casting to (char*) to print it as a C-string   
        }        
 
    }
};


  
/**
 * @brief Initializes the BLE device and starts scanning for nearby BLE devices.
 *
 * This function initializes the BLE device, sets up the BLE scanner with appropriate 
 * scanning parameters, and begins scanning for 10 seconds. It also sets the callback 
 * for handling advertised devices.
 */
void BLE_voidInit(){

    Serial.println("Starting BLE scan...");

    // Initialize BLE device
    BLEDevice::init("");

    // Create the BLE scanner
    pBLEScan = BLEDevice::getScan(); // create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
   

    // Set scan interval and window for optimal scanning
    pBLEScan->setInterval(100); // Interval in milliseconds
    pBLEScan->setWindow(100);   // Window in milliseconds (equal to interval for continuous scanning)
    pBLEScan->setActiveScan(true); // Active scan to get more information

    // Start the initial scan for 10 seconds not indefinite to improve stability
     pBLEScan->start(10, false); // Scan for 10 seconds
}

/**
 * @brief Checks if the advertised BLE device has the desired service UUID.
 *
 * This function checks if the advertised BLE device contains a service UUID. 
 * If the UUID matches the predefined service UUID, it prints a message and returns `true`. 
 * Otherwise, it returns `false`.
 * @param advertisedDevice The BLEAdvertisedDevice object containing the advertisement data, including the service UUID.
 * @return `true` if the service UUID matches; otherwise, `false`.
 */
bool BLE_boolCheckForServiceUUID(BLEAdvertisedDevice advertisedDevice){
    // Check if the device advertises the desired service UUID
    if (advertisedDevice.haveServiceUUID()) {
        String advertisedUUID = advertisedDevice.getServiceUUID().toString();

        // Check if the advertised UUID matches the desired service UUID
        if (advertisedUUID == SERVICE_UUID) {
            Serial.print(" | Found matching Service UUID: ");
            Serial.println(advertisedUUID.c_str()); // Print the matching UUID
            return true;

        }
        else{
          //Nothing
        }
    } 
    else {
            Serial.println("No UUID");
    }
    return false;
}
/**
 * @brief Retrieves the User ID from a BLE advertised device.
 *
 * This function checks whether the advertised device has a name. If the name exists, it is used as the User ID.
 * If no name is available, the function defaults to returning "Unknown" as the User ID. 
 * The function ensures that the User ID is null-terminated and fits within a static buffer before returning it.
 * 
 * @param advertisedDevice The BLE device whose User ID is being retrieved.
 * @return A pointer to the static array containing the User ID (either the device's name or "Unknown").
 */

const uint8* BLE_pconstuint8GetUserID(BLEAdvertisedDevice advertisedDevice) {
    // Static array to store the User ID (persistent across function calls)
    static uint8 UserID[USERID_SIZE];

    // Check if the device has a name
    if (advertisedDevice.haveName()) {
        // Get the name as a C-string and copy it to UserID
        strncpy((char*)UserID, advertisedDevice.getName().c_str(), sizeof(UserID) - 1);
        ((char*)UserID)[sizeof(UserID) - 1] = '\0';  // Ensure null termination
    } else {
        // If no name is available, return "Unknown"
        strncpy((char*)UserID, "Unknown", sizeof(UserID) - 1);
        ((char*)UserID)[sizeof(UserID) - 1] = '\0';  // Ensure null termination
    }

    return UserID;  // Return pointer to the User ID
}

/**
 * @brief Retrieves the RSSI (Received Signal Strength Indicator) of a BLE device.
 *
 * This function extracts the RSSI from a BLEAdvertisedDevice .
 * The RSSI value indicates the signal strength, which can help estimate the distance between the 
 * scanning device and the advertised BLE device.
 * 
 * @param advertisedDevice The BLE device whose RSSI is being retrieved.
 */
sint8 BLE_sint8GetRSSI(BLEAdvertisedDevice advertisedDevice){
        // Get the RSSI value
        sint8 rssi = advertisedDevice.getRSSI();

        return rssi;
}

/**
 * @brief Restarts BLE scan periodically.
 *
 * This function periodically stops and restarts the BLE scan to ensure continuous scanning.
 * It checks the elapsed time since the last scan and restarts the scan if the `SCAN_INTERVAL` has passed.
 * The scan is restarted for 1 second each time.
 */
void BLE_voidRestartScan(){
    uint32 currentMillis = millis();

    // Restart the scan periodically
    static uint32 lastScanTime = 0;
    if (currentMillis - lastScanTime >= SCAN_INTERVAL) {
        pBLEScan->stop();        // Stop the scan
        delay(10);               // Short delay before restarting
        pBLEScan->start(1, false); // Restart scanning for 1 second
        lastScanTime = currentMillis;
    }
}

/**
 * @brief Checks available memory and restarts the ESP32 if low.
 *
 * This function monitors the system's available heap memory. If the free memory drops below a specified threshold (10 KB),
 * the system will restart to prevent memory-related issues.
 * The function helps ensure the stability of the ESP32 by freeing up memory before any critical memory shortages occur.
 */
void ESP32_voidCheckMemory() {
    // Check memory usage to prevent issues with low memory
    if (ESP.getFreeHeap() < 10000) { // If free memory is below 10 KB
        Serial.println("Low memory, restarting...");
        ESP.restart(); // Restart ESP32 to clear memory  
    }
}
