#ifndef BLUE_HW_ATMEG8_H
#define BLUE_HW_ATMEG8_H

	#define TIMER_PRESCALER 64UL
	#define TIMER_TICKS_MS (F_CPU / (TIMER_PRESCALER*1000UL))
	#define TIMER_COUNTER_INIT_VALUE (256UL - TIMER_TICKS_MS)
	#define TIMER_US_FACTOR ((TIMER_PRESCALER * 1000000UL) / F_CPU)

	#define TIMER_CONGFIGURATION			TCCR0 = (1 << CS00) | (1 << CS01);
	#define TIMER_MASK_REGISTER				TIMSK
	#define TIMER_INTERRUPT_MASK			TOIE0
	#define BLUE_OS_INTERRUPT				TIMER0_OVF_vect
	#define TIMER_COUNTER					TCNT0
	#define TIMER_INTERRUPT_FLAG_REGISTER 	TIFR
	#define TIMER_INTERRUPT_FLAG			TOV0

	// Shell
	#define UART_CONFIGURATION				UBRRH = (unsigned char)(UART_UBRR_VALUE>>8); \
    										UBRRL = (unsigned char) UART_UBRR_VALUE; \
											UCSRB = _BV(RXCIE)|(1<<RXEN)|(1<<TXEN); \
											UCSRC = (1<<URSEL)|(3<<UCSZ0);

	// UART

	#define UART_UDR						UDR
	#define UART_FE							(UCSRA & (1<<FE))  // Framing error
	#define UART_DOR						(UCSRA & (1<<DOR)) // Data overrun
	#define UART_UPE						(UCSRA & (1<<PE)) // Parity error

	#define UART_UDRIE_REGISTER				UCSRB
	#define UART_TXCIE_REGISTER				UCSRB
	#define UART_RXCIE_REGISTER				UCSRB
	#define UART_UDRIE_FLAG					UDRIE
	#define UART_TXCIE_FLAG					TXCIE
	#define UART_RXCIE_FLAG					RXCIE

	#define UART_UDRE						(UCSRA & (1<<UDRE))

	// Interrupt definitions
	#define INTERRUPT_UART_RXC				USART_RXC_vect
	#define INTERRUPT_UART_TXC				USART_TXC_vect
	#define INTERRUPT_UART_UDRE				USART_UDRE_vect

	//TWI definition
	#define TWI_SPEC
	#define TWI_DDR_SCL DDRC
	#define TWI_DDR_SDL DDRC
	#define TWI_PORT_SCL PORTC
	#define TWI_PORT_SDL PORTC
	#define TWI_PIN_SCL PINC
	#define TWI_PIN_SDL PINC
	#define TWI_SCL 0
	#define TWI_SDL 1

	#define TIMER_INTERRUPT_ENABLE			TIMER_MASK_REGISTER |= (1 << TIMER_INTERRUPT_MASK);
	#define TIMER_INTERRUPT_DISABLE			TIMER_MASK_REGISTER &= ~(1 << TIMER_INTERRUPT_MASK);

#endif
