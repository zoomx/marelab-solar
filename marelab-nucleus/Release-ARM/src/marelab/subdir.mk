################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/marelab/IncomingMsg.cpp \
../src/marelab/LedString.cpp \
../src/marelab/LedTimerListe.cpp \
../src/marelab/MoonPhase.cpp \
../src/marelab/MsgOut.cpp \
../src/marelab/ipccom.cpp 

OBJS += \
./src/marelab/IncomingMsg.o \
./src/marelab/LedString.o \
./src/marelab/LedTimerListe.o \
./src/marelab/MoonPhase.o \
./src/marelab/MsgOut.o \
./src/marelab/ipccom.o 

CPP_DEPS += \
./src/marelab/IncomingMsg.d \
./src/marelab/LedString.d \
./src/marelab/LedTimerListe.d \
./src/marelab/MoonPhase.d \
./src/marelab/MsgOut.d \
./src/marelab/ipccom.d 


# Each subdirectory must supply rules for building sources it contributes
src/marelab/%.o: ../src/marelab/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/mnt/arm-tool/arm-2011.09/bin/arm-none-linux-gnueabi-g++ -D_REENTRANT -O3 -Wall -c -fmessage-length=0 -march=armv4t -mtune=arm920t -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


