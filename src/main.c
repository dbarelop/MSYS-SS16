/*
 * main.c
 *
 * Created: 07.04.2016 19:17:38
 *  Author: da431lop
 */ 

#include "blue_os.h"
#include "motor.h"
#include <stdlib.h>

#define STACKSIZE 128
BlueOsTCB taskTCB;
uint8_t taskStack[STACKSIZE];

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

void helloWorld()
{
	while (1)
    {
		blueOsWriteString("Hello World!\n");
        blueOsDelay(1000);
    }
}

void testMotor()
{
	char buffer[8];
	int MAX_SPEED = 1000;
	int i = 0;
	int speed = 0, step = 50;
	while(1)
	{
		itoa(speed, buffer, 10);
		blueOsWriteString(buffer);
		blueOsWriteString(",");
		itoa(i, buffer, 10);
		blueOsWriteString(buffer);
		blueOsWriteString("\n");
		// Every 25 seconds, push the brake, wait for 2 seconds and restart speed
		if (i == 0)
		{
			speed = 0;
			step = 50;
			brake();
			blueOsDelay(2000);
		}
		accelerate(speed);
		speed += step;
		if (speed == 0 || speed == MAX_SPEED)
		{
			step *= -1;
		}
		i = (i + 1) % 1000;
		blueOsDelay(1000);
	}
}

void testVT100()
{
	blueOsInitShellVt100();
	while (1)
	{
		blueOsEnterGraphic();
		blueOsDrawMainFrame(1, 1, 20, 20);
		blueOsLeaveGraphic();
		blueOsSetPosition(10, 10);
		blueOsWriteString("Welcome");
		blueOsDelay(250);
	}
}

int main()
{
	// Configure the microcontroller to run at 32 MHz
	select32MHzClk();
	
	// Configure pin 3 from port F as output and pin 2 as input to use the UART (terminal)
    PORTF.DIR = 0b00001000;
	
	blueOsInit();
	
	blueOsCreateTask(&taskTCB, taskStack, STACKSIZE, 1, testVT100, 0);
	
    blueOsStart();
	
	return 0;
}

