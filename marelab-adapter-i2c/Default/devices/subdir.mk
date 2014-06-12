################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../devices/I2C.cpp \
../devices/I2CDEVICE.cpp \
../devices/MAX520.cpp \
../devices/PCA9685.cpp \
../devices/PCF8574.cpp 

OBJS += \
./devices/I2C.o \
./devices/I2CDEVICE.o \
./devices/MAX520.o \
./devices/PCA9685.o \
./devices/PCF8574.o 

CPP_DEPS += \
./devices/I2C.d \
./devices/I2CDEVICE.d \
./devices/MAX520.d \
./devices/PCA9685.d \
./devices/PCF8574.d 


# Each subdirectory must supply rules for building sources it contributes
devices/%.o: ../devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/marelab/marelab-nucleus/src/globals" -O2 -g -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


