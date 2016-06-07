#include <avr/interrupt.h>
#include <avr/io.h>

#include "blue_os.h"
#include "constants.h"
#include "speedometer.h"

int speed;
volatile int ticks;

ISR(PORTB_INT0_vect)
{
	cli();
	// TODO: complete code to calculate speed
	ticks = TCD0_CNT;
	// Restart the timer
	TCD0_CTRLFCLR = TC_CMD_RESTART_gc;
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
	TCD0_CTRLA = 0x01;					// Prescaler = 1
	TCD0_CTRLB = 0x00;
	TCD0_CTRLC = 0x00;
	TCD0_CTRLD = 0x00;
	TCD0_CTRLE = 0x00;
	TCD0_PER = 0xFF;
	// Enable interrupts
	sei();
}

void measureSpeed()
{
	// TODO: implement
	initSpeedometer();
	while (1)
	{
		speed = ticks * 4;
		blueOsClearScreen();
		blueOsWriteString("Current speed: ");
		blueOsWriteInt(speed, 10);
		blueOsWriteString(" rpc");
		blueOsDelay(250);
	}
}

int getCurrentSpeed()
{
	// TODO: implement
	return -1;
}
