################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/Darek/workspace_v9-Synati_MQTT/SimpleLink_CC13XX_Async_HAL_Sensor_library" --include_path="/Users/Darek/ti/simplelink_cc13x0_sdk_4_10_01_01/source/ti/posix/ccs" --include_path="/Applications/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/include" --define=DeviceFamily_CC13X0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


