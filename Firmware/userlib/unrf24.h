#ifndef __UNRF24_H
#define __UNRF24_H
#include "stm32f1xx_hal.h"

		static const uint8_t nRF24_ADDR0[] = { 0x29, 0x73, 0x10 };
		
		
		
		
		
#define	O201 0x01 //ALLA
#define	O202 0x02 //AIR
#define	O203 0x03 //ANDREY
#define UID O203
		

		
		
// Used for not stuck waiting for IRQ
#define nRF24_WAIT_TIMEOUT         (uint32_t)0x000FFFFF
		#define payL1 1

// Result of packet transmission
typedef enum {
	nRF24_TX_ERROR  = (uint8_t)0x00, // Unknown error
	nRF24_TX_SUCCESS,                // Packet has been transmitted successfully
	nRF24_TX_TIMEOUT,                // It was timeout during packet transmit
	nRF24_TX_MAXRT                   // Transmit failed with maximum auto retransmit count
} nRF24_TXResult;


		
		
// Function prototypes
void nrfTXMULTI_START(void);
void nrfTXWHILE_START(void);
void nrfRXMULTI_START(void);
uint16_t nrfRXWHILE_START(void);
	nRF24_TXResult nRF24_TransmitPacket(uint8_t *pBuf, uint8_t length);
#endif // __UNRF24_H
