/*
 * motor.c
 *
 * Created: 07.04.2016 20:00:11
 *  Author: da431lop
 */

#include <avr/io.h>

/*
 * Sets the speed of the motor to a given value between 0 and 1000
 */
void accelerate(int speed)
{
    // Disable the brake (CCAEN) and enable the motor (CCBEN)
    TCD1.CTRLB = TC1_CCAEN_bm;
    // Configure motor pin (port D, pin 4) as output
    PORTD.DIR = (0x01 << 4);
    TCD1.CTRLA = 0x01;                    // Prescaler = 1
    TCD1.CTRLB |= TC_WGMODE_SS_gc;        // Timer set in Single-slope PWM operation mode
    TCD1.CTRLC = 0x00;
    TCD1.CTRLD = 0x00;
    TCD1.PER = 1000;                      // Period set to 1000
    // Set the speed
    TCD1.CCA = speed;                     // Compare value set to selected value
}

void brake()
{
    // Disable the motor (CCBEN) and enable the brake (CCAEN)
    TCD1.CTRLB = TC1_CCBEN_bm;
    // Configure brake pin (port D, pin 5)
    PORTD.DIR = (0x01 << 5);
    TCD1.CTRLA = 0x01;                // Prescaler = 1
    TCD1.CTRLB = TCD1.CTRLB | TC_WGMODE_SS_gc;    // Timer set in Single-slope PWM operation mode
    TCD1.CTRLC = 0x00;
    TCD1.CTRLD = 0x00;
    TCD1.PER = 1000;                  // Period set to 1000
    TCD1.CCB = TCD1_PER;              // Compare value set to selected period
}
