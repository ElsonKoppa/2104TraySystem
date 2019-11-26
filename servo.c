/* For usleep() */
#include <unistd.h>
#include <stddef.h>
#include <servo.h>
/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <msp432_mpu6050.h>

/* Driver configuration */
#include "ti_drivers_config.h"
PWM_Handle pwm2 = NULL;
PWM_Params params;
void startServo(){

    /* Period and duty in microseconds */
        uint16_t   pwmPeriod = 5000;
        uint16_t   duty = 1000;
        uint16_t   dutyInc = 500;

        /* Sleep time in microseconds */
        uint32_t   time = 500000;
//        PWM_Handle pwm1 = NULL;
//        PWM_Handle pwm2 = NULL;
//        PWM_Params params;

        /* Call driver init functions. */
        PWM_init();
    //  int degree = getDegree();////////////////////////////////////////////////////////////////////////////
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

}


void turn_cw(int degree){
    printf("turn clockwise");
    int finalPwm = 0;
    int base_pwm = 1000;
    //make value go lower
    //finalPwm =base_pwm + floor(degree/90 * 50);

    PWM_setDuty(pwm2, 1500);
    delayMs(5000);
}

void turn_acw(int degree){
    printf("turn anti clockwise");
    int finalPwm = 0;
    int base_pwm = 1000;
    //make value go lower
//    finalPwm =base_pwm - floor(degree/90 * 50);

    PWM_setDuty(pwm2, 3500);
    delayMs(5000);

}

void turn(){
    int i = 0;
    int duty = 1000;
    int dutyInc = 500;
    for(i=0; i<11; i++) {
            PWM_setDuty(pwm2, duty);
            duty = (duty + dutyInc);

            if (duty == 3500 || duty == 1000 || (!duty)) {
                dutyInc = - dutyInc;
            }
            delayMs(5000);
        }
}



