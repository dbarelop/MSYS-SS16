/*
 * main.c
 *
 * Created: 07.04.2016 19:17:38
 *  Author: da431lop
 */

#include "BlueOS/source/blue_os.h"
#include "constants.h"
#ifdef TEST_MOTOR
#include "motor_test.h"
#endif
#ifdef PID
#include "gui.h"
#include "pid.h"
#include "speedometer.h"
#endif
#ifdef RENNEN
#include "race.h"
#endif

void select32MHzClk()
{
	// 1 Set oscillator
	OSC.CTRL |= OSC_RC32MEN_bm;
	// 2. Wait for oscillator to stabilize
	while (! ((OSC.STATUS) & OSC_RC32MRDY_bm));
	// 3. Set Register Protection Signature
	CCP = CCP_IOREG_gc;
	// 4. Select clock source to 32 MHz
	CLK.CTRL = CLK_SCLKSEL0_bm;
}

#ifdef HELLO_WORLD
BlueOsTCB helloWorldTCB;
uint8_t helloWorldStack[STACKSIZE+1];

void helloWorld()
{
	while (1)
    {
		blueOsWriteString("Hello World!\n");
        blueOsDelay(1000);
    }
}
#endif

int main()
{
	// Configure the microcontroller to run at 32 MHz
	select32MHzClk();
	
	// Configure pin 3 from port F as output and pin 2 as input to use the UART (terminal)
    PORTF.DIR = 0b00001000;
	
	blueOsInit();
	
	#ifdef HELLO_WORLD
	blueOsCreateTask(&helloWorldTCB, helloWorldStack, STACKSIZE, 1, helloWorld, 0);
	#endif
	#ifdef TEST_MOTOR
	blueOsCreateTask(&motorTCB, motorStack, STACKSIZE, 1, motorTest, 0);
	#endif
	#ifdef PID
	initSpeedometer();
	//blueOsCreateTask(&GUITCB, GUIStack, STACKSIZE, 1, gui, 0);
	//blueOsCreateTask(&inputParserTCB, inputParserStack, STACKSIZE, 1, inputParser, 0);
	blueOsCreateTask(&pidTCB, pidStack, STACKSIZE, 1, pid, 0);
	#endif
	#ifdef RENNEN
	blueOsCreateTask(&positionSensorTCB, positionSensorTCB, STACKSIZE, 1, xmega3GetPositionTask, 0);
	blueOsCreateTask(&edisonCommTCB, edisonCommStack, STACKSIZE, 1, edisonGetSpeedTask, 0);
	#endif
	
    blueOsStart();
	
	return 0;
}

