################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Screen/bmp.c \
../User/Screen/font.c \
../User/Screen/image.c \
../User/Screen/lcd.c \
../User/Screen/oled.c \
../User/Screen/screen.c 

OBJS += \
./User/Screen/bmp.o \
./User/Screen/font.o \
./User/Screen/image.o \
./User/Screen/lcd.o \
./User/Screen/oled.o \
./User/Screen/screen.o 

C_DEPS += \
./User/Screen/bmp.d \
./User/Screen/font.d \
./User/Screen/image.d \
./User/Screen/lcd.d \
./User/Screen/oled.d \
./User/Screen/screen.d 


# Each subdirectory must supply rules for building sources it contributes
User/Screen/%.o User/Screen/%.su: ../User/Screen/%.c User/Screen/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"G:/Code/Car_F407/Car_F407/User" -I../FATFS/Target -I../FATFS/App -I"G:/Code/Car_F407/Car_F407/Middlewares/Third_Party/FatFs/src" -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Screen

clean-User-2f-Screen:
	-$(RM) ./User/Screen/bmp.d ./User/Screen/bmp.o ./User/Screen/bmp.su ./User/Screen/font.d ./User/Screen/font.o ./User/Screen/font.su ./User/Screen/image.d ./User/Screen/image.o ./User/Screen/image.su ./User/Screen/lcd.d ./User/Screen/lcd.o ./User/Screen/lcd.su ./User/Screen/oled.d ./User/Screen/oled.o ./User/Screen/oled.su ./User/Screen/screen.d ./User/Screen/screen.o ./User/Screen/screen.su

.PHONY: clean-User-2f-Screen

