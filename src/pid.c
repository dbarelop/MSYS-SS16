#include "blue_os.h"
#include "constants.h"
#include "motor.h"
#include "speedometer.h"
#include "pid.h"

// TODO: check values
#define KP 20000
#define KI 100000
#define KD 1500

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
		output = KP*err + KI*ierr + KD*derr;
		accelerate(output);
		prev_err = err;
	}
}

void setTargetSpeed(unsigned int new_value)
{
	target_speed = new_value;
}