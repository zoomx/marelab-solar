################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/globals/json/json_reader.cpp \
../src/globals/json/json_value.cpp \
../src/globals/json/json_writer.cpp 

OBJS += \
./src/globals/json/json_reader.o \
./src/globals/json/json_value.o \
./src/globals/json/json_writer.o 

CPP_DEPS += \
./src/globals/json/json_reader.d \
./src/globals/json/json_value.d \
./src/globals/json/json_writer.d 


# Each subdirectory must supply rules for building sources it contributes
src/globals/json/%.o: ../src/globals/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_REENTRANT -O2 -g -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


