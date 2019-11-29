#ifndef SERVO_H_
#define SERVO_H_

/* For usleep() */
#include <unistd.h>
#include <stddef.h>
/* Driver Header files */
#include <ti/drivers/PWM.h>
/* Driver configuration */
#include "ti_drivers_config.h"


//This 5 commented one are for the ID part of PID.
//unsigned long lastTime;
//double errSum, lastErr;
//double ki, kd;
//double lastInput;
double kp;
double Output, setPoint; //setPoint is the base gyroscope reading at rest for the tray.
int finalPWM; //The final PWM to give to the servo to make the necerssary changes.



/*
 * Function Prototype Declarations (MPU6050)
 *
 */

void startServo();
void turn_cw(int degree);
void turn_acw(int degree);
void turn();
void compute(int in);
void SetTunings(double Kp);


#endif
