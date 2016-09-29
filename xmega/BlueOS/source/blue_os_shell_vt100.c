#include "blue_os_shell_vt100.h"
#include "blue_os_shell.h"


#ifdef BLUE_OS_USE_SHELL_VT100

#include <avr/io.h>
#include <avr/interrupt.h>

void testttt()
{

}

#ifdef BLUE_OS_SHELL_VT100_LISTENER
BlueOsEvent blue_os_vt100Event;
BlueOsCallBack blueOsVT100CallBack = {(BlueOsCallBack*)0, blueOsVT100Parser};
#endif //BLUE_OS_SHELL_VT100_LISTENER


void uartPutDecimal( volatile uint8_t value )
{
    volatile unsigned char digit;
	uint8_t str[3];
    
    digit = '0';
    while( value >= 100 )                // Still larger than 100 ?
    {
        digit++;                         // Increment first digit
        value -= 100;
    }
    
    str[0] = digit;
    
    digit = '0';
    while( value >= 10 )                 // Still larger than 10 ?
    {
        digit++;                         // Increment second digit
        value -= 10;
    }
    
    str[1] = digit;                  // Send second digit    
   	str[2] = '0' + value ;            // Send third digit
	blueOsShellWrite(str, 3);
}

void blueOsInitShellVt100()
{
	const static char buffer[] PROGMEM =
	{
		// Select VT100 mode
		27,
		'<',

		// Select 132 columns
		27,
		'[',
		'?',
		'3',
		'h',

		// Set the cursor invisible
		27,
		'[',
		'?',
		'2',
		'5',
		'l',
		0
	};

	blueOsWriteStringFlash_(buffer);
	blueOsClearScreen();

	#ifdef BLUE_OS_SHELL_VT100_LISTENER
	blue_os_eventInit(&blue_os_vt100Event, 2,0);
	#endif //BLUE_OS_SHELL_VT100_LISTENER
}

void blueOsClearScreen()
{
	const static char buffer[] PROGMEM =
	{
		27,
		'[',
		'2',
		'J',
		27,
   		'[',
		'f',
		0
	};
	
	blueOsWriteStringFlash_(buffer);
}

void blueOsSetPosition(uint8_t row, uint8_t col)
{
	uint8_t irstate = blueOsEnterCriticalSection();

	const static char buffer[] PROGMEM =
	{
		27,
		'[',
		0
	};

	blueOsWriteStringFlash_(buffer);
	uartPutDecimal(row);
	blueOsShellWriteChar(';');
	uartPutDecimal(col);
	blueOsShellWriteChar('H');

	
	blueOsLeaveCriticalSection(irstate);
}

/**
 * This function draws a frame
 */
void blueOsDrawMainFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	//lines
	blueOsDrawHorizLine(x+1,y,w-2);
	blueOsDrawHorizLine(x+1,y+h,w-2);
	blueOsDrawVertLine(x,y+1,h);
	blueOsDrawVertLine(x+w,y+1,h-2);

	//corners
	//upper left
	blueOsSetPosition(y, x);
	blueOsShellWriteChar('l');
	//upper right
	blueOsSetPosition(y, x+w);
	blueOsShellWriteChar('k');
	//lower left
	blueOsSetPosition(y+h, x);
	blueOsShellWriteChar('m');
	//lower right
	blueOsSetPosition(y+h, x+w);
	blueOsShellWriteChar('j');
}


/**
 * This function draws a horizontal line
 */
void blueOsDrawHorizLine(uint8_t x, uint8_t y, uint8_t length)
{
	uint8_t i;

	blueOsSetPosition(y, x);
	for(i=x;i<=(x+length);i++)
	{
		blueOsShellWriteChar('q');
	}
}


/**
 * This function draws a vertical line
 */
void blueOsDrawVertLine(uint8_t x, uint8_t y, uint8_t length)
{
	uint8_t i;
	for(i=y;i<=(y+length);i++)
	{
		blueOsSetPosition(i, x);
		blueOsShellWriteChar('x');
	}
}


/**
 * This function changes the VT100 to "Graphic Mode" and vice versa
 */
void blueOsEnterGraphic()
{
	const static char buffer[] PROGMEM =
	{
		27,
		'(',
		'0',
		0
	};
	cli();	
	blueOsWriteStringFlash_(buffer);
	sei();
}
void blueOsLeaveGraphic()
{
	const static char buffer[] PROGMEM =
	{
		27,
		'(',
		'B',
		0
	};	
	cli();
	blueOsWriteStringFlash_(buffer);
	sei();
}


/**
 * This function defines the rolling area
 */
void blueOsSetRollingArea(uint8_t y1, uint8_t y2)
{
 	const static char buffer[] PROGMEM =
	{
		27,
		'[',
		0
	};

	blueOsWriteStringFlash_(buffer);
	if(y1!=y2)
	{
		uartPutDecimal(y1);
		blueOsShellWriteChar(';');
		uartPutDecimal(y2);
	}
	blueOsShellWriteChar('r');
}

void blueOsSetInvers(uint8_t state)
{
	const static char buffer[] PROGMEM =
	{
		27,
		'[',
		0
	};

	cli();
	blueOsWriteStringFlash_(buffer);
	if(state)
	{
		uartPutDecimal(7);
	}
	else
	{
		uartPutDecimal(0);
	}
	blueOsShellWriteChar('m');
	sei();
}

/**
 * This function parses the Inputstream
 */
void blueOsVT100Parser(uint8_t* source, uint8_t digit)
{
	static uint8_t esc_mode = 0;
	
	int16_t val = -1;
	if(digit == 27)
	{
		esc_mode = 1;
		return;
	}

	if(digit == '\r')
	{
		esc_mode = 0;
		return;
	}

	if(esc_mode)
	{
		switch(esc_mode)
		{
			case 1:
				switch(digit)
				{
					case '[':
						esc_mode = 2;
						break;
					case 'O':
						esc_mode = 3;
						break;
				}
				break;
			case 2:
				switch(digit)
				{
					case 'A':
						val = TERM_CURSOR_UP;
						break;
					case 'B':
						val = TERM_CURSOR_DOWN;
						break;
					case 'C':
						val = TERM_CURSOR_RIGHT;
						break;
					case 'D':
					 	val = TERM_CURSOR_LEFT;
						break;
					case 'H':
						break;
					case 'K':
						break;
				}
				esc_mode = 0;
				break;
			case 3:
				switch(digit)
				{
					case 'P'://F1
						val = TERM_F1;
						break;
					case 'Q'://F2
						val = TERM_F2;
						break;
					case 'R'://F3
						val = TERM_F3;
						break;
					case 'S'://F4
						val = TERM_F4;
						break;
					default:
						val = -1;
				}
				esc_mode = 0;
				break;
		}
	}
	else
	{
		val = digit;
	}

	if(val >= 0)
	{
		blue_os_eventSignal(&blue_os_vt100Event, (uint8_t*)&val);
	}

}

#endif
