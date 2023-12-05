################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc_thread_entry.c \
../src/can_thread_entry.c \
../src/drv_thread_entry.c \
../src/hal_entry.c \
../src/lcd_thread_entry.c \
../src/status_led_entry.c 

C_DEPS += \
./src/adc_thread_entry.d \
./src/can_thread_entry.d \
./src/drv_thread_entry.d \
./src/hal_entry.d \
./src/lcd_thread_entry.d \
./src/status_led_entry.d 

OBJS += \
./src/adc_thread_entry.o \
./src/can_thread_entry.o \
./src/drv_thread_entry.o \
./src/hal_entry.o \
./src/lcd_thread_entry.o \
./src/status_led_entry.o 

SREC += \
COPJunction.srec 

MAP += \
COPJunction.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_SYNERGY_ -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy_cfg/ssp_cfg/bsp" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy_cfg/ssp_cfg/driver" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/bsp" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/bsp/cmsis/Include" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/driver/api" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/driver/instances" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/src" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/src/synergy_gen" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy_cfg/ssp_cfg/framework/el" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/inc/framework/el" -I"C:/Users/khurr/e2_studio_latest/workspace/COPJunction/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

