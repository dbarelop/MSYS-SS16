BlueOsTCB pidTCB;
uint8_t pidStack[STACKSIZE+1];

/*
 * Starts the PID speed control task.
 */
void pid();

/*
 * Sets the target speed for the PID controller (Sollwert).
 */
void setTargetSpeed(int targetSpeed);