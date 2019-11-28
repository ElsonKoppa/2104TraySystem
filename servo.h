#ifndef SERVO_H_
#define SERVO_H_

/* For usleep() */
#include <unistd.h>
#include <stddef.h>
/* Driver Header files */
#include <ti/drivers/PWM.h>
/* Driver configuration */
#include "ti_drivers_config.h"

///* Period and duty in microseconds */
//uint16_t   pwmPeriod = 5000;
//uint16_t   duty = 1000;
//uint16_t   dutyInc = 500;
//
///* Sleep time in microseconds */
//uint32_t   time = 500000;


unsigned long lastTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
int finalPWM;

void startServo();
void turn_cw(int degree);
void turn_acw(int degree);
void turn();
void compute(int in);
void SetTunings(double Kp);


#endif
