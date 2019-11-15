################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_TIRTOS.cmd 

SYSCFG_SRCS += \
../servompu6050.syscfg 

C_SRCS += \
../i2cmasterexample1.c \
../main_tirtos.c \
../msp432_mpu6050.c \
../pwmservo.c \
./syscfg/ti_drivers_config.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./i2cmasterexample1.d \
./main_tirtos.d \
./msp432_mpu6050.d \
./pwmservo.d \
./syscfg/ti_drivers_config.d 

OBJS += \
./i2cmasterexample1.obj \
./main_tirtos.obj \
./msp432_mpu6050.obj \
./pwmservo.obj \
./syscfg/ti_drivers_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"i2cmasterexample1.obj" \
"main_tirtos.obj" \
"msp432_mpu6050.obj" \
"pwmservo.obj" \
"syscfg\ti_drivers_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"i2cmasterexample1.d" \
"main_tirtos.d" \
"msp432_mpu6050.d" \
"pwmservo.d" \
"syscfg\ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../i2cmasterexample1.c" \
"../main_tirtos.c" \
"../msp432_mpu6050.c" \
"../pwmservo.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../servompu6050.syscfg" 


