#include "edison.h"
#include "spi_driver.h"

#define EDISON_BUFFER_LEN 32
BlueOsQueue edisonTxQueue;
uint8_t _edisonTxQueue[EDISON_BUFFER_LEN];

struct BlueOsSignal _edisonTxQueueEmpty;

BlueOsQueue edisonRxQueue;
uint8_t _edisonRxQueue[EDISON_BUFFER_LEN];

struct BlueOsSignal _edisonRxQueueFull;

SPI_Slave_t spiSlaveC = {NULL, NULL};
	
void EdisonInit()
{
	// Initialize SPI slave on port C
	SPI_SlaveInit(&spiSlaveC,
	              &SPIC,
	              &PORTC,
	              false,
	              SPI_MODE_3_gc,
	              SPI_INTLVL_HI_gc);
				  
	//blueOsQueueInit(&edisonTxQueue, _edisonTxQueue, EDISON_BUFFER_LEN);
	//blueOsQueueInit(&edisonRxQueue, _edisonRxQueue, EDISON_BUFFER_LEN);
	
	blueOsInitSignal(&_edisonTxQueueEmpty);
	blueOsInitSignal(&_edisonRxQueueFull);
	blueOsQueueInitBlocking(&edisonTxQueue, _edisonTxQueue, EDISON_BUFFER_LEN, &_edisonTxQueueEmpty, 0);
	blueOsQueueInitBlocking(&edisonRxQueue, _edisonRxQueue, EDISON_BUFFER_LEN, 0, &_edisonRxQueueFull);
}


#define PACKET_BURST_LEN_MAX 8

static uint8_t txEscape = 0;
static void edisonSendByte()
{
	uint8_t sendByte = 0xFF;
	if (txEscape)
	{
		sendByte = txEscape;
		txEscape = 0;
	}
	else if (blueOsQueueDequeue(&edisonTxQueue, &sendByte) == 0)
	{
		switch (sendByte)
		{
			case 0x00: // Invalid byte
			{
				sendByte = 0xA5;
				txEscape = 0xA1;
				break;
			}
			
			case 0xA5: // Escape byte
			{
				sendByte = 0xA5;
				txEscape = 0xA2;
				break;
			}
			
			case 0xFF: // Idle byte
			{
				sendByte = 0xA5;
				txEscape = 0xA3;
				break;
			}
			
			default:
				break;
		}
	}
	SPI_SlaveWriteByte(&spiSlaveC, sendByte);
}

static uint8_t rxEscape = 0;
static uint8_t dataInvalid = 0;
static int16_t receiveByte()
{
	int16_t insertByte = -1;
	
	uint8_t rxByte = SPI_SlaveReadByte(&spiSlaveC);
		
	if (spiSlaveC.module->STATUS & SPI_WRCOL_bm)
		dataInvalid = 1;
	
	if (rxEscape)
	{
		rxEscape = 0;
		switch (rxByte)
		{
			case 0xA1: // Invalid byte
			{
				insertByte = 0x00;
				break;
			}
			
			case 0xA2: // Escape byte
			{
				insertByte = 0xA5;
				break;
			}
			
			case 0xA3: // Idle byte
			{
				insertByte = 0xFF;
				break;
			}
			
			case 0xA5: // Escape byte
			{
				rxEscape = 1;
				break;
			}
			
			case 0x00:
			case 0xFF:
				break;
			
			default:
				insertByte = rxByte;
				break;
		}
	}
	else
	{
		switch (rxByte)
		{
			case 0xA5: // Escape byte
			{
				rxEscape = 1;
				break;
			}
			
			case 0x00: // Invalid byte
			case 0xFF: // Idle byte
				break;
			
			default:
			{
				insertByte = rxByte;
				break;
			}	
		}
	}
	
	/*if (insertByte >= 0)
	{
		blueOsQueueEnqueue(&edisonRxQueue, (uint8_t) insertByte);
	}*/
	return insertByte;
}

static uint8_t rxBuffer[PACKET_BURST_LEN_MAX];
ISR(SPIC_INT_vect)
{
	uint8_t i = 0;
	uint8_t j = 0;
	dataInvalid = 0;
	
	uint8_t rxEscapeBackup = rxEscape;
	do 
	{
		edisonSendByte();
		int16_t rxByte = receiveByte();
		if (rxByte >= 0)
		{
			rxBuffer[i] = (uint8_t) rxByte;
			i++;
		}
	} while (/*!(PORTC.IN & (1 << 4))*/
			(spiSlaveC.module->STATUS &  SPI_IF_bm)
			&& (i < PACKET_BURST_LEN_MAX)); // Keep in ISR for the whole burst
	
	if (dataInvalid)
	{
		rxEscape = rxEscapeBackup;
		return;
	}
	
	for (j = 0; j < i; j++)
	{
		blueOsQueueEnqueue(&edisonRxQueue, rxBuffer[j]);
	}
}
