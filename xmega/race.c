#include "BlueOS/source/blue_os.h"
#include "BlueOS/source/blue_os_synchronization.h"
#include "constants.h"
#include "edison/edison.h"
#include "ir-comm/comm.h"
#include "race.h"
#include "pid.h"

static BlueOsSema mutex;
static uint8_t target_lane, target_speed;
static volatile uint8_t current_lane, current_position;

uint8_t calc_checksum(uint8_t a, uint8_t b, uint8_t c)
{
	return (a + b + c) & 0xFF;
}

void xmega3GetPositionTask()
{
	uint16_t eventData;
	uint8_t rxBuffer[4];

	commInit();
	commShareVariable(0, rxBuffer);

	while (1)
	{
		// Listen for an event from the sensor
		blue_os_eventListen(&commEvent, 0, (uint8_t *) &eventData);

		blueOsAcquireSema(&mutex);
		current_lane = rxBuffer[0];
		current_position = rxBuffer[1];
		blueOsReleaseSema(&mutex);

		blueOsDelay(30);
	}
}

// TODO: move to GUI!

void print_received_data(uint8_t rx_msg_id, uint8_t target_speed, uint8_t target_lane, uint8_t rx_checksum)
{
	uint8_t valid = calc_checksum(rx_msg_id, target_speed, target_lane) == rx_checksum;
	blueOsWriteString("rx_msg_id = ");
	blueOsWriteInt(rx_msg_id, 3);
	blueOsWriteString("\n\rtarget_speed = ");
	blueOsWriteInt(target_speed, 3);
	blueOsWriteString("\n\rtarget_lane = ");
	blueOsWriteInt(target_lane, 3);
	blueOsWriteString("\n\rchecksum = ");
	blueOsWriteInt(rx_checksum, 3);
	if (!valid)
		blueOsWriteString(" (invalid checksum!)");
	blueOsWriteString("\n\r\n\r");
}

void print_sent_data(uint8_t tx_msg_id, uint8_t pos, uint8_t lane, uint8_t tx_checksum)
{
	blueOsWriteString("tx_msg_id = ");
	blueOsWriteInt(tx_msg_id, 3);
	blueOsWriteString("\n\rcurrent_position = ");
	blueOsWriteInt(pos, 3);
	blueOsWriteString("\n\rcurrent_lane = ");
	blueOsWriteInt(lane, 3);
	blueOsWriteString("\n\rchecksum = ");
	blueOsWriteInt(tx_checksum, 3);
	blueOsWriteString("\n\r\n\r");
}

void edisonGetSpeedTask()
{
	uint8_t rx_msg_id, rx_checksum;
	uint8_t tx_msg_id, tx_checksum;
	uint8_t lane, pos;
	uint8_t valid;
	
	EdisonInit();
	
	// Send start message
	tx_msg_id = START_MSG;
	tx_checksum = calc_checksum(tx_msg_id, 0, 0);
	blueOsQueueEnqueue(&edisonTxQueue, tx_msg_id & 0xFF);
	blueOsQueueEnqueue(&edisonTxQueue, 0 & 0xFF);
	blueOsQueueEnqueue(&edisonTxQueue, 0 & 0xFF);
	blueOsQueueEnqueue(&edisonTxQueue, tx_checksum & 0xFF);
	
	while (1)
	{
		while (edisonRxQueue._elementcount < 4);
		
		// Receive data
		blueOsQueueDequeue(&edisonRxQueue, &rx_msg_id);
		blueOsQueueDequeue(&edisonRxQueue, &target_speed);
		blueOsQueueDequeue(&edisonRxQueue, &target_lane);
		blueOsQueueDequeue(&edisonRxQueue, &rx_checksum);

		valid = calc_checksum(rx_msg_id, target_speed, target_lane) == rx_checksum;

		if (valid)
			setTargetSpeedRPS(target_speed);

		blueOsClearScreen();
		
		print_received_data(rx_msg_id, target_speed, target_lane, rx_checksum);
		
		// TODO: decide whether to do a lane switch!
		
		if (edisonTxQueue._elementcount < (edisonTxQueue._queuesize - 4))
		{
			blueOsAcquireSema(&mutex);
			lane = current_lane;
			pos = current_position;
			blueOsReleaseSema(&mutex);

			tx_msg_id = POS_MSG;
			tx_checksum = calc_checksum(tx_msg_id, pos, lane);
			
			print_sent_data(tx_msg_id, pos, lane, tx_checksum);

			// Send data to the Edison module. Note: Queue access is blocking!
			blueOsQueueEnqueue(&edisonTxQueue, tx_msg_id & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, pos & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, lane & 0xFF);
			blueOsQueueEnqueue(&edisonTxQueue, tx_checksum & 0xFF);
		}
		
		blueOsDelay(30);
	}
}
