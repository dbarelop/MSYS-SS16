#ifndef BLUE_OS_INTERNALS_H
#define BLUE_OS_INTERNALS_H

#include "blue_os_config.h"
#include "blue_os.h"
#include "blue_os_task.h"

// Defines

#define TIMER_TIMEOUT_INACTIVE 0xFFFFFFFF
// Protected data

extern volatile int8_t _currentTaskId;

extern volatile BlueOsTCB* _currentTask;

extern volatile BlueOsTCB* _tasks[BLUE_OS_TASK_COUNT];

extern volatile BlueOsTCB _idleTask;

extern volatile uint32_t _milliseconds;

extern volatile uint32_t _taskDelays[BLUE_OS_TASK_COUNT];

#ifdef BLUE_OS_USE_USER_WATCHDOG
extern volatile blueOsWatchdogCallbackFunction _userWatchdogCallbacks[BLUE_OS_TASK_COUNT];

extern volatile uint32_t _userWatchdogTimeouts[BLUE_OS_TASK_COUNT];

extern volatile uint32_t _nextUserWatchdog;
#endif //BLUE_OS_USE_USER_WATCHDOG

// Protected functions
/**
 * This function does a software time interrupt.
 * The milliseconds are not incremented, but the sheduler is called.
 */
extern void blueOsSoftTimerInterrupt();

#ifdef BLUE_OS_USE_SHELL
/**
 * This function initializes the shell, don't use this function,
 * it is already done by the OS.
 */
void blueOsInitShell();

/**
 * This function writes a character of the sendbuffer to the shell.
 */
void blueOsShellUDRE();
#endif //BLUE_OS_USE_SHELL

#ifdef BLUE_OS_USE_USER_WATCHDOG
/**
 * This function selects the next watchdog timer, and
 * calls every callback function, which timed out.
 */
void blueOsUserWatchdogSchedule() __attribute__((always_inline));
#endif //BLUE_OS_USE_USER_WATCHDOG

#endif
