################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.c \
../Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.c 

OBJS += \
./Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.o \
./Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.o 

C_DEPS += \
./Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.d \
./Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/vl53l8cx/%.o Drivers/BSP/Components/vl53l8cx/%.su Drivers/BSP/Components/vl53l8cx/%.cyclo: ../Drivers/BSP/Components/vl53l8cx/%.c Drivers/BSP/Components/vl53l8cx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/vl53l8cx -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-vl53l8cx

clean-Drivers-2f-BSP-2f-Components-2f-vl53l8cx:
	-$(RM) ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.cyclo ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.d ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.o ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_api.su ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.cyclo ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.d ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.o ./Drivers/BSP/Components/vl53l8cx/vl53l8cx_platform.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-vl53l8cx

