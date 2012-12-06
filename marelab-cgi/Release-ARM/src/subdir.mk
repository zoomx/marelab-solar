################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/marelab-cgi.cpp 

OBJS += \
./src/marelab-cgi.o 

CPP_DEPS += \
./src/marelab-cgi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/media/ARM/ARM/arm-2011.09/bin/arm-none-linux-gnueabi-g++ -O3 -Wall -c -fmessage-length=0 -march=armv4t -mtune=arm920t -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


