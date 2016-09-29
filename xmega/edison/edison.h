#ifndef EDISON_H
#define EDISON_H

#include "../BlueOS/source/blue_os.h"

/**
 * This function initializes the SPI communication with the Edison module. The
 * whole communication is set up as a bytestream, but it can contain communication
 * errors. This means, that data can be wrong, or data can be lost.
 */
void EdisonInit();

/**
 * This queue is the bytestream to be sent to the Edison module.
 * Note: Writing to this queue is blocking.
 */
extern BlueOsQueue edisonTxQueue;


/**
 * This queue is the bytestream, which is received from the Edison module.
 * Note: Reading from this queue is blocking.
 */
extern BlueOsQueue edisonRxQueue;


#endif // EDISON_H
