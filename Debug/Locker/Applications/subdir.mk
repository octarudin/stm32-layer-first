################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Locker/Applications/app_doorlock.c 

OBJS += \
./Locker/Applications/app_doorlock.o 

C_DEPS += \
./Locker/Applications/app_doorlock.d 


# Each subdirectory must supply rules for building sources it contributes
Locker/Applications/%.o Locker/Applications/%.su Locker/Applications/%.cyclo: ../Locker/Applications/%.c Locker/Applications/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"D:/Documents/Github/stm32-layer-first/Locker/Applications" -I"D:/Documents/Github/stm32-layer-first/Locker/Drivers" -I"D:/Documents/Github/stm32-layer-first/Locker/Services" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Locker-2f-Applications

clean-Locker-2f-Applications:
	-$(RM) ./Locker/Applications/app_doorlock.cyclo ./Locker/Applications/app_doorlock.d ./Locker/Applications/app_doorlock.o ./Locker/Applications/app_doorlock.su

.PHONY: clean-Locker-2f-Applications

