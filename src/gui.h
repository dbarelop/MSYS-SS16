BlueOsTCB GUITCB;
uint8_t GUIStack[STACKSIZE+1];

/*
 * Starts the GUI task. Allows to set the target speed of the car
 * and displays the current speed.
 */
void gui();