################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Locker/Services/protocol.c 

OBJS += \
./Locker/Services/protocol.o 

C_DEPS += \
./Locker/Services/protocol.d 


# Each subdirectory must supply rules for building sources it contributes
Locker/Services/%.o Locker/Services/%.su Locker/Services/%.cyclo: ../Locker/Services/%.c Locker/Services/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"D:/Documents/Github/stm32-layer-first/Locker/Applications" -I"D:/Documents/Github/stm32-layer-first/Locker/Drivers" -I"D:/Documents/Github/stm32-layer-first/Locker/Services" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Locker-2f-Services

clean-Locker-2f-Services:
	-$(RM) ./Locker/Services/protocol.cyclo ./Locker/Services/protocol.d ./Locker/Services/protocol.o ./Locker/Services/protocol.su

.PHONY: clean-Locker-2f-Services

