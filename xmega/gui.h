BlueOsTCB GUITCB, inputParserTCB;
uint8_t GUIStack[STACKSIZE+1], inputParserStack[STACKSIZE+1];

typedef enum 
{ 
	ACCELERATE = 0, 
	DECELERATE = 1, 
	STOP = 2
} action_t;

/*
 * Starts the GUI task. Allows to set the target speed of the car
 * and displays the current speed.
 */
void gui();

/*
 * Starts the input parsing task. Reads the user input and interacts with
 * the GUI.
 */
void inputParser();