################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_riic/r_riic.c 

C_DEPS += \
./synergy/ssp/src/driver/r_riic/r_riic.d 

OBJS += \
./synergy/ssp/src/driver/r_riic/r_riic.o 

SREC += \
COPJunction.srec 

MAP += \
COPJunction.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_riic/%.o: ../synergy/ssp/src/driver/r_riic/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_SYNERGY_ -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy_cfg/ssp_cfg/bsp" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy_cfg/ssp_cfg/driver" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/inc" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/inc/bsp" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/inc/bsp/cmsis/Include" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/inc/driver/api" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/inc/driver/instances" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/src" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/src/synergy_gen" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy_cfg/ssp_cfg/framework/el" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/inc/framework/el" -I"C:/Users/lukeb/My Drive/RandD/Projects/Savaria/COP/Source/v1-00/COPJunction/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

