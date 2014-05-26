################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
task_5_3.lss \

FLASH_IMAGE += \
task_5_3.hex \

EEPROM_IMAGE += \
task_5_3.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: task_5_3.elf secondary-outputs

dependents:
	-cd /home/happy/workspace/ses/Release && $(MAKE) clean all

# Tool invocations
task_5_3.elf: $(OBJS) $(USER_OBJS) /home/happy/workspace/ses/Release/libses.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,task_5_3.map -L"/home/happy/workspace/ses/Release" -mmcu=atmega128rfa1 -o "task_5_3.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

task_5_3.lss: task_5_3.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S task_5_3.elf  >"task_5_3.lss"
	@echo 'Finished building: $@'
	@echo ' '

task_5_3.hex: task_5_3.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex task_5_3.elf  "task_5_3.hex"
	@echo 'Finished building: $@'
	@echo ' '

task_5_3.eep: task_5_3.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex task_5_3.elf  "task_5_3.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: task_5_3.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128rfa1 task_5_3.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) task_5_3.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
/home/happy/workspace/ses/Release/libses.a:

-include ../makefile.targets
