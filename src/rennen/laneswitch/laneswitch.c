#include "laneswitch.h"

/*
 * Diode for switching and car identification
 * Port F4, Capture/Compare Register A TCF1
 * 
 * use timer TCF1 for frequency
 * 
 */
void laneSwitchInit()
{
	/*
	 * diode
	 */
	// set prescaler which is stored in CLKSEL[3:0]
	TCF1.CTRLA |= TC_CLKSEL_DIV64_gc;
		
	// enable compare register a CCAEN[4]
	TCF1.CTRLB |= TC1_CCAEN_bm;
	
	// set waveform generation mode WGMODE[2:0]
	TCF1.CTRLB |= TC_WGMODE_FRQ_gc;
	
	// set the compare value registers for the identification
	TCF1.CCA = LANESWITCH_TIMER_COMPARE_ID;
	
	// set PF4 as output, always
	PORTF.DIRSET = (1 << 4);
}

/*
 * return 1 if switching, else 0
 */
uint8_t laneSwitchDiodeGet()
{
	if(TCF1.CCA == LANESWITCH_TIMER_COMPARE_SWITCH)
		return LANESWITCHDIODE_ON;
	else
		return LANESWITCHDIODE_OFF;
}

void laneSwitchDiodeToggle()
{
	if(laneSwitchDiodeGet() == LANESWITCHDIODE_OFF)
		TCF1.CCA = LANESWITCH_TIMER_COMPARE_SWITCH;
	else
		TCF1.CCA = LANESWITCH_TIMER_COMPARE_ID;
}

/*
 * if we want to switch lanes, set the CCA to the corresponding value
 */
void laneSwitchDiodeSet(uint8_t newvalue)
{
	if(newvalue == LANESWITCHDIODE_OFF)
		TCF1.CCA = LANESWITCH_TIMER_COMPARE_ID;
	else if(newvalue == LANESWITCHDIODE_ON)
		TCF1.CCA = LANESWITCH_TIMER_COMPARE_SWITCH;
}

