
#ifndef MSP432_MPU6050_H_
#define MSP432_MPU6050_H_


#include <stdint.h>
#include <stdio.h>
#include <ti/drivers/I2C.h>



/*
 * Definitions
 *
 */

#define MPU6050_ADDR 0x68
#define MPU6050_POWER_MGMT_1 0x6B
#define MPU6050_WHOAMI 0x75
//#define MPU6050_GYRO 0x3B
//#define MPU6050_ACCEL 0x43
#define MPU6050_ACCEL 0x3B
#define MPU6050_GYRO 0x43

/*
 * Function Prototype Declarations (General Helpers)
 */
void delayMs(int n);
int16_t merge8bitTo16bit(uint8_t val1, uint8_t val2);



/*
 * Function Prototype Declarations (I2C)
 *
 */
void i2cWrite(I2C_Handle i2c, uint8_t slaveRegAddr, uint8_t value);
uint8_t* i2cRead(I2C_Handle i2c, uint8_t regAddr, uint8_t readCount);
void initialiseI2C(int i2cBitRate);
void closeI2C();



/*
 * Function Prototype Declarations (MPU6050)
 *
 */
//STARTUP
void startMPU6050();
void wakeMPU6050();

void setRegister(uint8_t regAddr, uint8_t value);
uint8_t getRegisterValue(uint8_t regAddr);

int16_t* get3Axis(uint8_t regAddr);
int16_t* getGyro();
int16_t* getAccel();
void SysTick_init(void);
int16_t* getDegree(int x, int y, int z,int Delta);
int16_t* getFinalAngle(int16_t* degree, int16_t* accel);
#endif /* MSP432_MPU6050_H_ */
