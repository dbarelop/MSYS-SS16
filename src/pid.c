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
	pid_diff();
}

/*
 * Implementation of the PID position algorithm.
 */
void pid_pos()
{
	// TODO: implement using fixed point
	const float q0 = Kp + Ki * Ta + Kd / Ta, 
				q1 = -Kp - 2 * Kd / Ta, 
				q2 = Kd / Ta;
	float output, err, prev_err = 0, prev_err2 = 0;

	while (1)
	{
		err = target_speed - getCurrentSpeed();

		output += q0 * err + q1 * prev_err + q2 * prev_err2;

		prev_err2 = prev_err;
		prev_err = err;
		blueOsDelay(250);
	}
}

/*
 * Implementation of the PID differential algorithm.
 */
void pid_diff()
{
	// TODO: implement using fixed point
	float output, err, ierr = 0, derr, prev_err = 0;
	
	while (1)
	{
		err = target_speed - getCurrentSpeed();
		ierr += err;
		derr = err - prev_err;

		output = (Kp * err) + (Ki * Ta * ierr) + (Kd * derr / Ta);

		accelerate(output);

		prev_err = err;
		blueOsDelay(250);
	}
}

void setTargetSpeed(unsigned int new_value)
{
	target_speed = new_value;
}