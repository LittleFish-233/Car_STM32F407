################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/BasicInfor/debug.c \
../User/BasicInfor/delay.c \
../User/BasicInfor/dormancy.c \
../User/BasicInfor/statusControl.c \
../User/BasicInfor/tool.c 

OBJS += \
./User/BasicInfor/debug.o \
./User/BasicInfor/delay.o \
./User/BasicInfor/dormancy.o \
./User/BasicInfor/statusControl.o \
./User/BasicInfor/tool.o 

C_DEPS += \
./User/BasicInfor/debug.d \
./User/BasicInfor/delay.d \
./User/BasicInfor/dormancy.d \
./User/BasicInfor/statusControl.d \
./User/BasicInfor/tool.d 


# Each subdirectory must supply rules for building sources it contributes
User/BasicInfor/%.o User/BasicInfor/%.su: ../User/BasicInfor/%.c User/BasicInfor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"G:/Code/Car_F407/Car_F407/User" -I../FATFS/Target -I../FATFS/App -I"G:/Code/Car_F407/Car_F407/Middlewares/Third_Party/FatFs/src" -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-BasicInfor

clean-User-2f-BasicInfor:
	-$(RM) ./User/BasicInfor/debug.d ./User/BasicInfor/debug.o ./User/BasicInfor/debug.su ./User/BasicInfor/delay.d ./User/BasicInfor/delay.o ./User/BasicInfor/delay.su ./User/BasicInfor/dormancy.d ./User/BasicInfor/dormancy.o ./User/BasicInfor/dormancy.su ./User/BasicInfor/statusControl.d ./User/BasicInfor/statusControl.o ./User/BasicInfor/statusControl.su ./User/BasicInfor/tool.d ./User/BasicInfor/tool.o ./User/BasicInfor/tool.su

.PHONY: clean-User-2f-BasicInfor

