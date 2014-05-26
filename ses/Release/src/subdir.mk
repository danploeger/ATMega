################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/ses_lcd.o \
../src/ses_uart.o 

C_SRCS += \
../src/ses_lcdDriver.c \
../src/ses_led.c \
../src/ses_scheduler.c \
../src/ses_timer.c 

OBJS += \
./src/ses_lcdDriver.o \
./src/ses_led.o \
./src/ses_scheduler.o \
./src/ses_timer.o 

C_DEPS += \
./src/ses_lcdDriver.d \
./src/ses_led.d \
./src/ses_scheduler.d \
./src/ses_timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/happy/workspace/ses/include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=1600000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


