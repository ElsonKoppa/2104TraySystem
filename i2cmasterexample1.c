/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
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
 */

/*
 *    ======== i2cmasterexample1.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include <stdio.h>
#include "msp432_mpu6050.h"
#include "msp.h"
/* DriverLib Includes */
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    printf("\n== Start of thread ==\n\n");
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P6REN |= 0x30;
    P6OUT |= 0x30;

    printf("Startup MPU6050\n\n");
    startMPU6050();
    SysTick_init();
    //delayMs(5000);
    //calibrateMPU6050();
    printf("==================================\n");
    printf("Reading MPU6050 Data...\n\n");
        while(1)
        {
            P1OUT ^= BIT0;
            //getGyroData();
            // Gyro Data
            int Start = SysTick->VAL;
            int16_t* gyro = getGyro();
            int End = SysTick->VAL;
            int Delta = 0xFFFFFF & (Start - End);
            int x = (gyro[0]/131)+4;
            int y = (gyro[1]/131)-4;
            int z = gyro[2]/131;
//            printf("[Gyro]  X = %-4d, Y =  %-4d, Z = %-4d\n", (gyro[0]/131)+4, (gyro[1]/131)-4, gyro[2]/131);
            int16_t* degree = getDegree(x,y,z, Delta);
//            printf("[Gyro]  X = %-4d, Y =  %-4d, Z = %-4d\n", degree[0], degree[1],degree[2]);

            //Accel data
            int16_t* accel = getAccel();
//            printf("\n[Accel] X = %-4d, Y = %-4d, Z = %-4d\n", accel[0]/16384, accel[1]/16384, accel[2]/16384);

            int16_t* finalAngle = getFinalAngle(degree, accel);
            printf("[Gyro]  X = %-4d, Y =  %-4d, Z = %-4d\n", finalAngle[0], finalAngle[1],finalAngle[2]);

            printf("----------------------------------------\n");

            delayMs(1000);
        }
    return (0);
}
