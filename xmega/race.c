#include "BlueOS/source/blue_os.h"
#include "constants.h"
#include "edison/edison.h"
#include "race.h"

uint8_t calc_checksum(uint8_t msg_id, uint8_t pos, uint8_t lane)
{
	return (msg_id + pos + lane) & 0xFF;
}

void edisonCommGetSpeedTask()
{
	static uint8_t msg_id, speed, pos, lane, checksum;
	
	EdisonInit();
	
	// Send start message
	msg_id = START_MSG; pos = 0; lane = 0; checksum = calc_checksum(msg_id, pos, lane);
	blueOsQueueEnqueue(&edisonTxQueue, msg_id & 0xFF);
	blueOsQueueEnqueue(&edisonTxQueue, pos & 0xFF);
	blueOsQueueEnqueue(&edisonTxQueue, lane & 0xFF);
	blueOsQueueEnqueue(&edisonTxQueue, checksum & 0xFF);
	
	while (1)
	{
		while (edisonRxQueue._elementcount < 4);
		
		// Receive data
		blueOsQueueDequeue(&edisonRxQueue, &msg_id);
		blueOsQueueDequeue(&edisonRxQueue, &speed);
		blueOsQueueDequeue(&edisonRxQueue, &lane);
		blueOsQueueDequeue(&edisonRxQueue, &checksum);
		
		blueOsClearScreen();
		
		// Print received data
		blueOsWriteInt(msg_id, 3);
		blueOsWriteInt(speed, 3);
		blueOsWriteInt(lane, 3);
		blueOsWriteInt(checksum, 3);
		blueOsShellWriteChar('\n');
		blueOsShellWriteChar('\r');
		
		blueOsShellWriteChar('\n');
		blueOsShellWriteChar('\r');
		
		if (edisonTxQueue._elementcount < (edisonTxQueue._queuesize - 4))
		{
			msg_id = POS_MSG; pos = 0; lane = 0; checksum = calc_checksum(msg_id, pos, lane);
			
			// Print sent data
			blueOsWriteInt(msg_id, 3);
			blueOsWriteInt(pos, 3);
			blueOsWriteInt(lane, 3);
			blueOsWriteInt(checksum, 3);
			blueOsShellWriteChar('\n');
			blueOsShellWriteChar('\r');

			// Send data to the Edison module. Note: Queue access is blocking!
			blueOsQueueEnqueue(&edisonTxQueue, msg_id & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, pos & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, lane & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, checksum & 0xFF);
		}
		
		blueOsDelay(30);
	}
}
