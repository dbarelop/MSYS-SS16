
/********************************************************************************
*Copyright (C) 2010 	Moritz Nagel 	mnagel@htwg-konstanz.de					*
*					Daniel Urbanietz 	daniel@d-urbanietz.de					*
*********************************************************************************
*FILE: blue_os_twi.h															*
*																				*
*AUTHOR(S): Daniel Urbanietz													*
*																				*
*DESCRIPTION:																	*
*This module provides a multimaster TWI interface. You can configure it in the	*
*\BlueOS\config\blue_os_config.h . 												*
*																				*
*VERSION: 1.0.																	*
*																				*			
*********************************************************************************
*This file is part of BlueOS.													*
*																				*
*BlueOS is free software: you can redistribute it and/or modify					*
*it under the terms of the GNU Lesser General Public License as published by	*
*the Free Software Foundation, either version 3 of the License, or				*
*(at your option) any later version.											*
*																				*
*BlueOS is distributed in the hope that it will be useful,						*
*but WITHOUT ANY WARRANTY; without even the implied warranty of					*
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					*
*GNU Lesser General Public License for more details.							*
*																				*
*You should have received a copy of the GNU Lesser General Public License		*
*along with BlueOS.  If not, see <http://www.gnu.org/licenses/>.				*
********************************************************************************/


#ifndef BLUE_OS_TWI_H
#define BLUE_OS_TWI_H

#include "../config/blue_os_config.h"
#include "../config/blue_os_hardware.h"

#ifdef BLUE_OS_TWI

	#ifndef TWI_SPEC
		#error "TWI is not supported for this device"
	#endif

#include <util/twi.h>
#include "blue_os_synchronization.h"


#define TWI_WAIT_VAL 5

#define MAX_ITER 100

#define TWI_READY 0
#define TWI_MASTER_TRANS 1
#define TWI_MASTER_RECV  2
#define TWI_SLAVE_TRANS	 3
#define TWI_SLAVE_RECV   4
#define TWI_ARBIT_LOST	 5
#define TWI_ERROR		 6
#define TWI_SLA_NA		 7
#define TWI_DAT_NA		 8

//#define TWI_DEBUG
//#define TWI_MEASURE


typedef volatile struct _blue_twi_message
{
	uint8_t header;
	uint8_t data[5];
}blue_twi_message;

#ifdef BLUE_OS_TWI_SLAVE
	extern BlueOsEvent blue_os_twiEvent;
#endif

extern volatile uint8_t twi_last_action;

extern volatile int8_t twi_listening;

extern volatile uint8_t last_twst;
extern volatile uint8_t last_cmd;
extern volatile int8_t listr;
extern volatile uint8_t twi_state;



uint8_t blue_os_twi_getState();

int8_t blue_os_twi_init();
int8_t blue_os_twi_send(uint8_t addr, blue_twi_message msg);

#ifdef TWI_MEASURE
uint16_t getRecvTicks();
#endif //TWI_MEASURE

#endif
#endif
