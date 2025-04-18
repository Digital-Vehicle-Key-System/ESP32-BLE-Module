
#include "BLE_CAN.h"

//==============================================================================//
//  CAN Sender
//==============================================================================//
void canSender(int id, char* message, int message_len) {
    Serial.print("Sending packet ... ");

    // Begin a CAN packet with 11-bit identifier 'id'
    CAN.beginPacket(id);  
    // Write the data byte by byte from the message array (up to 8 bytes)
    for (int i = 0; i < message_len; i++) {
        CAN.write(message[i]);
    }
    // Finish the packet
    CAN.endPacket();

    // Optionally, send an RTR (Remote Transmission Request) packet
    CAN.beginPacket(id, 3, true); // '3' is the requested data length, true indicates RTR
    CAN.endPacket();

    Serial.println("done");

    delay(100);
}

//==============================================================================//
//  CAN Receiver
//==============================================================================//
void canReceiver() {
    // Try to parse a received CAN packet
    int packetSize = CAN.parsePacket();

    if (packetSize) { // A packet was received
        Serial.print("Received ");

        if (CAN.packetExtended()) {
            Serial.print("extended ");
        }

        if (CAN.packetRtr()) {
            Serial.print("RTR ");
        }

        Serial.print("packet with id 0x");
        Serial.print(CAN.packetId(), HEX);

        if (CAN.packetRtr()) {
            Serial.print(" and requested length ");
            Serial.println(CAN.packetDlc());
        } else {
            Serial.print(" and length ");
            Serial.println(packetSize);

            // Print packet data (only for non-RTR packets)
            while (CAN.available()) {
                Serial.print((char) CAN.read());
            }
            Serial.println();
        }

        Serial.println();
    }
}