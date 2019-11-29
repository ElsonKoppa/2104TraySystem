#include "msp.h"
#include <msp432_mpu6050.h>
#include <servo.h>
#include "ti_drivers_config.h"
#include <ti/drivers/I2C.h>
#include <stdio.h>


I2C_Handle      i2c;
I2C_Params      i2cParams;

I2C_Transaction i2cTransaction;         // Initialise I2C Transaction

uint8_t txBuffer[10];                   // Initialise I2C transmitting buffer for MPU6050
static uint8_t rxBuffer[20];            // Initialise I2C receiving buffer for MPU6050

uint8_t fs_sel = 3; //Gyro split
uint8_t afs_sel = 3; //accel split


/*
 * General Helper Functions
 *
 */
void delayMs(int n) {
    // Setting 1 cycle of Timer32 to be 1ms
    int i;
    TIMER32_1->LOAD = 48000 - 1;                  // Load timer with the number of ticks of the clock per millisecond (Clock rate/1000 e.g. 3Mhz/3 --> 3000)
    TIMER32_1->CONTROL = 0xC2;                   // Set Timer32 config: no prescaler, periodic wrapping mode, disable interrupt, 32-bit timer

    // Repeat the 1ms cycle of Timer32 n times
    for(i = 0; i < n; i++) {                     // Loop n number of times
        while((TIMER32_1->RIS & 1) == 0);        // Polling wait for RAW_IFG flag to be set when Timer32 finishes counting
            TIMER32_1->INTCLR = 0;               // Clear the RAW_IFG flag to 0
    }
}

int16_t merge8bitTo16bit(uint8_t val1, uint8_t val2) {
    /*
     * This function is to convert 2 8bit value from the GYRO and ACCEL from MPU Registers into a 16bit value
     * Input : 2 x 8bit unsigned int
     * Output: a 16bit integer
     * */
    return (int16_t) (((uint16_t) val1 << 8) | ((uint16_t) val2));
}


/*
 * I2C Functions
 *
 */

void i2cWrite(I2C_Handle i2c, uint8_t slaveRegAddr, uint8_t value)
{
    /*
     * A i2C write function from I2C.h
     * Input: i2c, Slave Address(MPU6050 Address), value to write.
     *
     * */

    // Set local boolean to check I2C transfer completion
    bool retVal = 0;
    // Set txBuffer Register Address
    txBuffer[0] = slaveRegAddr;
    // Set txBuffer data to write
    txBuffer[1] = value;
    i2cTransaction.slaveAddress = MPU6050_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;
    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);
}

uint8_t* i2cRead(I2C_Handle i2c, uint8_t regAddr, uint8_t readCount)
{    /*
     * A i2C Read function from I2C.h
     * Input: i2c, Slave Address(MPU6050 Address), value to read.
     *
     * */
    // Set local boolean to check I2C transfer completion
    bool retVal = 0;
    // Set transfer
    txBuffer[0] = regAddr; //register addr
    i2cTransaction.slaveAddress = MPU6050_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = readCount;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);
    // Return pointer to the receiving buffer for reading
    return rxBuffer;
}


//=============================================================================================

/*
 * MPU6050 Functions
 *
 */

void setRegister(uint8_t regAddr, uint8_t value)
{
    /*
     * Function for writing MPU Register to the I2C for read later.
     * Input : MPU Register,Value
     * */
    i2cWrite(i2c, regAddr, value);
}

uint8_t getRegister(uint8_t regAddr)
{
    /*
     * Function for getting MPU Register to the I2C for write later.
     * Input : MPU Register address
     * */
    return i2cRead(i2c, regAddr, 1)[0];
}

int16_t* get3AxisData(uint8_t regAddr)
{
    /*
     * getting the GYRO and/or Acceleration data based on the register address
     * Input : Address of the Gyro or Accel
     * Output : A 16bit int array of the X, Y and Z data of Gyro or Accel
     * */
    uint8_t* tempBuffer = i2cRead(i2c, regAddr, 6);
    static int16_t axisData[3];
    axisData[0] = merge8bitTo16bit(tempBuffer[0], tempBuffer[1]);
    axisData[1] = merge8bitTo16bit(tempBuffer[2], tempBuffer[3]);
    axisData[2] = merge8bitTo16bit(tempBuffer[4], tempBuffer[5]);
    return axisData;
}
int16_t* getGyro()
{
    /*
     * Getting the gyro data from the MPU Register which is 8bit each (Total there are 2Register)
     * Output : The XYZ Gyro data
     * */
    return get3AxisData(MPU6050_GYRO);
}

int16_t* getAccel()
{
    /*
     * Getting the Accel data from the MPU Register which is 8bit each (Total there are 2Register)
     * Output : The XYZ Accel data
     * */
    return get3AxisData(MPU6050_ACCEL);
}

void initialiseI2C(int i2cBitRate){
    //Init and set params for I2C
    printf("Initialising I2C\n");
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
   }

void closeI2C(){
    /* Deinitialise I2C */
    I2C_close(i2c);
    printf("I2C closed!\n");

}
void wakeMPU6050(){
    //Wake up MPU by setting the power from the register)
    printf("Waking up MPU6050\n");

    setRegister(MPU6050_POWER_MGMT_1, 0x40); //reset to default values
    delayMs(100);
    setRegister(MPU6050_POWER_MGMT_1, 0x00); //clear sleep bit
    delayMs(100);
    setRegister(MPU6050_POWER_MGMT_1, 0x01); //use internal gyro clock

    printf("MPU6050 Awake, WHOAMI = %x\n\n", getRegister(MPU6050_WHOAMI));
}

void startMPU6050()
{

    initialiseI2C(100);
    i2c = I2C_open(0, &i2cParams);
    P1OUT |= BIT0;


    printf("Power cycling MPU prior to reading of data for reliability\n");
    P6DIR = 0; //Port 6 direction for the SCL and SDA
    P6OUT = 0;
    delayMs(5000);
    P1OUT &= ~BIT0; //LED 1.0 for testing
    P6DIR = 1;
    P6OUT = 1;
    delayMs(5000);
    P1OUT |= BIT0;
    printf("Power cycle complete.\n");

    wakeMPU6050();

}
void SysTick_init(void){
    SysTick->LOAD = 0xFFFFFF; //Load Max 24Bit value into Systick
    SysTick->VAL = 0; //Clear Systick value
    SysTick->CTRL = 5; // Enable Systick , Disable interrupt, Enable clk source
}

int16_t* getDegree(int x, int y, int z, int delta){
    /*
     * Getting degree from the gyro because Gyro returns degree/sec.
     * Input : X, Y, Z and DELTA(SYSTICK->VAL)elapsed time
     * return degree
     * */

    int16_t degreeData[3] = 0;
    double periodTime = 0.00000002083; // 1/48MHz
    delta *= periodTime;
    degreeData[0] = degreeData[0] + (x * delta);
    degreeData[1] = degreeData[1] + (y * delta);
    degreeData[2] = degreeData[2] + (z * delta);
    return degreeData;
}

int16_t* getFinalAngle(int16_t* degree, int16_t* accel)
{
    /*Formular to get the final angle by combining the degree form getDegree and accel from MPU6050
     * Input : Degree, Accel
     * Output: Final Degree in 16bit int
     * */

    int16_t finalData[3];
    finalData[0] = (0.96 * degree[0]) + 0.04 * (accel[0]/16384);
    finalData[1] = (0.96 * degree[1]) + 0.04 * (accel[1]/16384);
    finalData[2] = degree[2];
    return finalData;
}



//=============================================================================================
