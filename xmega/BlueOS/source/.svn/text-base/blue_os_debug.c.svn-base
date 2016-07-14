#include "blue_os_debug.h"
#include "blue_os_internals.h"

#ifdef BLUE_OS_DEBUG_SHELL
	#include "blue_os_shell.h"
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef BLUE_OS_DEBUG_SHELL
	#ifndef BLUE_OS_USE_SHELL
		#error "BLUE_OS_USE_SHELL is not defined, but needed for debug shell"
	#endif // BLUE_OS_USE_SHELL
#endif // BLUE_OS_DEBUG_SHELL

// <private variables>
static volatile uint8_t _kernelPanicError;
// </private variables>

void USART_UDRE_vect();

// <public functions>
void blueOsKernelPanic(volatile uint8_t errorValue)
{
	// Freeze the system
	cli();

	// Disable the timer interrupt
	TIMER_INTERRUPT_DISABLE;

	_kernelPanicError = errorValue;

	#ifdef BLUE_OS_DEBUG_SHELL
	uint16_t sp_old = SP;
	#endif //BLUE_OS_DEBUG_SHELL

	SP = RAMEND;

	#ifdef BLUE_OS_IMPLEMENT_PANIC_CALLBACK
	blueOsKernelPanicHook(errorValue);
	#endif //BLUE_OS_IMPLEMENT_PANIC_CALLBACK


	// Reinit stack pointer
	#ifdef BLUE_OS_DEBUG_SHELL
	uint8_t i;
	for (i = 0; i < 32;i++)
	{

			while(!UART_UDRE)
				;



		blueOsShellUDRE();
	}

	blueOsInitShell();


	blueOsWriteString("\r\n\r\n\r\n");

	blueOsWriteStringFlash("Kernel Panic\r\n\r\nCaused by task: ");
	blueOsWriteInt(_currentTaskId, 3);
	blueOsWriteStringFlash("\r\nError: ");
	blueOsWriteInt(errorValue, 5);
	blueOsWriteStringFlash("\r\nStack pointer: ");
	blueOsWriteInt(sp_old, 5);
	blueOsWriteStringFlash("\r\nStack end: ");
	blueOsWriteInt((int)_currentTask->_stack, 5);
	blueOsWriteStringFlash("\r\nStack base: ");
	blueOsWriteInt(((_currentTask->_stackSize) + ((int)_currentTask->_stack)), 5);
	#endif //BLUE_OS_DEBUG_SHELL


	while(1)
	{
		#ifdef BLUE_OS_DEBUG_SHELL
		// Send everything of the shell buffer

			while(!UART_UDRE)
				;


		blueOsShellUDRE();
		#endif //BLUE_OS_DEBUG_SHELL
	}
}

// </public functions>



