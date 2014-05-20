################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lcdtest.c \
../src/ses_lcdDriver.c \
../src/ses_led.c 

OBJS += \
./src/lcdtest.o \
./src/ses_lcdDriver.o \
./src/ses_led.o 

C_DEPS += \
./src/lcdtest.d \
./src/ses_lcdDriver.d \
./src/ses_led.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\dploeger\Documents\Uni\Software for Embedded Systems\ATMega\sheet4\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128rfa1 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


