#ifndef BLUE_OS_SHELL_VT100_H
#define BLUE_OS_SHELL_VT100_H

#include "blue_os.h"

#include <avr/io.h>

#ifdef BLUE_OS_USE_SHELL_VT100

#ifndef BLUE_OS_USE_SHELL
#error "BLUE_OS_USE_SHELL not defined, but needed for VT100"
#endif //BLUE_OS_USE_SHELL

#ifdef BLUE_OS_SHELL_VT100_LISTENER
BlueOsEvent blue_os_vt100Event;
#endif //BLUE_OS_SHELL_VT100_LISTENER

/**
 * KeyCodes
 */
#define TERM_CURSOR_UP 		256
#define TERM_CURSOR_DOWN 	257
#define TERM_CURSOR_LEFT 	258
#define TERM_CURSOR_RIGHT 	259
#define TERM_POS1			260
#define TERM_ENDE			261
#define TERM_F1				262
#define TERM_F2				263
#define TERM_F3				264
#define TERM_F4				265



/**
 * This function initializes the VT100 terminal emulation,
 * and cleares the screen
 */
void blueOsInitShellVt100();


/**
 * This function cleares the screen of the VT100 terminal.
 */
void blueOsClearScreen();


/**
 * This function draws a frame
 */
void blueOsDrawMainFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h);


/**
 * This function draws a horizontal line
 */
void blueOsDrawHorizLine(uint8_t x, uint8_t y, uint8_t length);


/**
 * This function draws a vertical line
 */
void blueOsDrawVertLine(uint8_t x, uint8_t y, uint8_t length);


/**
 * This function changes the VT100 to "Graphic Mode" and vice versa
 */
void blueOsEnterGraphic();
void blueOsLeaveGraphic();


/**
 * This function defines the rolling area
 * For reset, type: BlueOsSetRollingArea(0,0)
 */
void blueOsSetRollingArea(uint8_t y1, uint8_t y2);


void blueOsSetInvers(uint8_t state);

void blueOsSetPosition(uint8_t row, uint8_t col);

/**
 * This function parses the Inputstream
 * It is a callback function for a queue
 */

void blueOsVT100Parser(uint8_t* source, uint8_t digit);

extern BlueOsCallBack blueOsVT100CallBack;

#endif //BLUE_OS_USE_SHELL_VT100

#endif //BLUE_OS_SHELL_VT100_H
