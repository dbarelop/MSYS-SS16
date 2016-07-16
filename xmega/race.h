#define START_MSG 1
#define POS_MSG 2

BlueOsTCB rennenTCB;
uint8_t rennenStack[STACKSIZE+1];

void edisonCommGetSpeedTask();