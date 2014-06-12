################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/json/json_reader.cpp \
../src/json/json_value.cpp \
../src/json/json_writer.cpp 

C_SRCS += \
../src/json/jsmn.c 

OBJS += \
./src/json/jsmn.o \
./src/json/json_reader.o \
./src/json/json_value.o \
./src/json/json_writer.o 

C_DEPS += \
./src/json/jsmn.d 

CPP_DEPS += \
./src/json/json_reader.d \
./src/json/json_value.d \
./src/json/json_writer.d 


# Each subdirectory must supply rules for building sources it contributes
src/json/%.o: ../src/json/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_REENTRANT -O2 -g -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/json/%.o: ../src/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_REENTRANT -O2 -g -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


