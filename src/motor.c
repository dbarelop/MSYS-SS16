/*
 * motor.c
 *
 * Created: 07.04.2016 20:00:11
 *  Author: da431lop
 */

#include <avr/io.h>

// TODO: disable motor when using brake and viceversa
// CTRLB -> CCAEN for motor; CCBEN for brake
// Configure pin as output (motor bit nr. 4 of register DIR; brake bit nr. 5)
void accelerate(int speed)
{
	speed = speed % 1000;
	TCD1_CTRLA = 0x01;					// Prescaler = 1
	TCD1_CTRLB = TC_WGMODE_SS_gc;		// Timer set in Single-slope PWM operation mode
	TCD1_CTRLC = 0x00;
	TCD1_CTRLD = 0x00;
	TCD1_PER = 0xFF;					// Period set to highest possible value
	TCD1_CCA = TCD1_PER / 1000 * speed;	// Compare value set to val equivalent for selected period
}

void brake()
{
	TCD1_CTRLA = 0x01;				// Prescaler = 1
	TCD1_CTRLB = TC_WGMODE_SS_gc;	// Timer set in Single-slope PWM operation mode
	TCD1_CTRLC = 0x00;
	TCD1_CTRLD = 0x00;
	TCD1_PER = 0xFF;				// Period set to highest possible value
	TCD1_CCA = 0xFF;				// Compare value set to selected period
}
