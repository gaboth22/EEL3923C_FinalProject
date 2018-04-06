################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Application.obj" \
"./main.obj" \
"./Controllers/DacController.obj" \
"./Controllers/LcdDisplayController.obj" \
"./Controllers/StepperCalibrator.obj" \
"./DataStructures/LinkedList.obj" \
"./DataStructures/LinkedListNode.obj" \
"./Event/EventSubscriber_Synchronous.obj" \
"./Event/Event_Synchronous.obj" \
"./HardwareAbstractions/Adc_10.obj" \
"./HardwareAbstractions/GpioGroup.obj" \
"./HardwareAbstractions/HeartbeatLed.obj" \
"./HardwareAbstractions/Interrupt_1MsSystemTick.obj" \
"./HardwareAbstractions/LcdDisplayParallel2Line.obj" \
"./HardwareAbstractions/Spi.obj" \
"./HardwareAbstractions/StepperMotor.obj" \
"./HardwareAbstractions/TimeSource_Interrupt1MsSystemTick.obj" \
"./HardwareAbstractions/WaveformGenerator.obj" \
"./Input/Input_Button.obj" \
"./Input/Input_Dial.obj" \
"./Input/Input_StepperMotorHomeSensor.obj" \
"./Input/Input_TwoButtonTimedPress.obj" \
"./Timer/Timer_OneShot.obj" \
"./Timer/Timer_Periodic.obj" \
"./TimerModule/TimerModule.obj" \
"../lnk_msp430f2274.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Controllers/subdir_vars.mk
-include DataStructures/subdir_vars.mk
-include Event/subdir_vars.mk
-include HardwareAbstractions/subdir_vars.mk
-include Input/subdir_vars.mk
-include Timer/subdir_vars.mk
-include TimerModule/subdir_vars.mk
-include subdir_rules.mk
-include Controllers/subdir_rules.mk
-include DataStructures/subdir_rules.mk
-include Event/subdir_rules.mk
-include HardwareAbstractions/subdir_rules.mk
-include Input/subdir_rules.mk
-include Timer/subdir_rules.mk
-include TimerModule/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
FinalProject.out \

EXE_OUTPUTS__QUOTED += \
"FinalProject.out" \

BIN_OUTPUTS += \
FinalProject.hex \

BIN_OUTPUTS__QUOTED += \
"FinalProject.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "FinalProject.out"

# Tool invocations
FinalProject.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -O0 --opt_for_speed=0 --use_hw_mpy=none --advice:power=all --define=__MSP430F2274__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU19 -z -m"FinalProject.map" --heap_size=80 --stack_size=80 -i"/Applications/ti/ccsv7/ccs_base/msp430/include" -i"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/lib" -i"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="FinalProject_linkInfo.xml" --use_hw_mpy=none --rom_model -o "FinalProject.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

FinalProject.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: MSP430 Hex Utility'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "FinalProject.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Application.obj" "main.obj" "Controllers/DacController.obj" "Controllers/LcdDisplayController.obj" "Controllers/StepperCalibrator.obj" "DataStructures/LinkedList.obj" "DataStructures/LinkedListNode.obj" "Event/EventSubscriber_Synchronous.obj" "Event/Event_Synchronous.obj" "HardwareAbstractions/Adc_10.obj" "HardwareAbstractions/GpioGroup.obj" "HardwareAbstractions/HeartbeatLed.obj" "HardwareAbstractions/Interrupt_1MsSystemTick.obj" "HardwareAbstractions/LcdDisplayParallel2Line.obj" "HardwareAbstractions/Spi.obj" "HardwareAbstractions/StepperMotor.obj" "HardwareAbstractions/TimeSource_Interrupt1MsSystemTick.obj" "HardwareAbstractions/WaveformGenerator.obj" "Input/Input_Button.obj" "Input/Input_Dial.obj" "Input/Input_StepperMotorHomeSensor.obj" "Input/Input_TwoButtonTimedPress.obj" "Timer/Timer_OneShot.obj" "Timer/Timer_Periodic.obj" "TimerModule/TimerModule.obj" 
	-$(RM) "Application.d" "main.d" "Controllers/DacController.d" "Controllers/LcdDisplayController.d" "Controllers/StepperCalibrator.d" "DataStructures/LinkedList.d" "DataStructures/LinkedListNode.d" "Event/EventSubscriber_Synchronous.d" "Event/Event_Synchronous.d" "HardwareAbstractions/Adc_10.d" "HardwareAbstractions/GpioGroup.d" "HardwareAbstractions/HeartbeatLed.d" "HardwareAbstractions/Interrupt_1MsSystemTick.d" "HardwareAbstractions/LcdDisplayParallel2Line.d" "HardwareAbstractions/Spi.d" "HardwareAbstractions/StepperMotor.d" "HardwareAbstractions/TimeSource_Interrupt1MsSystemTick.d" "HardwareAbstractions/WaveformGenerator.d" "Input/Input_Button.d" "Input/Input_Dial.d" "Input/Input_StepperMotorHomeSensor.d" "Input/Input_TwoButtonTimedPress.d" "Timer/Timer_OneShot.d" "Timer/Timer_Periodic.d" "TimerModule/TimerModule.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

