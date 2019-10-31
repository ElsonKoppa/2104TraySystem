/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 Timer_A - Variable PWM
 *
 * Description: In this example, the Timer_A module is used to create a precision 
 * PWM with an adjustable duty cycle. The PWM initial period is 200 ms and is
 * output on P2.4. The initial duty cycle of the PWM is 10%, however when the
 * button is pressed on P1.1 the duty cycle is sequentially increased by 10%.
 * Once the duty cycle reaches 90%, the duty cycle is reset to 10% on the
 * following button press.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.1  |<--Toggle Switch
 *            |                  |
 *            |                  |
 *            |            P2.4  |--> Output PWM
 *            |                  |
 *            |                  |
 *
 *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


Timer_A_PWMConfig pwmConfig =
{
        //uint_fast16_t clockSource;
        TIMER_A_CLOCKSOURCE_SMCLK,
        //uint_fast16_t clockSourceDivider;
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        //uint_fast16_t timerPeriod;
        1500,
        //uint_fast16_t compareRegister;
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        //uint_fast16_t compareOutputMode;
        TIMER_A_OUTPUTMODE_RESET_SET,
        //uint_fast16_t dutyCycle;
        100
};

// PWM - PIN - 5.6 - TIMER_A2_BASE
// PWM - PIN - 2.4 - TIMER_A0_BASE
int speed = 1000;
void delayMs(int n);
void turn_cw(float a);
void turn_acw(float a);
void init_pwm(int port, int pin);
int main(void)
{
    /* Halting the watchdog */
    MAP_WDT_A_holdTimer();


    /* Setting MCLK to REFO at 128Khz for LF mode setting SMCLK to 64Khz
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_PCM_setPowerState(PCM_AM_LF_VCORE0);

    // Configuring GPIO2.4 as peripheral output for PWM
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configuring Timer_A to have a period of approximately 20ms
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);*/


    init_pwm(5, 64); //pin 5.6



    P1DIR &= ~BIT1;
    P1DIR &= ~BIT4;
    //ENABLE REGISTER
    P1REN |= BIT1;
    P1REN |= BIT4;
    //PULL UP RESISTER
    P1OUT |= BIT1;
    P1OUT |= BIT4;
    //ENABLE INTERRUPT
    P1IE |= BIT1;
    P1IE |= BIT4;
    //HIGH TO LOW TRANSITION
    P1IES |= BIT1;
    P1IES |= BIT4;
    //CLEAR INTERRUPT FLAG
    P1IFG = 0;

    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    __enable_interrupts();

    //SET LED TO OUTPUT
    P1DIR |= BIT0;
    P2DIR |= BIT0;
    P2DIR |= BIT1;
    P2DIR |= BIT2;
    //CLEAR THE LED
    P1OUT &= ~BIT0;
    P2OUT &= ~BIT0;
    P2OUT &= ~BIT1;
    P2OUT &= ~BIT2;

    int counter = 0;
    while (1)
    {
        if (counter == 0){
            //CLEAR UNWANTED LED
            P2OUT &= ~BIT0;
            P2OUT &= ~BIT1;
            //TOGGLE RED LED
            P1OUT ^= BIT0;
            //ON BLUE LED
            P2OUT |= BIT2;
        }
        else if (counter == 1){
            //CLEAR UNWANTED LED
            P2OUT &= ~BIT0;
            P2OUT &= ~BIT2;
            //ON GREEN LED
            P2OUT |= BIT1;
        }
        else if (counter == 2){
            //CLEAR UNWANTED LED
            P2OUT &= ~BIT1;
            P2OUT &= ~BIT2;
            //ON RED LED
            P2OUT |= BIT0;
            counter = -1;
        }
        //delayMs(speed);
        counter += 1;

        int a;
        //pwmConfig.dutyCycle = 102;  //102 is flat
        //for(a=10000; a>0; a--);
        //MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);

    }
}

void delayMs(int n)
{
    int i;

    TIMER32_1->LOAD = 3000 - 1;
    TIMER32_1->CONTROL = 0xC2;

    for(i = 0; i < n; i++) {
        while((TIMER32_1->RIS & 1) == 0);
        TIMER32_1->INTCLR = 0;
    }
}


void PORT1_IRQHandler(void){
    int a;
    //left button click left up
    if(P1IFG & BIT1){
//        pwmConfig.dutyCycle = 50;  //50 is left up
//        //for(a=5000; a>0; a--);
//        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
        turn_cw(45);
        P1IFG &= ~BIT1;
    }
    //right button click right up
    else if (P1IFG & BIT4){
//        pwmConfig.dutyCycle = 150;  //150 is right up
//        //for(a=5000; a>0; a--);
//        MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
        turn_acw(45);
        P1IFG &= ~BIT4;
    }
}


void init_pwm(int port, int pin){\
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    MAP_CS_initClockSignal(CS_MCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_PCM_setPowerState(PCM_AM_LF_VCORE0);

    // Configuring GPIO2.4 as peripheral output for PWM
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(port, pin, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configuring Timer_A to have a period of approximately 20ms
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);


}


void turn_cw(float degree){
    int finalPwm = 0;
    int base_pwm = 50;
    //make value go lower
    finalPwm =base_pwm + floor(degree/90.0 * 50);
    pwmConfig.dutyCycle = finalPwm;
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
}

void turn_acw(float degree){
    int finalPwm = 0;
    int base_pwm = 150;
    //make value go lower
    finalPwm =base_pwm - floor(degree/90.0 * 50);
    pwmConfig.dutyCycle = finalPwm;
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);
}


/*
pwmConfig.dutyCycle = b;
b = b + 5; //changing this value will rotate lower distance (original value +10)
if(b == 170)
b = 40; // the bigger the number the shorter it start from
for(a=5000; a>0; a--); // change delay
MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);

*/
