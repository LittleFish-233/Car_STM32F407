################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/UART/bsp_uart.c 

OBJS += \
./User/UART/bsp_uart.o 

C_DEPS += \
./User/UART/bsp_uart.d 


# Each subdirectory must supply rules for building sources it contributes
User/UART/%.o User/UART/%.su: ../User/UART/%.c User/UART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"G:/Code/Car_F407/Car_F407/User" -I../FATFS/Target -I../FATFS/App -I"G:/Code/Car_F407/Car_F407/Middlewares/Third_Party/FatFs/src" -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-UART

clean-User-2f-UART:
	-$(RM) ./User/UART/bsp_uart.d ./User/UART/bsp_uart.o ./User/UART/bsp_uart.su

.PHONY: clean-User-2f-UART

