

/* Section : Includes*/

#include "BLE_CAN.h"

void canInit(void){
  Serial.println ("CAN Receiver/Receiver");

  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

  // start the CAN bus at 500 kbps
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }

}

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
   /* CAN.beginPacket(id, 3, true); // '3' is the requested data length, true indicates RTR
    CAN.endPacket();*/

    Serial.println("done");

    delay(100);
}

 void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print ("Received ");

    if (CAN.packetExtended()) {
      Serial.print ("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print ("RTR ");
    }

    Serial.print ("packet with id 0x");
    Serial.print (CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print (" and requested length ");
      Serial.println (CAN.packetDlc());
    } else {
      Serial.print (" and length ");
      Serial.println (packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print ((char) CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}