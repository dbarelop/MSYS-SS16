#include "comm.h"
#include "../BlueOS/source/blue_os.h"
#include <avr/interrupt.h>

uint8_t* comm_addr_map[SHARED_VARIABLES];


typedef struct
{
	Comm_Frame rx;
	Comm_Frame tx;
	
	uint8_t ind_rx;
	uint8_t ind_tx;	

	USART_t* uart;
}XmegConn;

XmegConn con1;
XmegConn con2;


#define COMM_BAUD 107
#define COMM_SCALE 0b1011

void commISR_TX(XmegConn* con);
void commISR_RX(XmegConn* con);

#ifndef RT_DEBUG
ISR(X0_UART_RXVECT)
{
	cli();
	commISR_RX(&con1);
	sei();
}
#endif

ISR(X1_UART_RXVECT)
{
	cli();
	commISR_RX(&con2);
	sei();
}

#ifndef RT_DEBUG
ISR(X0_UART_TXVECT)
{
	cli();
	commISR_TX(&con1);
	sei();
}
#endif

ISR(X1_UART_TXVECT)
{
	cli();
	commISR_TX(&con2);
	sei();
}

void commInit()
{
	uint16_t i;
	
	con1.uart = &X0_UART;
	con1.ind_rx = 0;
	con1.ind_tx = 0;
	con1.rx.size = 0;
	con1.rx.addr = 0;
	con1.rx.data = 0;

	con2.uart = &X1_UART;
	con2.ind_rx = 0;
	con2.ind_tx = 0;
	con2.rx.size = 0;
	con2.rx.addr = 0;
	con2.rx.data = 0;

	for(i=0; i<SHARED_VARIABLES; i++)
	{
		comm_addr_map[i] = 0;
	}
	
	COMM_PIN_CONFIG


	X0_UART.CTRLC = 3;
    X0_UART.BAUDCTRLA =(uint8_t)COMM_BAUD; 
	X0_UART.BAUDCTRLB =(COMM_SCALE << USART_BSCALE0_bp)|(COMM_BAUD >> 8);
	X0_UART.CTRLB |= USART_TXEN_bm;
	X0_UART.CTRLB |= USART_RXEN_bm;
#ifndef RT_DEBUG
	X0_UART.CTRLA |= (3<<USART_RXCINTLVL_gp);// | (3<<USART_DREINTLVL_gp);
#endif

	X1_UART.CTRLC = 3;
    X1_UART.BAUDCTRLA =(uint8_t)COMM_BAUD; 
	X1_UART.BAUDCTRLB =(COMM_SCALE << USART_BSCALE0_bp)|(COMM_BAUD >> 8);
	X1_UART.CTRLB |= USART_TXEN_bm;
	X1_UART.CTRLB |= USART_RXEN_bm;
	X1_UART.CTRLA |= (3<<USART_RXCINTLVL_gp);// | (3<<USART_DREINTLVL_gp);

	blue_os_eventInit(&commEvent, 2, 0);
}

void commTask()
{
	while(1)
	{

	}

}
void commISR_TX(XmegConn* con)
{
	uint8_t data_tmp = 0;

	uint8_t tmp, tmp2;

	data_tmp = (con->ind_tx << 4);
	tmp = con->ind_tx / 2;
	tmp2 = ((uint8_t*)(&(con->tx)))[tmp];

	if(con->ind_tx % 2)
	{
		tmp2 = (tmp2 & 0xF0) >> 4;
	}
	else
	{
		tmp2 = tmp2 & 0x0F;
	}
	data_tmp |= tmp2;
	
	con->uart->DATA = data_tmp;
	con->ind_tx++;
	if(con->ind_tx >= 14)
	{
		con->ind_tx = 0;
		con->uart->CTRLA &= ~(3<<USART_DREINTLVL_gp);	

	}	
}

void commISR_RX(XmegConn* con)
{
	uint8_t data_tmp = con->uart->DATA;

	uint8_t tmp;
	uint8_t tmp2;

	
	if(con->ind_rx == ((data_tmp & 0xF0)>>4))
	{
		uint16_t i;
		
		tmp = con->ind_rx / 2;
		tmp2 = data_tmp & 0xf;

		if(con->ind_rx % 2)
		{
			tmp2 = tmp2 << 4;
		}
		((uint8_t*)(&con->rx))[tmp] |= tmp2;

		con->ind_rx++;
		if(con->ind_rx >= 14)
		{

			if(comm_addr_map[con->rx.addr] !=0)
			{
				for(i=0;i<con->rx.size;i++)
				{
					((uint8_t*)(comm_addr_map[con->rx.addr]))[i] = ((uint8_t*)(&con->rx.data))[i];
				}
			}	
			
			blue_os_eventSignal(&commEvent, (uint8_t*)&(con->rx.addr));
			
			con->ind_rx = 0;
			con->rx.size = 0;
			con->rx.addr = 0;
			con->rx.data = 0;
		}		
	}
	else
	{
		con->ind_rx = 0;
		con->rx.size = 0;
		con->rx.addr = 0;
		con->rx.data = 0;
	}

}

void commGetData(USART_t* uart, uint16_t addr, uint8_t* data)
{


}

void commSetData(USART_t* uart, uint16_t addr, uint8_t* data, uint8_t len)
{
	XmegConn *tmp;
	uint8_t i;

	if(uart == &X0_UART)
	 	tmp = &con1;
	else
		tmp = &con2;

	while(tmp->uart->CTRLA & (3<<USART_DREINTLVL_gp))
	{
		
	}

	tmp->tx.addr = addr;
	tmp->tx.size = len;
	for(i=0; i<len; i++)
	{
		((uint8_t*)(&(tmp->tx.data)))[i] = data[i];
	}

	tmp->uart->CTRLA |= (3<<USART_DREINTLVL_gp);
}


void commShareVariable(uint16_t addr, uint8_t* var)
{
	if(addr < SHARED_VARIABLES)
		comm_addr_map[addr] = var;
}


#ifdef RT_DEBUG
void commSendRealtime(uint8_t *data)
{
	uint8_t tmp = *data;
	return;
	while((con1.uart->STATUS & USART_DREIF_bm)==0) //any queue solution would delay this important message
		;
	con1.uart->DATA = tmp;
}
#endif