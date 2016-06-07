#include "blue_os.h"
#include "constants.h"
#include "motor.h"
#include "speedometer.h"
#include "pid.h"

#define SPEEDOMETER

void pid()
{
	// TODO: find out correct values
	const int Kp = -1, Ki = -1, Kd = -1;
	// TODO: make setpoint extern variable
	int setpoint = 0;
	// TODO: implement using fixed point
	float output, err, ierr = 0, derr, prev_err = 0;
	
	while (1)
	{
		err = setpoint - getCurrentSpeed();
		ierr += err;
		derr = err - prev_err;
		output = Kp*err + Ki*ierr + Kd*derr;
		accelerate(output);
		prev_err = err;
	}
}