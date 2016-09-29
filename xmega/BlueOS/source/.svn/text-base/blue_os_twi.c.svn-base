
/********************************************************************************
*Copyright (C) 2010 	Moritz Nagel 	mnagel@htwg-konstanz.de					*
*					Daniel Urbanietz 	daniel@d-urbanietz.de					*
*********************************************************************************
*FILE: blue_os_twi.c															*
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


#include <avr/io.h>
#include <avr/interrupt.h>
#include "blue_os_task.h"
#include "blue_os_twi.h"
#include "blue_os_shell.h"

#include "blue_os_shell_vt100.h"

#ifdef BLUE_OS_TWI

#ifdef BLUE_OS_TWI_SLAVE
	void blue_os_twi_listen();
	volatile uint8_t twi_tmp_msgRX[sizeof(blue_twi_message)];
	BlueOsEvent blue_os_twiEvent;
	volatile int8_t twi_listening;
#endif //BLUE_OS_TWI_SLAVE

#ifdef BLUE_OS_TWI_MASTER
	volatile BlueOsSignal twi_sent;
	volatile uint8_t twi_tmp_msgTX[sizeof(blue_twi_message)];
	volatile uint8_t blue_twi_transmit;
	volatile uint8_t twi_addr;

	#ifdef BLUE_OS_USE_SEMA
		BlueOsSema twi_sema;
	#endif //BLUE_OS_USE_SEMA

#endif //BLUE_OS_TWI_MASTER


volatile uint8_t twi_last_action;
volatile uint8_t twi_state;
volatile uint8_t last_twst;



#ifdef TWI_MEASURE
volatile uint16_t recv_tick;
#endif //TWI_MEASURE




//ACK nach empfangenen Daten senden/ ACK nach gesendeten Daten erwarten
#define TWCR_ACK TWCR  = _BV(TWINT)|_BV(TWEN)|_BV(TWIE)|_BV(TWEA)
//NACK nach empfangenen Daten senden/ NACK nach gesendeten Daten erwarten     
#define TWCR_NACK TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWIE)
//switched to the non adressed slave mode...
#define TWCR_RESET TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWSTO)  

//send STOP-Condition
#define TWCR_STOP TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE)|(1<<TWSTO) 

//send START-Condition
#define TWCR_START TWCR = (1<<TWSTA) | (1<<TWINT) | (1<<TWEN) | (1<<TWIE) 


int8_t blue_os_twi_init()
{
	
	uint8_t crit = blueOsEnterCriticalSection();
		TWI_DDR_SCL &= ~(1 << TWI_SCL);
		TWI_DDR_SDL &= ~(1 << TWI_SDL);
		TWI_PORT_SCL |= (1 << TWI_SCL);
		TWI_PORT_SCL |= (1 << TWI_SDL);

		#ifdef BLUE_OS_TWI_SLAVE
			blue_os_eventInit(&blue_os_twiEvent, sizeof(blue_twi_message),blue_os_twi_listen);
			TWAR = ( BLUE_OS_TWI_OWN_ADDR <<1)+BLUE_OS_TWI_BROADCAST;
			twi_listening = 0;
		#endif //BLUE_OS_TWI_SLAVE

		#ifdef BLUE_OS_TWI_MASTER
			blueOsInitSignal(&twi_sent);
			blue_twi_transmit = 0;
			TWBR = 16;
			#ifdef BLUE_OS_USE_SEMA
				blueOsInitSema(&twi_sema);
			#endif //BLUE_OS_USE_SEMA
		#endif //BLUE_OS_TWI_MASTER
		
			
		TWCR = (1<<TWEN) | (1<<TWIE);

		twi_state = TWI_READY;
		

		#ifdef TWI_MEASURE
		recv_tick = 0; //TESTING
		#endif
	blueOsLeaveCriticalSection(crit);

	return 0;
}

#ifdef BLUE_OS_TWI_SLAVE
	void blue_os_twi_listen()
	{
		twi_listening = 1;
		TWCR |= (1<<TWEA);
	}
#endif //BLUE_OS_TWI_SLAVE

#ifdef BLUE_OS_TWI_MASTER
int8_t blue_os_twi_send(uint8_t addr, blue_twi_message msg) 
{
	#ifdef BLUE_OS_USE_SEMA
		blueOsAcquireSema(&twi_sema);
	#endif //BLUE_OS_USE_SEMA
	uint8_t crit = blueOsEnterCriticalSection();
	uint8_t retval = 0;
	
	//Copy the message into the tranceive buffer
	*((blue_twi_message*) twi_tmp_msgTX) = msg;

	twi_addr = addr;//(addr<<1) + TW_WRITE;

	//How many retries, when the listener is not ready
	blue_twi_transmit = 50;

	//send Start-Condition
	TWCR |= (1<<TWSTA); 

	if(blueOsWait(&twi_sent) < 0)
	{		
		blue_twi_transmit = 0;
		retval = -1;
	}
	if(blue_twi_transmit)
	{		
		blue_twi_transmit = 0;		
		retval =  -1;
	}		
	blueOsLeaveCriticalSection(crit);
	#ifdef BLUE_OS_USE_SEMA
		blueOsReleaseSema(&twi_sema);
	#endif //BLUE_OS_USE_SEMA
	return retval;
}
#endif // BLUE_OS_TWI_MASTER

#ifdef TWI_MEASURE
uint16_t getRecvTicks()
{
	static uint32_t lastTime = 0;
	uint8_t crit_state = blueOsEnterCriticalSection();
//	uint16_t tmp = (lastTime = blueOsGetMs()) - lastTime;
	uint16_t tmp = recv_tick;
	recv_tick = 0;
	blueOsLeaveCriticalSection(crit_state);
	return tmp;
}
#endif


ISR(TWI_vect)
{	
	volatile uint8_t tmp;

	static volatile uint8_t twi_msg_ptrTX = 0;
	static volatile uint8_t twi_msg_ptrRX = 0;
	static volatile uint8_t check_sum = 0;
	volatile uint8_t ack_bit = 0;

	#ifdef BLUE_OS_TWI_SLAVE
		if(twi_listening)
			ack_bit = (1<<TWEA);
	#endif

	switch (last_twst = TW_STATUS) 
	{

	#ifdef BLUE_OS_TWI_MASTER
		case TW_START:		//0x08 start condition transmitted
		case TW_REP_START:	//0x10 repeated start condition transmitted
			twi_msg_ptrTX = 0;
			check_sum = 0;
			TWDR = (twi_addr<<1);//twi_addr;
			TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWIE) | ack_bit;
			break;

//MASTER-TRANSCEIVER
		  
		case TW_MT_SLA_NACK://0x20 SLA+W transmitted, NACK received
			twi_state = TWI_READY;
			if(blue_twi_transmit >0)
			{
				blue_twi_transmit--;
				TWCR_STOP |_BV(TWSTA) | ack_bit;
			}
			else
			{
				blue_twi_transmit = 1;
				blueOsSignalSingle(&twi_sent);
				TWCR_STOP | ack_bit;
			}		
			break;

		case TW_MT_SLA_ACK:	//0x18 SLA+W transmitted, ACK received
		case TW_MT_DATA_ACK://0x28 data transmitted, ACK received
			if(twi_msg_ptrTX < sizeof(blue_twi_message))
			{
				check_sum += twi_tmp_msgTX[twi_msg_ptrTX];
				TWDR = twi_tmp_msgTX[twi_msg_ptrTX++];
				TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWIE) | ack_bit;
			}
			else if(twi_msg_ptrTX == sizeof(blue_twi_message))
			{
				twi_msg_ptrTX++;
				TWDR = check_sum;
				TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWIE) | ack_bit;
			}	
			else
			{
				TWDR = 0x00;
				TWCR |= (1<<TWINT);
				while (!(TWCR & (1<<TWINT)));

				twi_last_action = TWI_MASTER_TRANS;
				blue_twi_transmit = 0;
				blueOsSignalSingle(&twi_sent);
				TWCR_STOP | ack_bit;
			}
			break; 

		case TW_MT_DATA_NACK://0x30 data transmitted, NACK received
			twi_last_action = TWI_DAT_NA;
			blueOsSignalSingle(&twi_sent);
			TWCR_STOP | ack_bit;
			break;
		case TW_MT_ARB_LOST://0x38 arbitration lost in SLA+W or data
			TWCR_START | ack_bit; //retry when the bus is free
			break;

//MASTER-RECEIVER
//		case TW_MR_ARB_LOST://0x38 arbitration lost in SLA+R or NACK
/*
		case TW_MR_SLA_ACK://0x40 SLA+R transmitted, ACK received
		//	TWCR_ACK;
			break;			
		case TW_MR_DATA_ACK://0x50 data received, ACK returned
			if(twi_msg_ptrTX < (sizeof(blue_twi_message)-2))
				((uint8_t*)&twi_tmp_msgTX)[twi_msg_ptrTX++] = TWDR;
			else
			{
				twi_state = TWI_READY;
				TWCR_STOP;
			}
			break;
					
		case TW_MR_DATA_NACK://0x58 data received, NACK returned
			((uint8_t*)&twi_tmp_msgTX)[twi_msg_ptrTX++] = TWDR;			
		case TW_MR_SLA_NACK://0x48 SLA+R transmitted, NACK received
			twi_state = TWI_READY;
			twi_msg_ptrTX = 0;
			TWCR_STOP;
			break;
	*/
	#endif // BLUE_OS_TWI_MASTER

	#ifdef BLUE_OS_TWI_SLAVE
	/*
//SLAVE-TRANSCEIVER
		case TW_ST_SLA_ACK://0xA8 SLA+R received, ACK returned
		case TW_ST_ARB_LOST_SLA_ACK://0xB0 arbitration lost in SLA+RW, SLA+R received, ACK returned
		case TW_ST_DATA_ACK://0xB8 data transmitted, ACK received
			TWDR = 0;
			TWCR_ACK;
			break;

		case TW_ST_DATA_NACK://0xC0 data transmitted, NACK received
		case TW_ST_LAST_DATA://0xC8 last data byte transmitted, ACK received
			twi_state = TWI_READY;
			TWCR_ACK;
			break;
	*/
//SLAVE-RECEIVER
		case TW_SR_ARB_LOST_GCALL_ACK://0x78 arbitration lost in SLA+RW, general call received, ACK returned
		case TW_SR_ARB_LOST_SLA_ACK://0x68 arbitration lost in SLA+RW, SLA+W received, ACK returned
			twi_state = TWI_SLAVE_RECV;
			twi_msg_ptrRX = 0;
			check_sum = 0;
			TWCR_ACK | (1<<TWSTA);
			break;
		case TW_SR_GCALL_ACK://0x70 general call received, ACK returned
		case TW_SR_SLA_ACK://0x60 SLA+W received, ACK returned
			twi_state = TWI_SLAVE_RECV;
			twi_msg_ptrRX = 0;
			check_sum = 0;
			TWCR_ACK;
			break;

		
		case TW_SR_GCALL_DATA_ACK://0x90 general call data received, ACK returned
		case TW_SR_DATA_ACK://0x80 data received, ACK returned
			tmp = TWDR;
			if(twi_msg_ptrRX < sizeof(blue_twi_message)+1)
			{
				if(twi_msg_ptrRX < sizeof(blue_twi_message))
				{
					check_sum += tmp;
					twi_tmp_msgRX[twi_msg_ptrRX] = tmp;
				}
				twi_msg_ptrRX++;

				if(twi_msg_ptrRX == sizeof(blue_twi_message)+1)
				{
					#ifdef TWI_MEASURE
					recv_tick++;
					#endif
					//TODO: Check for listeners for maskes
					if(tmp == check_sum)
						blue_os_eventSignal(&blue_os_twiEvent, (uint8_t*)twi_tmp_msgRX);
					
					twi_listening = blue_os_eventGetListenerCnt(&blue_os_twiEvent, 0);
					//Ending sequence must be done by polling
					
					twi_last_action = TWI_SLAVE_RECV;
					#ifdef BLUE_OS_TWI_MASTER
						blueOsSignalSingle(&twi_sent);	
					#endif //BLUE_OS_TWI_MASTER	
					for(uint8_t i=0;i<2;i++)
					{
						TWCR |= (1<<TWEN)|(1<<TWEA)|(1<<TWINT);
						while (!(TWCR & (1<<TWINT)));
						if(i==0)
						{
							continue;
						}
						if(twi_listening)
						{
							TWCR |= (1<<TWEN)|(1<<TWINT)|(1<<TWEA)|(1<<TWIE);
						}
						else
						{
							TWCR &= ~(1<<TWEA);
							TWCR |= (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
						}
					}		
					break;
				}
				TWCR |= (1<<TWEA)|(1<<TWINT);
			}
			else
			{
				//should not happen
				TWCR &= ~(1<<TWEA);
				TWCR |= (1<<TWINT);
			}
			break;
			

		case TW_SR_STOP://0xA0 stop or repeated start condition received while selected		
			//should not happen
			twi_msg_ptrRX = 0;
			check_sum = 0;
			TWCR |= (1<<TWINT)| ack_bit;	
			break;
	#endif //BLUE_OS_TWI_SLAVE	
//ERROR

		case TW_NO_INFO://0xF8 no state information available
			break;
//		case TW_SR_GCALL_DATA_NACK://0x98 general call data received, NACK returned
//		case TW_SR_DATA_NACK://0x88 data received, NACK returned
//		case TW_BUS_ERROR://0x00 illegal start or stop condition
		default:
			twi_state = TWI_READY;
			twi_last_action = TWI_ERROR;

			#ifdef BLUE_OS_TWI_MASTER
				blueOsSignalSingle(&twi_sent);	
			#endif //BLUE_OS_TWI_MASTER	

			twi_msg_ptrTX = 0;
			twi_msg_ptrRX = 0;

			#ifdef BLUE_OS_TWI_SLAVE
			if(twi_listening)
				TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWIE)|_BV(TWEA)|_BV(TWSTO);
			else
			#endif
				TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWIE)|_BV(TWSTO);
			break;
	}
}

#endif
