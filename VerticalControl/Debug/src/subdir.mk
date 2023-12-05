################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc_thread_entry.c \
../src/can_thread_entry.c \
../src/drv_thread_entry.c \
../src/status_thread_entry.c 

C_DEPS += \
./src/adc_thread_entry.d \
./src/can_thread_entry.d \
./src/drv_thread_entry.d \
./src/status_thread_entry.d 

OBJS += \
./src/adc_thread_entry.o \
./src/can_thread_entry.o \
./src/drv_thread_entry.o \
./src/status_thread_entry.o 

SREC += \
VerticalControl.srec 

MAP += \
VerticalControl.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_SYNERGY_ -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy_cfg/ssp_cfg/bsp" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy_cfg/ssp_cfg/driver" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/inc" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/inc/bsp" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/inc/bsp/cmsis/Include" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/inc/driver/api" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/inc/driver/instances" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/src" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/src/synergy_gen" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy_cfg/ssp_cfg/framework/el" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/inc/framework/el" -I"C:/Users/khurr/e2_studio_latest/workspace/VerticalControl/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

