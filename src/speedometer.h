BlueOsTCB speedometerTCB;
uint8_t speedometerStack[STACKSIZE+1];

/*
 * Starts the speed measuring task.
 */
void measureSpeed();

/*
 * Returns the current speed of the car.
 */
int getCurrentSpeed();
