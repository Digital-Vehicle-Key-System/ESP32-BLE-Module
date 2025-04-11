/* 
 * File:   BLE_config.h
 * Author: Nehal
 *
 * Created on December 2, 2024, 10:49 PM
 */

#ifndef BLE_CONFIG_H
#define	BLE_CONFIG_H

/* Define the specific service UUID */
#define SERVICE_UUID "b3e24b49-7d2b-4f97-8b5b-0c4a5e70f7a2"
//#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"

/* Define User ID size */
#define USERID_SIZE   27
/* Define scan interval */
#define SCAN_INTERVAL  1000 /*Restart scan every 1 seconds*/

/* 1 second timeout for CAN communication*/
#define TIMEOUT_MS 1000  

#endif	/* BLE_CONFIG_H */