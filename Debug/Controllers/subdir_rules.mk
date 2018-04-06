################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Controllers/DacController.obj: ../Controllers/DacController.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -O0 --opt_for_speed=0 --use_hw_mpy=none --include_path="/Applications/ti/ccsv7/ccs_base/msp430/include" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Input" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Models" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Controllers" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/DataStructures" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Event" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/HardwareAbstractions" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Timer" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/TimerModule" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Utilities" --include_path="/Users/gaboth22/Documents/workspace/FinalProject" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F2274__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="Controllers/DacController.d_raw" --obj_directory="Controllers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Controllers/LcdDisplayController.obj: ../Controllers/LcdDisplayController.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -O0 --opt_for_speed=0 --use_hw_mpy=none --include_path="/Applications/ti/ccsv7/ccs_base/msp430/include" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Input" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Models" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Controllers" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/DataStructures" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Event" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/HardwareAbstractions" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Timer" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/TimerModule" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Utilities" --include_path="/Users/gaboth22/Documents/workspace/FinalProject" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F2274__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="Controllers/LcdDisplayController.d_raw" --obj_directory="Controllers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Controllers/StepperCalibrator.obj: ../Controllers/StepperCalibrator.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -O0 --opt_for_speed=0 --use_hw_mpy=none --include_path="/Applications/ti/ccsv7/ccs_base/msp430/include" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Input" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Models" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Controllers" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/DataStructures" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Event" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/HardwareAbstractions" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Timer" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/TimerModule" --include_path="/Users/gaboth22/Documents/workspace/FinalProject/Utilities" --include_path="/Users/gaboth22/Documents/workspace/FinalProject" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F2274__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="Controllers/StepperCalibrator.d_raw" --obj_directory="Controllers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


