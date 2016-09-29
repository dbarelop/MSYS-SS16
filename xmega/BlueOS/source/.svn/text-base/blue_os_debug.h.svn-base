#ifndef BLUE_OS_DEBUG_H
#define BLUE_OS_DEBUG_H

#include "blue_os.h"
#include "blue_os_config.h"

// Defines
#define BLUE_OS_PANIC_STACKPOINTER_OUTSIDE	1
#define BLUE_OS_PANIC_STACK_DAMAGED			2
#define BLUE_OS_PANIC_IDLE_TASK_RETURNED 	3

// Functions
#ifdef BLUE_OS_DEBUG
	
	
	#define blueOsAssert(e, r)	((e)?(void)0:blueOsKernelPanic(r))
#else
	#define blueOsAssert(e, r)
#endif //BLUE_OS_DEBUG

/**
 * This function is the kernel panic function.
 * It will halt the system for debugging purposes.
 *
 * @warning This function will not return.
 * @warning This function disables the interrupts
 *
 * @param errorValue ID of the error, that caused the kernel panic.
 */
void blueOsKernelPanic(volatile uint8_t errorValue);

#ifdef BLUE_OS_IMPLEMENT_PANIC_CALLBACK
/**
 * If BLUE_OS_IMPLEMENT_PANIC_CALLBACK is defined, this function
 * will be called in the panic procedure.
 *
 * @warning This function must be implemented by the user, if BLUE_OS_IMPLEMENT_PANIC_CALLBACK is defined!
 *
 * @param errorValue ID of the error, that caused the kernel panic.
 */
void blueOsKernelPanicHook(volatile uint8_t errorValue);
#endif //BLUE_OS_IMPLEMENT_PANIC_CALLBACK

#endif
