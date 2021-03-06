#include <stdlib.h>

#include "BlueOS/source/blue_os.h"
#include "constants.h"
#include "motor.h"
#include "motor_test.h"
#include "speedometer.h"

void motorTest()
{
	accelerate(0);
	/*int i = 10, speed;
	while (1)
	{
		blueOsClearScreen();
		accelerate(i);
		speed = getCurrentSpeed();
		i += 10;
		i = (i % 40);
		blueOsWriteInt(i, 10);
		blueOsWriteString(", ");
		blueOsWriteInt(speed, 10);
		blueOsDelay(1000);
	}*/
}

void motorTest2()
{
	char buffer[8];
	int MAX_SPEED = 1000;
	int i = 0;
	int speed = 0, step = 50;
	while(1)
	{
		itoa(speed, buffer, 10);
		blueOsWriteString(buffer);
		blueOsWriteString(",");
		itoa(i, buffer, 10);
		blueOsWriteString(buffer);
		//blueOsWriteString("\n");
		// Every 25 seconds, push the brake, wait for 2 seconds and restart speed
		if (i == 0)
		{
			speed = 0;
			step = 50;
			brake();
			blueOsDelay(2000);
		}
		accelerate(speed);
		speed += step;
		if (speed == 0 || speed == MAX_SPEED)
		{
			step *= -1;
		}
		i = (i + 1) % 1000;
		blueOsDelay(1000);
	}
}
