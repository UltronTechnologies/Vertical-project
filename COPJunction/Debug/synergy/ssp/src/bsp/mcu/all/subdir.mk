################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/bsp/mcu/all/bsp_common.c \
../synergy/ssp/src/bsp/mcu/all/bsp_common_leds.c \
../synergy/ssp/src/bsp/mcu/all/bsp_delay.c \
../synergy/ssp/src/bsp/mcu/all/bsp_irq.c \
../synergy/ssp/src/bsp/mcu/all/bsp_locking.c \
../synergy/ssp/src/bsp/mcu/all/bsp_register_protection.c \
../synergy/ssp/src/bsp/mcu/all/bsp_sbrk.c 

C_DEPS += \
./synergy/ssp/src/bsp/mcu/all/bsp_common.d \
./synergy/ssp/src/bsp/mcu/all/bsp_common_leds.d \
./synergy/ssp/src/bsp/mcu/all/bsp_delay.d \
./synergy/ssp/src/bsp/mcu/all/bsp_irq.d \
./synergy/ssp/src/bsp/mcu/all/bsp_locking.d \
./synergy/ssp/src/bsp/mcu/all/bsp_register_protection.d \
./synergy/ssp/src/bsp/mcu/all/bsp_sbrk.d 

OBJS += \
./synergy/ssp/src/bsp/mcu/all/bsp_common.o \
./synergy/ssp/src/bsp/mcu/all/bsp_common_leds.o \
./synergy/ssp/src/bsp/mcu/all/bsp_delay.o \
./synergy/ssp/src/bsp/mcu/all/bsp_irq.o \
./synergy/ssp/src/bsp/mcu/all/bsp_locking.o \
./synergy/ssp/src/bsp/mcu/all/bsp_register_protection.o \
./synergy/ssp/src/bsp/mcu/all/bsp_sbrk.o 

SREC += \
COPJunction.srec 

MAP += \
COPJunction.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/bsp/mcu/all/%.o: ../synergy/ssp/src/bsp/mcu/all/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_SYNERGY_ -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy_cfg/ssp_cfg/bsp" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy_cfg/ssp_cfg/driver" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/bsp" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/bsp/cmsis/Include" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/driver/api" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/driver/instances" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/src" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/src/synergy_gen" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy_cfg/ssp_cfg/framework/el" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/framework/el" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

