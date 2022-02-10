################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../QA_Drivers/QAD_EXTI.cpp \
../QA_Drivers/QAD_Encoder.cpp \
../QA_Drivers/QAD_FMC.cpp \
../QA_Drivers/QAD_GPIO.cpp \
../QA_Drivers/QAD_I2C.cpp \
../QA_Drivers/QAD_LTDC.cpp \
../QA_Drivers/QAD_PWM.cpp \
../QA_Drivers/QAD_QuadSPI.cpp \
../QA_Drivers/QAD_RNG.cpp \
../QA_Drivers/QAD_RTC.cpp \
../QA_Drivers/QAD_SDMMC.cpp \
../QA_Drivers/QAD_Timer.cpp \
../QA_Drivers/QAD_UART.cpp 

OBJS += \
./QA_Drivers/QAD_EXTI.o \
./QA_Drivers/QAD_Encoder.o \
./QA_Drivers/QAD_FMC.o \
./QA_Drivers/QAD_GPIO.o \
./QA_Drivers/QAD_I2C.o \
./QA_Drivers/QAD_LTDC.o \
./QA_Drivers/QAD_PWM.o \
./QA_Drivers/QAD_QuadSPI.o \
./QA_Drivers/QAD_RNG.o \
./QA_Drivers/QAD_RTC.o \
./QA_Drivers/QAD_SDMMC.o \
./QA_Drivers/QAD_Timer.o \
./QA_Drivers/QAD_UART.o 

CPP_DEPS += \
./QA_Drivers/QAD_EXTI.d \
./QA_Drivers/QAD_Encoder.d \
./QA_Drivers/QAD_FMC.d \
./QA_Drivers/QAD_GPIO.d \
./QA_Drivers/QAD_I2C.d \
./QA_Drivers/QAD_LTDC.d \
./QA_Drivers/QAD_PWM.d \
./QA_Drivers/QAD_QuadSPI.d \
./QA_Drivers/QAD_RNG.d \
./QA_Drivers/QAD_RTC.d \
./QA_Drivers/QAD_SDMMC.d \
./QA_Drivers/QAD_Timer.d \
./QA_Drivers/QAD_UART.d 


# Each subdirectory must supply rules for building sources it contributes
QA_Drivers/%.o: ../QA_Drivers/%.cpp QA_Drivers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Componenets/Common -I../Drivers/Components/otm8009a -I../Core -I../QA_Drivers -I../QA_Drivers/QAD_PeripheralManagers -I../QA_Drivers/QAD_Devices -I../QA_Tools -I../QA_Systems -I../QA_Systems/QAS_Serial -I../QA_Systems/QAS_LCD -I../QA_Systems/QAS_LCD/QAS_LCD_Fonts -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-QA_Drivers

clean-QA_Drivers:
	-$(RM) ./QA_Drivers/QAD_EXTI.d ./QA_Drivers/QAD_EXTI.o ./QA_Drivers/QAD_Encoder.d ./QA_Drivers/QAD_Encoder.o ./QA_Drivers/QAD_FMC.d ./QA_Drivers/QAD_FMC.o ./QA_Drivers/QAD_GPIO.d ./QA_Drivers/QAD_GPIO.o ./QA_Drivers/QAD_I2C.d ./QA_Drivers/QAD_I2C.o ./QA_Drivers/QAD_LTDC.d ./QA_Drivers/QAD_LTDC.o ./QA_Drivers/QAD_PWM.d ./QA_Drivers/QAD_PWM.o ./QA_Drivers/QAD_QuadSPI.d ./QA_Drivers/QAD_QuadSPI.o ./QA_Drivers/QAD_RNG.d ./QA_Drivers/QAD_RNG.o ./QA_Drivers/QAD_RTC.d ./QA_Drivers/QAD_RTC.o ./QA_Drivers/QAD_SDMMC.d ./QA_Drivers/QAD_SDMMC.o ./QA_Drivers/QAD_Timer.d ./QA_Drivers/QAD_Timer.o ./QA_Drivers/QAD_UART.d ./QA_Drivers/QAD_UART.o

.PHONY: clean-QA_Drivers

