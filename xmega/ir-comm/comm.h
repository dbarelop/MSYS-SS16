#ifndef COMM_H
#define COMM_H

#include "../BlueOS/source/blue_os.h"
#include <avr/io.h>

#define XMEGA_SELF2
#define SHARED_VARIABLES 10

typedef struct
{
	uint8_t size;
	uint16_t addr;
	uint32_t data;	
} Comm_Frame;




void commInit();
void commTask();
void commGetData(USART_t* uart, uint16_t addr, uint8_t* data);
void commSetData(USART_t* uart, uint16_t addr, uint8_t* data, uint8_t len);
void commShareVariable(uint16_t addr, uint8_t* var);
#ifdef RT_DEBUG
void commSendRealtime(uint8_t *data);
#endif

BlueOsEvent commEvent;

#ifdef XMEGA_SELF1

#ifdef RT_DEBUG
	#error Xmega 1 is not intended to send realtime data
#endif

#define X0_UART USARTD0
#define X1_UART USARTD1

#define X0_UART_RXVECT 	USARTD0_RXC_vect
#define X0_UART_TXVECT	USARTD0_DRE_vect

#define X1_UART_RXVECT 	USARTD1_RXC_vect
#define X1_UART_TXVECT	USARTD1_DRE_vect

#define COMM_PIN_CONFIG 	PORTD.DIRSET   = PIN3_bm;\
					PORTD.DIRCLR   = PIN2_bm;\
					PORTD.DIRSET   = PIN7_bm;\
					PORTD.DIRCLR   = PIN6_bm;	
#endif

#ifdef XMEGA_SELF2

#define X0_UART USARTD0
#define X1_UART USARTF1

#define X0_UART_RXVECT 	USARTD0_RXC_vect
#define X0_UART_TXVECT	USARTD0_DRE_vect

#define X1_UART_RXVECT 	USARTF1_RXC_vect
#define X1_UART_TXVECT	USARTF1_DRE_vect

#define COMM_PIN_CONFIG 	PORTD.DIRSET   = PIN3_bm;\
					PORTD.DIRCLR   = PIN2_bm;\
					PORTF.DIRSET   = PIN7_bm;\
					PORTF.DIRCLR   = PIN6_bm;	
#endif

#ifdef XMEGA_SELF3

#define X0_UART USARTD1
#define X1_UART USARTF1

#define X0_UART_RXVECT 	USARTD1_RXC_vect
#define X0_UART_TXVECT	USARTD1_DRE_vect

#define X1_UART_RXVECT 	USARTF1_RXC_vect
#define X1_UART_TXVECT	USARTF1_DRE_vect

#define COMM_PIN_CONFIG 	PORTD.DIRSET   = PIN7_bm;\
					PORTD.DIRCLR   = PIN6_bm;\
					PORTF.DIRSET   = PIN7_bm;\
					PORTF.DIRCLR   = PIN6_bm;	
#endif





#endif
