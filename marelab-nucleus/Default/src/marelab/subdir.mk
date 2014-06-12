################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/marelab/ConfigNucleus.cpp \
../src/marelab/DataLogger.cpp \
../src/marelab/IncomingMsg.cpp \
../src/marelab/MoonPhase.cpp \
../src/marelab/MsgOut.cpp \
../src/marelab/PluginRegistry.cpp \
../src/marelab/ipccom.cpp 

OBJS += \
./src/marelab/ConfigNucleus.o \
./src/marelab/DataLogger.o \
./src/marelab/IncomingMsg.o \
./src/marelab/MoonPhase.o \
./src/marelab/MsgOut.o \
./src/marelab/PluginRegistry.o \
./src/marelab/ipccom.o 

CPP_DEPS += \
./src/marelab/ConfigNucleus.d \
./src/marelab/DataLogger.d \
./src/marelab/IncomingMsg.d \
./src/marelab/MoonPhase.d \
./src/marelab/MsgOut.d \
./src/marelab/PluginRegistry.d \
./src/marelab/ipccom.d 


# Each subdirectory must supply rules for building sources it contributes
src/marelab/%.o: ../src/marelab/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_REENTRANT -I/usr/include/lua5.2 -O2 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


