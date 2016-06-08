#include "blue_os.h"
#include "constants.h"
#include "motor.h"
#include "speedometer.h"
#include "pid.h"

// TODO: check values
#define Ta 0.25
#define Kp 9.0
#define Ki 90.0
#define Kd 0.025

static volatile unsigned int target_speed = 0;

void pid()
{
	// TODO: implement using fixed point
	float output, err, ierr = 0, derr, prev_err = 0;
	
	while (1)
	{
		err = target_speed - getCurrentSpeed();
		ierr += err;
		derr = err - prev_err;
		prev_err = err;

		output = (Kp * err) + (Ki * Ta * ierr) + (Kd * derr / Ta);

		accelerate(output);

		blueOsDelay(250);
	}
}

void setTargetSpeed(unsigned int new_value)
{
	target_speed = new_value;
}