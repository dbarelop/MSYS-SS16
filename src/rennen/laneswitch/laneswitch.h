#ifndef LANESWITCH_H_
#define LANESWITCH_H_

#include "blue_os.h"
#include "comm.h"

/**
 * This is the ID, which will be measured externally!  
 */
#define BLUERIDER_ID 1

/*
 * 1953.125 Hz for lane switching <=> CCA = 129
 * 1,14E-03 Hz for BlueRider ID3 <=> CCA = 155
 */
#define LANESWITCH_TIMER_COMPARE_SWITCH 129
#define LANESWITCH_TIMER_COMPARE_ID (139 + (BLUERIDER_ID * 8))

#define LANESWITCHDIODE_OFF 0
#define LANESWITCHDIODE_ON 1

#define LANEINFO_LANE_INDEX 0
#define LANEINFO_SECTOR_INDEX 1

void laneSwitchInit();
void laneSwitchDiodeToggle();
uint8_t laneSwitchDiodeGet();
void laneSwitchDiodeSet(uint8_t newvalue);

void laneswitchTask();

#endif /* LANESWITCH_H_ */