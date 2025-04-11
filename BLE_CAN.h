
/* Section : Includes*/

#ifndef BLE_CAN_H
#define	BLE_CAN_H

#include <STD_Types.h>
#include <CAN.h>

#define TX_GPIO_NUM   21  // Connects to CTX
#define RX_GPIO_NUM   22  // Connects to CRX

void canInit(void);
void canSender(uint16 ID , uint8 size  ,uint8* ptr  );
void canReceiver(void);

#endif	/* BLE_CAN_H */