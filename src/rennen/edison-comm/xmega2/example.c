#include "edison.h"
/**
 * This task just receives data from the Edison module and sends an incrementing ID back.
 */
int edisonCommTask()
{
	static uint32_t id = 0;
	static uint8_t rxBuffer[4];
	
	// Initialize SPI communication
	EdisonInit();
	
	while (1)
	{
		uint8_t retvalue;
		
		while (edisonRxQueue._elementcount < 4)
			;
		
		for (uint8_t i = 0; i < 4; i++)
		{
			// Receive data from the Wdison module. Note: Queue access is blocking!
			blueOsQueueDequeue(&edisonRxQueue, &retvalue);
			rxBuffer[i] = retvalue;
		}
		
		// Print received data
		for (uint8_t i = 0; i < 4; i++)
		{
			blueOsWriteInt(rxBuffer[i], 3);
		}
		blueOsShellWriteChar('\n');
		blueOsShellWriteChar('\r');
		
		if (edisonTxQueue._elementcount < (edisonTxQueue._queuesize - 4))
		{
			// Send data to the Edison module. Note: Queue access is blocking!
			blueOsQueueEnqueue(&edisonTxQueue, (id >> 24) & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, (id >> 16) & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, (id >> 8) & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, (id >> 0) & 0xFF);
			id++;
		}
	}
}
