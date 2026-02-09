################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Locker/Drivers/drv_mcu.c 

OBJS += \
./Locker/Drivers/drv_mcu.o 

C_DEPS += \
./Locker/Drivers/drv_mcu.d 


# Each subdirectory must supply rules for building sources it contributes
Locker/Drivers/%.o Locker/Drivers/%.su Locker/Drivers/%.cyclo: ../Locker/Drivers/%.c Locker/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"D:/Documents/Github/stm32-layer-first/Locker/Applications" -I"D:/Documents/Github/stm32-layer-first/Locker/Drivers" -I"D:/Documents/Github/stm32-layer-first/Locker/Services" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Locker-2f-Drivers

clean-Locker-2f-Drivers:
	-$(RM) ./Locker/Drivers/drv_mcu.cyclo ./Locker/Drivers/drv_mcu.d ./Locker/Drivers/drv_mcu.o ./Locker/Drivers/drv_mcu.su

.PHONY: clean-Locker-2f-Drivers

