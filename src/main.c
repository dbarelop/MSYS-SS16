/*
 * main.c
 *
 * Created: 07.04.2016 19:17:38
 *  Author: da431lop
 */ 

#include <util/delay.h>
#include "motor.h"

int main(void)
{
	int MAX_SPEED = 1000;
	int i = 0;
	int speed, step;
    while(1)
    {
		// Every 25 seconds, push the brake, wait for 2 seconds and restart speed
		if (i == 0)
		{
			speed = 0;
			step = 50;
			brake();
			_delay_ms(2000);
		}
        accelerate(speed);
		speed += step;
		if (speed == MAX_SPEED)
		{
			step *= -1;
		}
		i = (i + 1) % 1000;
		_delay_ms(250);
    }
}
