################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lib/subdir.mk
-include display/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
projFinalMCC.lss \

FLASH_IMAGE += \
projFinalMCC.hex \

EEPROM_IMAGE += \
projFinalMCC.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: projFinalMCC.elf secondary-outputs

# Tool invocations
projFinalMCC.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,projFinalMCC.map -mmcu=atmega328p -o "projFinalMCC.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

projFinalMCC.lss: projFinalMCC.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S projFinalMCC.elf  >"projFinalMCC.lss"
	@echo 'Finished building: $@'
	@echo ' '

projFinalMCC.hex: projFinalMCC.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex projFinalMCC.elf  "projFinalMCC.hex"
	@echo 'Finished building: $@'
	@echo ' '

projFinalMCC.eep: projFinalMCC.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex projFinalMCC.elf  "projFinalMCC.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: projFinalMCC.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p projFinalMCC.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) projFinalMCC.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
