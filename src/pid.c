#include "blue_os.h"
#include "constants.h"
#include "motor.h"
#include "speedometer.h"
#include "pid.h"

#define FIXED_POINT

// TODO: check values
#define _Ta 0.25
#define _Kp 9.0
#define _Ki 90.0
#define _Kd 0.025

#define Q 15
#define Ta (int) (_Ta * (2 << Q) / 1000)
#define Kp (int) (_Kp * (2 << Q) / 1000)
#define Ki (int) (_Ki * (2 << Q) / 1000)
#define Kd (int) (_Kd * (2 << Q) / 1000)

static volatile unsigned int target_speed = 0;

/*
 * Implementation of the PID position algorithm.
 */
void pid_pos()
{
	#ifdef FIXED_POINT
	const int	q0 = Kp + Ki * Ta + Kd / Ta,
				q1 = -Kp - 2 * Kd / Ta,
				q2 = Kd / Ta;
	unsigned int output = 0;
	int err, prev_err = 0, prev_err2 = 0;
	#else
	const float q0 = _Kp + _Ki * _Ta + _Kd / _Ta,
				q1 = -_Kp - 2 * _Kd / _Ta,
				q2 = _Kd / _Ta;
	float output = 0, err, prev_err = 0, prev_err2 = 0;
	#endif

	while (1)
	{
		err = target_speed - getCurrentSpeed();

		output += q0 * err + q1 * prev_err + q2 * prev_err2;
		#ifdef FIXED_POINT
        accelerate(output * 1000 >> Q);
        #else
        accelerate((int) output);
        #endif

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
	#ifdef FIXED_POINT
	unsigned int output;
	int err, ierr = 0, derr, prev_err = 0;
	#else
	float output, err, ierr = 0, derr, prev_err = 0;
	#endif

	while (1)
	{
		#ifdef FIXED_POINT
		err = target_speed - (getCurrentSpeed() * (2 << Q) / 1000);
		#else
		err = target_speed - getCurrentSpeed();
		#endif
		ierr += err;
		derr = err - prev_err;

		#ifdef FIXED_POINT
		output = (Kp * err) + (Ki * Ta * ierr) + (Kd * derr / Ta);
		accelerate(output * 1000 >> Q);
		#else
		output = (_Kp * err) + (_Ki * _Ta * ierr) + (_Kd * derr / _Ta);
		accelerate((int) output);
		#endif

		prev_err = err;
		blueOsDelay(250);
	}
}

void pid()
{
	pid_diff();
}

void setTargetSpeed(unsigned int new_value)
{
	target_speed = new_value;
}