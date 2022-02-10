################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Systems/QAS_LCD/QAS_LCD.cpp \
../QA_Systems/QAS_LCD/QAS_LCD_Fonts.cpp 

OBJS += \
./QA_Systems/QAS_LCD/QAS_LCD.o \
./QA_Systems/QAS_LCD/QAS_LCD_Fonts.o 

CPP_DEPS += \
./QA_Systems/QAS_LCD/QAS_LCD.d \
./QA_Systems/QAS_LCD/QAS_LCD_Fonts.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Systems/QAS_LCD/%.o: ../QA_Systems/QAS_LCD/%.cpp QA_Systems/QAS_LCD/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Componenets/Common -I../Drivers/Components/otm8009a -I../Core -I../QA_Drivers -I../QA_Drivers/QAD_PeripheralManagers -I../QA_Drivers/QAD_Devices -I../QA_Tools -I../QA_Systems -I../QA_Systems/QAS_Serial -I../QA_Systems/QAS_LCD -I../QA_Systems/QAS_LCD/QAS_LCD_Fonts -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-QA_Systems-2f-QAS_LCD

clean-QA_Systems-2f-QAS_LCD:
	-$(RM) ./QA_Systems/QAS_LCD/QAS_LCD.d ./QA_Systems/QAS_LCD/QAS_LCD.o ./QA_Systems/QAS_LCD/QAS_LCD_Fonts.d ./QA_Systems/QAS_LCD/QAS_LCD_Fonts.o

.PHONY: clean-QA_Systems-2f-QAS_LCD

