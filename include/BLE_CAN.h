

#ifndef BLE_CAN_H
#define BLE_CAN_H
#include <Arduino.h>
#include <CAN.h> 

#define TX_GPIO_NUM   21  // Connects to CTX
#define RX_GPIO_NUM   22  // Connects to CRX

// Function prototypes
void canReceiver();
void canSender(int id, char* message, int message_len);

#endif