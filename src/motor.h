/*
 * motor.h
 *
 * Created: 07.04.2016 20:08:54
 *  Author: da431lop
 */ 

/*
 * The motor will change its speed to the supplied value (being 0 the lowest
 * speed and 1000 the highest).
 */
void accelerate(int speed);

/*
 * The motor will be stopped
 */
void brake();

/*
 * Returns the current speed of the car (between 0 and 1000)
 */
int getCurrentSpeed();
