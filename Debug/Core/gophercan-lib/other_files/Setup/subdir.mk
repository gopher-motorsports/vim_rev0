################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Isaiah/STM32CubeIDE/workspace_1.10.1/gophercan-lib/other_files/Setup/F0_example_main.c \
C:/Users/Isaiah/STM32CubeIDE/workspace_1.10.1/gophercan-lib/other_files/Setup/F7_example_main.c 

OBJS += \
./Core/gophercan-lib/other_files/Setup/F0_example_main.o \
./Core/gophercan-lib/other_files/Setup/F7_example_main.o 

C_DEPS += \
./Core/gophercan-lib/other_files/Setup/F0_example_main.d \
./Core/gophercan-lib/other_files/Setup/F7_example_main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/gophercan-lib/other_files/Setup/F0_example_main.o: C:/Users/Isaiah/STM32CubeIDE/workspace_1.10.1/gophercan-lib/other_files/Setup/F0_example_main.c Core/gophercan-lib/other_files/Setup/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Isaiah/STM32CubeIDE/workspace_1.10.1/vim_fsae_rev0/Core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/gophercan-lib/other_files/Setup/F7_example_main.o: C:/Users/Isaiah/STM32CubeIDE/workspace_1.10.1/gophercan-lib/other_files/Setup/F7_example_main.c Core/gophercan-lib/other_files/Setup/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Isaiah/STM32CubeIDE/workspace_1.10.1/vim_fsae_rev0/Core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-gophercan-2d-lib-2f-other_files-2f-Setup

clean-Core-2f-gophercan-2d-lib-2f-other_files-2f-Setup:
	-$(RM) ./Core/gophercan-lib/other_files/Setup/F0_example_main.d ./Core/gophercan-lib/other_files/Setup/F0_example_main.o ./Core/gophercan-lib/other_files/Setup/F0_example_main.su ./Core/gophercan-lib/other_files/Setup/F7_example_main.d ./Core/gophercan-lib/other_files/Setup/F7_example_main.o ./Core/gophercan-lib/other_files/Setup/F7_example_main.su

.PHONY: clean-Core-2f-gophercan-2d-lib-2f-other_files-2f-Setup

