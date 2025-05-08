
/* Section : Includes*/

#ifndef BLE_CAN_H
#define	BLE_CAN_H

#include <STD_Types.h>
#include <CAN.h>

#define TX_GPIO_NUM   21  // Connects to CTX
#define RX_GPIO_NUM   22  // Connects to CRX

/* 1st BLE Module */
#define 	BLE1_ID_1stPART						0x009	//0b00000 001 001
#define		BLE1_ID_2ndPART						0x011	//0b00000 010 001
#define		BLE1_ID_3rdPART						0x019	//0b00000 011 001
#define 	BLE1_ID_4thPART						0x021	//0b00000 100 001
#define		BLE1_RSSI_FRAME						0x029	//0b00000 101 001

/* 2nd BLE Module */
#define 	BLE2_ID_1stPART						0x00A	//0b00000 001 010
#define		BLE2_ID_2ndPART						0x012	//0b00000 010 010
#define		BLE2_ID_3rdPART						0x01A	//0b00000 011 010
#define 	BLE2_ID_4thPART						0x022	//0b00000 100 010
#define		BLE2_RSSI_FRAME						0x02A	//0b00000 101 010

/* 3rd BLE Module */
#define 	BLE3_ID_1stPART						0x00B	//0b00000 001 011
#define		BLE3_ID_2ndPART						0x013	//0b00000 010 011
#define		BLE3_ID_3rdPART						0x01B	//0b00000 011 011
#define 	BLE3_ID_4thPART						0x023	//0b00000 100 011
#define		BLE3_RSSI_FRAME						0x02B	//0b00000 101 011

/* 4th BLE Module */
#define 	BLE4_ID_1stPART						0x00C	//0b00000 001 100
#define		BLE4_ID_2ndPART						0x014	//0b00000 010 100
#define		BLE4_ID_3rdPART						0x01C	//0b00000 011 100
#define 	BLE4_ID_4thPART						0x024	//0b00000 100 100
#define		BLE4_RSSI_FRAME						0x02C	//0b00000 101 100


void canInit(void);
void canSender(int id, char* message, int message_len);
void canReceiver(void);

#endif	/* BLE_CAN_H */