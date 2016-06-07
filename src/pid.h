BlueOsTCB pidTCB;
uint8_t pidStack[STACKSIZE+1];

/*
 * Starts the PID speed control task.
 */
void pid();