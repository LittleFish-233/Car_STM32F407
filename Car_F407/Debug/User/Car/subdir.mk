################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Car/GMR_Encoder.c \
../User/Car/TB6612.c \
../User/Car/infrared.c \
../User/Car/speedControl.c \
../User/Car/speedPID.c 

OBJS += \
./User/Car/GMR_Encoder.o \
./User/Car/TB6612.o \
./User/Car/infrared.o \
./User/Car/speedControl.o \
./User/Car/speedPID.o 

C_DEPS += \
./User/Car/GMR_Encoder.d \
./User/Car/TB6612.d \
./User/Car/infrared.d \
./User/Car/speedControl.d \
./User/Car/speedPID.d 


# Each subdirectory must supply rules for building sources it contributes
User/Car/%.o User/Car/%.su: ../User/Car/%.c User/Car/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"G:/Code/Car_F407/Car_F407/User" -I../FATFS/Target -I../FATFS/App -I"G:/Code/Car_F407/Car_F407/Middlewares/Third_Party/FatFs/src" -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Car

clean-User-2f-Car:
	-$(RM) ./User/Car/GMR_Encoder.d ./User/Car/GMR_Encoder.o ./User/Car/GMR_Encoder.su ./User/Car/TB6612.d ./User/Car/TB6612.o ./User/Car/TB6612.su ./User/Car/infrared.d ./User/Car/infrared.o ./User/Car/infrared.su ./User/Car/speedControl.d ./User/Car/speedControl.o ./User/Car/speedControl.su ./User/Car/speedPID.d ./User/Car/speedPID.o ./User/Car/speedPID.su

.PHONY: clean-User-2f-Car

