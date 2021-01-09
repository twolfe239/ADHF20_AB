#include "unrf24.h"
#include "nrf24spi1.h"
#include "ssd1306.h"
#include "stm32f1xx_hal.h"

// Buffer to store a payload of maximum width
uint8_t nRF24_payload[32];

// Pipe number
nRF24_RXResult pipe;

// Length of received payload
uint8_t payload_length;

//------------------------------------------------------------------ TX HELP

nRF24_TXResult tx_res;

// Function to transmit data packet
// input:
//   pBuf - pointer to the buffer with data to transmit
//   length - length of the data buffer in bytes
// return: one of nRF24_TX_xx values
nRF24_TXResult nRF24_TransmitPacket(uint8_t *pBuf, uint8_t length) {
	volatile uint32_t wait = nRF24_WAIT_TIMEOUT;
	uint8_t status;

	// Deassert the CE pin (in case if it still high)
	nRF24_CE_L();

	// Transfer a data from the specified buffer to the TX FIFO
	nRF24_WritePayload(pBuf, length);

	// Start a transmission by asserting CE pin (must be held at least 10us)
	nRF24_CE_H();
	HAL_Delay(1);
	// Poll the transceiver status register until one of thMAX_RT - means the maximum number of TX retransmits happenede following flags will be set:
	//   TX_DS  - means the packet has been transmitted
	//   
	// note: this solution is far from perfect, better to use IRQ instead of polling the status
	do {
		status = nRF24_GetStatus();
		if (status & (nRF24_FLAG_TX_DS | nRF24_FLAG_MAX_RT )) {
			break;
		}
	} while (wait--);

	// Deassert the CE pin (Standby-II --> Standby-I)
	nRF24_CE_L();

	if (!wait) {
		// Timeout
		return nRF24_TX_TIMEOUT;
	}

	// Check the flags in STATUS register
//	UART_SendHex8(status);

	// Clear pending IRQ flags
	nRF24_ClearIRQFlags();

	if (status & nRF24_FLAG_MAX_RT) {
		// Auto retransmit counter exceeds the programmed maximum limit (FIFO is not removed)
		return nRF24_TX_MAXRT;
	}

	if (status & nRF24_FLAG_TX_DS) {
		// Successful transmission
		return nRF24_TX_SUCCESS;
	}

	// Some banana happens, a payload remains in the TX FIFO, flush it
	nRF24_FlushTX();

	return nRF24_TX_ERROR;
}

void nrfTXMULTI_START(void) {
	//------------------------------------------------------------------ TX MULTI START
	// This is simple transmitter (to multiple logic addresses):
	//   - TX addresses and payload lengths:
	//   - RF channel: 115 (2515MHz)
	//   - data rate: 250kbps (minimum possible, to increase reception reliability)
	//   - CRC scheme: 2 byte

	// The transmitter sends a data packets to the three logic addresses without Auto-ACK (ShockBurst disabled)
	// The payload length depends on the logic address

	// Set RF channel
	nRF24_SetRFChannel(107);

	// Set data rate
	nRF24_SetDataRate(nRF24_DR_250kbps);

	// Set CRC scheme
	nRF24_SetCRCScheme(nRF24_CRC_2byte);

	// Set address width, its common for all pipes (RX and TX)
	nRF24_SetAddrWidth(3);

	nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR0); // program TX address
	nRF24_SetAddr(nRF24_PIPE0, nRF24_ADDR0); // program pipe#0 RX address, must be same as TX (for ACK packets)

	// Set TX power (maximum)
	nRF24_SetTXPower(nRF24_TXPWR_0dBm);

	// Configure auto retransmit: 10 retransmissions with pause of 2500s in between
	nRF24_SetAutoRetr(nRF24_ARD_4000us, 10);

	//nRF24_EnableAA(nRF24_PIPE0);
	nRF24_DisableAA(nRF24_PIPE0);
	// Set operational mode (PTX == transmitter)
	nRF24_SetOperationalMode(nRF24_MODE_TX);

	// Clear any pending IRQ flags
	nRF24_ClearIRQFlags();

	// Wake the transceiver
	nRF24_SetPowerMode(nRF24_PWR_UP);

	//------------------------------------------------------------------ TX MULTI STOP		
}

void nrfRXMULTI_START(void) {
	//------------------------------------------------------------------ RX MULTI START	
	// This is simple receiver with multiple RX pipes:
	//   - RF channel: 115 (2515MHz)
	//   - data rate: 250kbps (minimum possible, to increase reception reliability)
	//   - CRC scheme: 2 byte

	// The transmitter sends packets of different length to the three different logical addresses,
	// cycling them one after another, that packets comes to different pipes (0, 1 and 4)

	// Set RF channel
	nRF24_SetRFChannel(107);

	// Set data rate
	nRF24_SetDataRate(nRF24_DR_250kbps);

	// Set CRC scheme
	nRF24_SetCRCScheme(nRF24_CRC_2byte);

	// Set address width, its common for all pipes (RX and TX)
	nRF24_SetAddrWidth(3);

	// Configure RX PIPE#0
	nRF24_SetAddr(nRF24_PIPE1, nRF24_ADDR0);
	nRF24_SetRXPipe(nRF24_PIPE1, nRF24_AA_OFF, payL1);

	// Set TX power for Auto-ACK (maximum, to ensure that transmitter will hear ACK reply)
	//	nRF24_SetTXPower(nRF24_TXPWR_0dBm);

	// Set operational mode (PRX == receiver)
	nRF24_SetOperationalMode(nRF24_MODE_RX);

	// Clear any pending IRQ flags
	nRF24_ClearIRQFlags();

	// Wake the transceiver
	nRF24_SetPowerMode(nRF24_PWR_UP);

	// Put the transceiver to the RX mode
	nRF24_CE_H();
	//------------------------------------------------------------------ RX MULTI STOP	

}

void nrfTXWHILE_START(void) {
	//------------------------------------------------------------------ !!!TX WHILE START!!!
	payload_length = 0;
	nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR0);
	payload_length = payL1;
	nRF24_payload[0] = UID;
	// Transmit a packet
	tx_res = nRF24_TransmitPacket(nRF24_payload, payload_length);
	// Wait ~0.5s
	HAL_Delay(500);

	//------------------------------------------------------------------ !!!TX WHILE STOP!!
}

uint16_t nrfRXWHILE_START(void) {
	//------------------------------------------------------------------ !!!RX WHILE START!!!
	//
	// Constantly poll the status of the RX FIFO and get a payload if FIFO is not empty
	//
	// This is far from best solution, but it's ok for testing purposes
	// More smart way is to use the IRQ pin :)
	//
	if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY) {
		// Get a payload from the transceiver
		pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);
		// Clear all pending IRQ flags
		nRF24_ClearIRQFlags();
	}
	return nRF24_payload[0];
	//------------------------------------------------------------------ !!!RX WHILE STOP!!!
}

