/*
 * main.c
 *
 * Created: 07.04.2016 19:17:38
 *  Author: da431lop
 */ 

#include "blue_os.h"
#include "motor.h"

int main(void)
{
    // Configure pin 3 from port F as output and pin 2 as input to use the UART
    //PORTF.DIR = (0x01 << 4);
    PORTF.DIR = 0b00001000;
    while (1)
    {
        blueOsWriteString("Hello world!\n");
        blueOsDelay(1000);
    }
}

int main2(void)
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
            blueOsDelay(2000);
        }
        accelerate(speed);
        speed += step;
        if (speed == MAX_SPEED)
        {
            step *= -1;
        }
        i = (i + 1) % 1000;
        blueOsDelay(250);
    }
}
