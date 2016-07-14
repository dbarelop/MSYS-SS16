#ifndef BLUE_HW_ATMEG128_H
#define BLUE_HW_ATMEG128_H

	#define TIMER_PRESCALER 64UL
	#define TIMER_TICKS_MS (F_CPU / (TIMER_PRESCALER*1000UL))
	#define TIMER_COUNTER_INIT_VALUE (256UL - TIMER_TICKS_MS)
	#define TIMER_US_FACTOR ((TIMER_PRESCALER * 1000000UL) / F_CPU)

	#define TIMER_CONGFIGURATION			TCCR0 = (1 << CS02);
	#define TIMER_MASK_REGISTER				TIMSK
	#define TIMER_INTERRUPT_MASK			TOIE0
	#define BLUE_OS_INTERRUPT				TIMER0_OVF_vect
	#define TIMER_COUNTER					TCNT0
	#define TIMER_INTERRUPT_FLAG_REGISTER 	TIFR
	#define TIMER_INTERRUPT_FLAG			TOV0

	// Shell
	#if BLUE_OS_USE_SHELL_UART > 2
		#error This UART does not exist!
	#endif

	#define __UART_CONFIGURATION(NR)		UBRR##NR##H = (unsigned char)(UART_UBRR_VALUE>>8); \
    										UBRR##NR##L = (unsigned char) UART_UBRR_VALUE; \
											UCSR##NR##B = _BV(RXCIE##NR)|(1<<RXEN##NR)|(1<<TXEN##NR); \
											UCSR##NR##C = (3<<UCSZ##NR##0);
	
	#define _UART_CONFIGURATION(NR) __UART_CONFIGURATION(NR)
	#define UART_CONFIGURATION _UART_CONFIGURATION(BLUE_OS_USE_SHELL_UART)
	
	// UART
	#define __UART_UDR(NR)					UDR##NR
	#define _UART_UDR(NR) __UART_UDR(NR)
	#define UART_UDR _UART_UDR(BLUE_OS_USE_SHELL_UART)
	
	#define __UART_FE(NR)					(UCSR##NR##A & (1<<FE##NR))  // Framing error
	#define _UART_FE(NR) __UART_FE(NR)
	#define UART_FE _UART_FE(BLUE_OS_USE_SHELL_UART)

	#define __UART_DOR(NR)					(UCSR##NR##A & (1<<DOR##NR)) // Data overrun
	#define _UART_DOR(NR) __UART_DOR(NR)
	#define UART_DOR _UART_DOR(BLUE_OS_USE_SHELL_UART)
	
	#define __UART_UPE(NR)					(UCSR##NR##A & (1<<UPE##NR)) // Parity error
	#define _UART_UPE(NR) __UART_UPE(NR)
	#define UART_UPE _UART_UPE(BLUE_OS_USE_SHELL_UART)			


	#define __UART_UDRIE_REGISTER(NR)		UCSR##NR##B
	#define _UART_UDRIE_REGISTER(NR) __UART_UDRIE_REGISTER(NR)
	#define UART_UDRIE_REGISTER _UART_UDRIE_REGISTER(BLUE_OS_USE_SHELL_UART)
	#define UART_TXCIE_REGISTER			UART_UDRIE_REGISTER
	#define UART_RXCIE_REGISTER			UART_UDRIE_REGISTER
	#define UART_UDRIE_FLAG				UDRIE0
	#define UART_TXCIE_FLAG				TXCIE0
	#define UART_RXCIE_FLAG				RXCIE0

	#define __UART_UDRE(NR)				(UCSR##NR##A & (1<<UDRE##NR))
	#define _UART_UDRE(NR) __UART_UDRE(NR)
	#define UART_UDRE _UART_UDRE(BLUE_OS_USE_SHELL_UART)


	// Interrupt definitions
	#define __INTERRUPT_UART_RXC(NR)		USART##NR##_RX_vect
	#define _INTERRUPT_UART_RXC(NR) __INTERRUPT_UART_RXC(NR)
	#define INTERRUPT_UART_RXC _INTERRUPT_UART_RXC(BLUE_OS_USE_SHELL_UART)

	#define __INTERRUPT_UART_TXC(NR)		USART##NR##_TX_vect
	#define _INTERRUPT_UART_TXC(NR) __INTERRUPT_UART_TXC(NR)
	#define INTERRUPT_UART_TXC _INTERRUPT_UART_TXC(BLUE_OS_USE_SHELL_UART)
	
	#define __INTERRUPT_UART_UDRE(NR)		USART##NR##_UDRE_vect
	#define _INTERRUPT_UART_UDRE(NR) __INTERRUPT_UART_UDRE(NR)
	#define INTERRUPT_UART_UDRE _INTERRUPT_UART_UDRE(BLUE_OS_USE_SHELL_UART)

	// TWI definitions
	#define TWI_SPEC
	#define TWI_DDR_SCL DDRD
	#define TWI_DDR_SDL DDRD
	#define TWI_PORT_SCL PORTD
	#define TWI_PORT_SDL PORTD
	#define TWI_PIN_SCL PIND
	#define TWI_PIN_SDL PIND
	#define TWI_SCL 0
	#define TWI_SDL 1

	#define TIMER_INTERRUPT_ENABLE			TIMER_MASK_REGISTER |= (1 << TIMER_INTERRUPT_MASK);
	#define TIMER_INTERRUPT_DISABLE			TIMER_MASK_REGISTER &= ~(1 << TIMER_INTERRUPT_MASK);

#endif
