################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_TIRTOS.cmd 

SYSCFG_SRCS += \
../i2cmasterexample1.syscfg 

C_SRCS += \
../i2cmasterexample1.c \
./syscfg/ti_drivers_config.c \
../main_tirtos.c \
../msp432_mpu6050.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./i2cmasterexample1.d \
./syscfg/ti_drivers_config.d \
./main_tirtos.d \
./msp432_mpu6050.d 

OBJS += \
./i2cmasterexample1.obj \
./syscfg/ti_drivers_config.obj \
./main_tirtos.obj \
./msp432_mpu6050.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg/" 

OBJS__QUOTED += \
"i2cmasterexample1.obj" \
"syscfg/ti_drivers_config.obj" \
"main_tirtos.obj" \
"msp432_mpu6050.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg/ti_drivers_config.h" \
"syscfg/syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"i2cmasterexample1.d" \
"syscfg/ti_drivers_config.d" \
"main_tirtos.d" \
"msp432_mpu6050.d" 

GEN_FILES__QUOTED += \
"syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../i2cmasterexample1.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_drivers_config.c" \
"../main_tirtos.c" 


