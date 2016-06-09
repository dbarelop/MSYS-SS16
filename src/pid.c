#include "blue_os.h"
#include "constants.h"
#include "motor.h"
#include "speedometer.h"
#include "pid.h"

// TODO: check values
#define _Ta 0.25
#define _Kp 9.0
#define _Ki 90.0
#define _Kd 0.025

#define Q 15
#define Ta ((int) (_Ta * (2 << Q) / 1000) == 0 ? 1 : (int) (_Ta * (2 << Q) / 1000))
#define Kp ((int) (_Kp * (2 << Q) / 1000) == 0 ? 1 : (int) (_Kp * (2 << Q) / 1000))
#define Ki ((int) (_Ki * (2 << Q) / 1000) == 0 ? 1 : (int) (_Ki * (2 << Q) / 1000))
#define Kd ((int) (_Kd * (2 << Q) / 1000) == 0 ? 1 : (int) (_Kd * (2 << Q) / 1000))

static volatile unsigned int target_speed = 0;
static volatile unsigned int output = 0;

/*
 * Implementation of the PID position algorithm.
 */
#define Q0 (Kp + Ki * Ta + Kd / Ta)
#define Q1 (-Kp - 2 * Kd / Ta)
#define Q2 (Kd / Ta)
void pid_pos()
{
	// FIXME: very slow!!
	int output_tmp = 0, err, prev_err = 0, prev_err2 = 0;

	while (1)
	{
		err = target_speed - getCurrentSpeedRPS();

		output_tmp += Q0 * err + Q1 * prev_err + Q2 * prev_err2;
		output = output_tmp < 0 ? 0 : (output_tmp > MAX_OUTPUT ? MAX_OUTPUT : output_tmp);
		accelerate(output);

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
	int output_tmp, err, ierr = 0, derr, prev_err = 0;

	while (1)
	{
		err = target_speed - getCurrentSpeedRPS();
		ierr += err;
		derr = err - prev_err;

		output_tmp = (Kp * err) + (Ki * Ta * ierr) + (Kd * derr / Ta);
		output = output_tmp < 0 ? 0 : (output_tmp > MAX_OUTPUT ? MAX_OUTPUT : output_tmp);
		accelerate(output);

		prev_err = err;
		blueOsDelay(10);
	}
}

void pid()
{
	pid_diff();
}

void setTargetSpeedRPS(unsigned int new_value)
{
	target_speed = new_value;
}

unsigned int getOutput()
{
	return output;
}