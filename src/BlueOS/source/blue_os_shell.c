#include "blue_os_shell.h"

#include "blue_os_container.h"

#ifdef BLUE_OS_USE_SHELL

#include "blue_os_internals.h"
#include "blue_os_interrupt.h"

#include <string.h>

#ifdef BLUE_OS_USE_SHELL_VT100
#include "blue_os_shell_vt100.h"
#endif


#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16L)-1L)

#define UART_UBRR_VALUE UART_BAUD_SELECT(BLUE_OS_USE_SHELL_BAUD_RATE,F_CPU)


static volatile uint8_t blueOsShellRxBuffer[BLUE_OS_SHELL_RXBUFFER];
volatile BlueOsQueue blueOsShellRxQueue;

static volatile uint8_t blueOsShellTxBuffer[BLUE_OS_SHELL_TXBUFFER];
volatile BlueOsQueue blueOsShellTxQueue;


void blueOsInitShell()
{
	blueOsQueueInit(&blueOsShellRxQueue, blueOsShellRxBuffer, BLUE_OS_SHELL_RXBUFFER);
	blueOsQueueInit(&blueOsShellTxQueue, blueOsShellTxBuffer, BLUE_OS_SHELL_TXBUFFER);
	
	#ifdef BLUE_OS_USE_SHELL_VT100
		#ifdef BLUE_OS_SHELL_VT100_LISTENER		
			blueOsQueueAddCallBack(&blueOsShellRxQueue, &blueOsVT100CallBack );
		#endif
	#endif

	UART_CONFIGURATION;
	
	#ifdef BLUE_OS_USE_SEMA
	blueOsInitSema(&blueOsShellSema);	
	#endif

}

void blueOsShellWrite(const uint8_t* buffer, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		blueOsShellWriteChar(buffer[i]);
	}
}

void blueOsShellWriteChar(char c)
{
	int8_t retval = 0;
	while(1)
	{
		uint8_t irstate = blueOsEnterCriticalSection();

		retval = blueOsQueueEnqueue(&blueOsShellTxQueue, c);

		
		if(retval < 0)
		{

			while(!UART_UDRE)
						;
	
			blueOsShellUDRE();
			blueOsLeaveCriticalSection(irstate);
			continue;

		}
		
		blueOsLeaveCriticalSection(irstate);
		break;
	}
	
	
	// Enable interrupt
	UART_UDRIE_REGISTER |= (1 << UART_UDRIE_FLAG);


}

void blueOsWriteString(const char* string)
{
	blueOsShellWrite((uint8_t*)string, strlen(string));
}

void blueOsWriteStringFlash_(const char *s )
{
	char c;
	
	while ( (c = pgm_read_byte(s++)) ) 
		blueOsShellWriteChar(c);
}


static uint8_t shortToString(int16_t value, uint8_t leading, char* string)
{
	// This function returns the number in reverse order!
	
	char sign;
	if (value < 0)
	{
		sign = '-';
		value = -value;
	}
	else
		sign = ' ';

	uint8_t i;
	for (i = 0; i < 5; i++)
	{
		if ((value == 0) && (i >= leading))
			break;
		
		string[i] = '0' + (uint8_t)(value % 10);

		value = value / 10;
	}

	string[i] = sign;
	return i;
}

void blueOsWriteInt(int value, char leading)
{
	char tempString[6];
	uint8_t i;

	uint8_t irstate = blueOsEnterCriticalSection();

	uint8_t stringlength = shortToString(value, leading, tempString);

	blueOsLeaveCriticalSection(irstate);

	for (i = 0; i <= stringlength; i++)
		blueOsShellWriteChar(tempString[stringlength - i]);
	
}

static uint8_t longToString(int32_t value, uint8_t leading, char* string)
{
	// This function returns the number in reverse order!
	
	char sign;
	if (value < 0L)
	{
		sign = '-';
		value = -value;
	}
	else
		sign = ' ';

	uint8_t i;
	for (i = 0; i < 10; i++)
	{
		if ((value == 0L) && (i >= leading))
			break;
		
		string[i] = '0' + (uint8_t)((uint32_t)value % 10UL);

		value = value / 10UL;
	}

	string[i] = sign;
	return i;
}

void blueOsWriteLong(long value, char leading)
{
	char tempString[11];
	uint8_t i;

	uint8_t irstate = blueOsEnterCriticalSection();

	uint8_t stringlength = longToString(value, leading, tempString);

	blueOsLeaveCriticalSection(irstate);

	for (i = 0; i <= stringlength; i++)
		blueOsShellWriteChar(tempString[stringlength - i]);
}

uint8_t blueOsShellRead(uint8_t* buffer, uint8_t length)
{
	uint8_t i = 0;
	uint8_t data;
	for (i = 0; i < length; i++)
	{
		if (blueOsQueueDequeue(&blueOsShellRxQueue, &data) >= 0)
		{
			buffer[i] = data;
		}
		else
		{
			break;
		}
	}
	return i;
}


void blueOsShellUDRE()
{
	uint8_t data;
	if (blueOsQueueDequeue(&blueOsShellTxQueue, &data) >= 0)
	{
		UART_UDR = data;
	}
	else
	{
		// Disable interrupt

		UART_UDRIE_REGISTER &= ~(1 << UART_UDRIE_FLAG);
	}
}

static inline void blueOsShellRXC()
{
	uint8_t data;
	uint8_t error;
	uint8_t tmp;

	data = UART_UDR;
	error = UART_FE + UART_UPE;


	if (!error)
	{
		while(blueOsQueueEnqueue(&blueOsShellRxQueue, data) == -1)
		{
			blueOsQueueDequeue(&blueOsShellRxQueue, &tmp);
		}
	}

}


BLUE_OS_ISR(INTERRUPT_UART_UDRE, blueOsShellUDRE);
BLUE_OS_ISR(INTERRUPT_UART_RXC, blueOsShellRXC);


#endif //BLUE_OS_USE_SHELL
