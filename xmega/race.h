#define START_MSG 1
#define POS_MSG 2

BlueOsTCB positionSensorTCB;
uint8_t positionSensorStack[STACKSIZE+1];

/**
 * This task retrieves the current position and lane of the
 * car by communicating with the XMEGA3 microcontroller.
 */
void xmega3GetPositionTask();

BlueOsTCB edisonCommTCB;
uint8_t edisonCommStack[STACKSIZE+1];

/**
 * This task handles the communication with the Edison board.
 * It receives the target speed and lane and interacts with the system.
 */
void edisonGetSpeedTask();