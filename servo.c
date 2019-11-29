/* For usleep() */
#include <unistd.h>
#include <stddef.h>
#include <servo.h>
/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <msp432_mpu6050.h>
#include <math.h>
/* Driver configuration */
#include "ti_drivers_config.h"
PWM_Handle pwm2 = NULL;
PWM_Params params;
int delayTime =10;


/* Initialization of Servo */
void startServo(){

    /* Period and duty in microseconds */
        uint16_t   pwmPeriod = 5000;
        uint16_t   duty = 1000;
        uint16_t   dutyInc = 500;

        /* Sleep time in microseconds */
        uint32_t   time = 500000;


        /* Call driver init functions. */
        PWM_init();
        PWM_Params_init(&params);
        params.dutyUnits = PWM_DUTY_US;
        params.dutyValue = 0;
        params.periodUnits = PWM_PERIOD_US;
        params.periodValue = pwmPeriod;

        pwm2 = PWM_open(CONFIG_PWM_0, &params);
        if (pwm2 == NULL) {
            /* CONFIG_PWM_0 did not open */
            printf("did not open\n");
            while (1);
        }

        PWM_start(pwm2);

        //PID INITIALISE
        SetTunings(0.8);
        setPoint = -500;


}


//setPoint is the power that i continuously transmit to motor.
//Error is the extra power to give to the turn_cw turn_ccw power
void compute(int in)
{
    //This is part of P(ID) implementation that we tried that did not get it to work
//    unsigned long now = millis();
//    double timeChange = (double)(now - lastTime);
    double error; //Error will be the difference in base gyroscope reading VS current gyroscope reading
    if(setPoint > in){
       error = setPoint - in;
    } else {
       error = in - setPoint;
    }

    //This is part of P(ID) implementation that we tried that did not get it to work
    //errSum += (error * timeChange);
    //double dErr = (error - lastErr) / timeChange;

   /*Compute PID Output*/
    //This will be the entire working PID if we manage to implement the I and D.
    //Output = kp * error + ki * errSum + kd * dErr;


    //Output is the amount extra amount of power that i require the PWM to output or downput, in form of gyroscope readings.
    //16000 and -17000 are the extreme ranges for gyroscope.
    //-500 is the middle point which we will take as the base resting horizontol position for the tray.
    //We take -500 +-150 as the threshold for the turn_cw or turn_acw will run.
   Output = kp * error;
   if(in>=-350 && in <=16000){
       printf("Turn Right\n");
       turn_cw(error);
   }
   else if(in<=-650 && in>=-17000){
       printf("Turn Right\n");
       turn_acw(error);
   }
   else{
       delayMs(delayTime); //This is the nomove threshold.
   }

}

//This is the function to call to tune the PID.
void SetTunings(double Kp)
{
   kp = Kp;

   //This are the I and D variables that will be used if we can get the Integral and Derivative part to run.
//   ki = Ki;
//   kd = Kd;
}

/**This functions control the amount of PWM to produce to the Servo for the deviation of original value**/
void turn_cw(int gyroReading){
    printf("turn clockwise by this amount %d\n",gyroReading);
    int basePWM = 1000; //1000 is the base PWM at hosrizontal position.
    finalPWM =basePWM - floor((double)gyroReading/16500 * 500); //This is the final PWM to be sent to the servo. 500 is the amount of PWM needed to turn the servo from a rest position to 90 degrees.
    printf("This is final pwm to be given to motor: %d\n",finalPWM);
    PWM_setDuty(pwm2, finalPWM);
    delayMs(delayTime);
}

/**This functions control the amount of PWM to produce to the Servo for the deviation of original value**/
void turn_acw(int gyroReading){
    printf("turn anti clockwise by this amount %d\n",gyroReading);
    int basePWM = 1000; //1000 is the base PWM at hosrizontal position.
    finalPWM =basePWM + floor((double)gyroReading/16500 * 500); //This is the final PWM to be sent to the servo. 500 is the amount of PWM needed to turn the servo from a rest position to 90 degrees.
    printf("This is final pwm to be given to motor: %d\n",finalPWM);
    PWM_setDuty(pwm2, finalPWM);
    delayMs(delayTime);

}

