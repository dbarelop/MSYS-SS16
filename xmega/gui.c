#include "BlueOS/source/blue_os.h"
#include "constants.h"
#include "gui.h"
#include "motor.h"
#include "pid.h"
#include "speedometer.h"

static volatile unsigned int target_speed = 0;
static volatile action_t cursor = ACCELERATE;

void gui()
{
	unsigned int current_output, current_speed;
	action_t cursor_tmp;
	blueOsInitShellVt100();
	while(1)
	{
		blueOsClearScreen();
		// Draw the main frame
		blueOsEnterGraphic();
		blueOsDrawMainFrame(3, 1, 80, 24);
		blueOsDrawHorizLine(4 , 3, 78);
		blueOsLeaveGraphic();
		blueOsSetPosition(2, 35);
		blueOsWriteString("MSYS SS16");
		// Display the parameters
		blueOsSetPosition(4, 5);
		blueOsWriteString("Parameters");
		// Display the current speed in RPS
		current_speed = getCurrentSpeedRPS();
		blueOsSetPosition(6, 5);
		blueOsWriteString("* Current speed:  ");
		blueOsWriteInt(current_speed / 10, 4);
		blueOsWriteString(".");
		blueOsWriteInt(current_speed % 10, 1);
		blueOsWriteString(" rps");
		// Display the target speed in RPS
		blueOsSetPosition(8, 5);
		blueOsWriteString("* Target speed:   ");
		blueOsWriteInt(target_speed, 4);
		blueOsWriteString(" rps");
		// Display the current output value in %
		current_output = getOutput();
		blueOsSetPosition(10, 5);
		blueOsWriteString("* Current output: ");
		blueOsWriteInt(current_output / 10, 3);
		blueOsWriteString(".");
		blueOsWriteInt(current_output % 10, 1);
		blueOsWriteString(" %");
		// Display the buttons and highlight the cursor selection
		cursor_tmp = cursor;
		blueOsSetPosition(14, 20);
		blueOsSetInvers(cursor_tmp == ACCELERATE);
		blueOsWriteString("Accelerate 1");
		blueOsSetPosition(14, 40);
		blueOsSetInvers(cursor_tmp == DECELERATE);
		blueOsWriteString("Decelerate 1");
		blueOsSetPosition(14, 60);
		blueOsSetInvers(cursor_tmp == STOP);
		blueOsWriteString("Stop");
		blueOsSetInvers(0);
		// Display the help
		blueOsSetPosition(18, 5);
		blueOsWriteString("Help:");
		blueOsSetPosition(20, 5);
		blueOsWriteString("- Press up to increase the speed by 10 rpm (MAX set to");
		blueOsWriteInt(MAX_SPEED_RPS, 2);
		blueOsWriteString(")");
		blueOsSetPosition(22, 5);
		blueOsWriteString("- Press down to decrease the speed by 10 rpm");
		// Move the VT100 cursor to the bottom
		blueOsSetPosition(24, 1);
		
		blueOsDelay(30);
	}
}

void inputParser()
{
	uint8_t buf;
	while (1)
	{
		if (blueOsShellRead(&buf, 1) >= sizeof(uint8_t))
		{
			switch (buf)
			{
				case '[':
					if (blueOsShellRead(&buf, 1) >= sizeof(uint8_t))
					{
						switch (buf)
						{
							case 'C':
								// Cursor right
								cursor = cursor < 2 ? cursor + 1 : 2;
								break;
							case 'D':
								// Cursor left
								cursor = cursor > 0 ? cursor - 1 : 0;
								break;
							case 'A':
								// Accelerate
								target_speed = target_speed + 10 < MAX_SPEED_RPS ? target_speed + 10 : MAX_SPEED_RPS;
								break;
							case 'B':
								// Decelerate
								target_speed = (int) target_speed - 10 > 0 ? target_speed - 10 : 0;
								break;
						}
					}
					break;
				case '\r':
					switch (cursor)
					{
						case ACCELERATE:
							target_speed = target_speed + 1 < MAX_SPEED_RPS ? target_speed + 1 : MAX_SPEED_RPS;
							break;
						case DECELERATE:
							target_speed = (int) target_speed - 1 > 0 ? target_speed - 1 : 0;
							break;
						case STOP:
							target_speed = 0;
							brake();
							break;
					}
					break;
			}
			setTargetSpeedRPS(target_speed);
		}
		blueOsDelay(50);
	}
}