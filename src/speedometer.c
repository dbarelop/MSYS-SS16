#include <avr/interrupt.h>
#include <avr/io.h>

#include "blue_os.h"
#include "constants.h"
#include "speedometer.h"

static unsigned int speed;
static volatile unsigned int ticks;

ISR(PORTB_INT0_vect)
{
	cli();
	ticks = TCD0.CNT;
	// Restart the timer
	TCD0.CTRLFSET = TC_CMD_RESTART_gc;
	sei();
}

void initSpeedometer()
{
	// Disable interrupts
	cli();
	// Initialize speed
	speed = 0;
	// Configure the speedometer pin (port B, pin 2) as input
	PORTB.DIR = 0x00;
	// Configure the speedometer pin to interrupt on every rising edge
	PORTB.INTCTRL = 0x02;				// Set the interrupt level for INT0 to MED
	PORTB.INT0MASK = (0x01 << 2);		// Activate the INT0 for pin 2
	PORTB.PIN2CTRL = PORT_ISC0_bm;		// Set the ISC (Input/sense configuration) to RISING
	// Configure the timer 0 to count the time between interruptions
	TCD0.CTRLA = 0x07;					// Prescaler = 1024
	TCD0.CTRLB = 0x00;
	TCD0.CTRLC = 0x00;
	TCD0.CTRLD = 0x00;
	TCD0.CTRLE = 0x00;
	TCD0.PER = (unsigned int) 0xFFFFFFFF;
	// Enable interrupts
	sei();
}

void measureSpeed()
{
	initSpeedometer();
	while (1)
	{
		speed = 2500000 / (CLK_MHZ * ticks);	// 2.5 * 1000000 / (CLK_MHZ * ticks)
		blueOsClearScreen();
		blueOsWriteString("Current speed: ");
		blueOsWriteInt(speed, 10);
		blueOsWriteString(" rps");
		blueOsDelay(250);
	}
}

int getCurrentSpeed()
{
	return ticks;
}

int getCurrentSpeedRPS()
{
	return speed;
}
