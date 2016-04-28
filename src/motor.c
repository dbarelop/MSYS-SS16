/*
 * motor.c
 *
 * Created: 07.04.2016 20:00:11
 *  Author: da431lop
 */

#include <avr/io.h>

void accelerate(int speed)
{
    // Disable the brake (CCAEN) and enable the motor (CCBEN)
    TCD1_CTRLB = TC1_CCAEN_bm;
    // Configure motor pin (port D, pin 4) as output
    PORTD.DIR = (0x01 << 4);
    //PORTD.DIR = 0b00010000;
    TCD1_CTRLA = 0x01;                    // Prescaler = 1
    TCD1_CTRLB |= TC_WGMODE_SS_gc;        // Timer set in Single-slope PWM operation mode
    TCD1_CTRLC = 0x00;
    TCD1_CTRLD = 0x00;
    TCD1_PER = 0xFF;                      // Period set to highest possible value
    // Set the speed
    TCD1_CCA = TCD1_PER * speed / 1000;   // Compare value set to value equivalent for selected period
}

void brake()
{
    // Disable the motor (CCBEN) and enable the brake (CCAEN)
    TCD1_CTRLB = TC1_CCBEN_bm;
    // Configure brake pin (port D, pin 5)
    PORTD.DIR = (0x01 << 5);
    //PORTD.DIR = 0b00100000;
    TCD1_CTRLA = 0x01;                // Prescaler = 1
    TCD1_CTRLB = TCD1_CTRLB | TC_WGMODE_SS_gc;    // Timer set in Single-slope PWM operation mode
    TCD1_CTRLC = 0x00;
    TCD1_CTRLD = 0x00;
    TCD1_PER = 0xFF;                // Period set to highest possible value
    TCD1_CCB = 0xFF;                // Compare value set to selected period
}
