#include <avr/interrupt.h>
#include <avr/io.h>

#include "BlueOS/source/blue_os.h"
#include "constants.h"
#include "speedometer.h"

static unsigned int speed_rps = 0;
static volatile unsigned int ticks = 0;

ISR(PORTB_INT0_vect)
{
	unsigned int time_ms;
	cli();
	ticks = TCD0.CNT;
	// Calculate the time needed for 1 revolution (ms)
	time_ms = 4 * (ticks + 1) / CLK_MHZ;	// freq = CLK / (presc * (ticks + 1)) (presc ~= 1000)
	// Calculate speed (rps)
	speed_rps = 10000 / time_ms;
	// Restart the timer
	TCD0.CTRLFSET = TC_CMD_RESTART_gc;
	sei();
}

void initSpeedometer()
{
	// Disable interrupts
	cli();
	// Configure the speedometer pin (port B, pin 2) as input
	PORTB.DIR = 0x00;
	// Configure the speedometer pin to interrupt on every rising edge
	PORTB.INTCTRL = 0x02;				// Set the interrupt level for INT0 to MED
	PORTB.INT0MASK = (0x01 << 2);		// Activate the INT0 for pin 2
	PORTB.PIN2CTRL = PORT_ISC0_bm;		// Set the ISC (Input/sense configuration) to RISING
	// Configure the timer 0 to count the time between interruptions
	// FIXME: measure a reasonable amount of ticks to wait to consider
	// the speed = 0 (add timer interrupt and tune TCD0.PER)
	TCD0.CTRLA = 0x07;					// Prescaler = 1024
	TCD0.CTRLB = 0x00;
	TCD0.CTRLC = 0x00;
	TCD0.CTRLD = 0x00;
	TCD0.CTRLE = 0x00;
	TCD0.PER = (unsigned int) 0xFFFFFFFF;
	// Enable interrupts
	sei();
}

int getCurrentSpeedRPS()
{
	return speed_rps;
}
