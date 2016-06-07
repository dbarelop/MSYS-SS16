/*
 * main.c
 *
 * Created: 07.04.2016 19:17:38
 *  Author: da431lop
 */

#include <stdlib.h>

#include "blue_os.h"
#include "constants.h"
#include "gui.h"
#include "motor.h"

#define TEST_MOTOR
#define GUI

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

#ifdef TEST_MOTOR
BlueOsTCB motorTCB;
uint8_t motorStack[STACKSIZE+1];

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
#endif

#ifdef PID
BlueOsTCB pidTCB;
uint8_t pidStack[STACKSIZE+1];

void pid()
{
	// TODO: find out correct values
	const int Kp = ??, Ki = ??, Kd = ??;
	// TODO: implement using fixed point
	float input, output, err, ierr = 0, prev_err = 0;
	
	while (1)
	{
		err = setpoint - input;
		ierr += err;
		derr = err - prev_err;
		output = Kp*err + Ki*ierr + Kd*derr;
		accelerate(output);
		prev_err = err;
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
	blueOsCreateTask(&motorTCB, motorStack, STACKSIZE, 1, testMotor, 0);
	#endif
	#ifdef GUI
	blueOsCreateTask(&GUITCB, GUIStack, STACKSIZE, 1, gui, 0);
	#endif
	#ifdef PID
	blueOsCreateTask(&pidTCB, pidStack, STACKSIZE, 1, pid, 0);
	#endif
	
    blueOsStart();
	
	return 0;
}

