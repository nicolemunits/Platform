################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Shared/MTCPListener.cpp \
../src/Shared/MThread.cpp \
../src/Shared/TCPSocket.cpp 

OBJS += \
./src/Shared/MTCPListener.o \
./src/Shared/MThread.o \
./src/Shared/TCPSocket.o 

CPP_DEPS += \
./src/Shared/MTCPListener.d \
./src/Shared/MThread.d \
./src/Shared/TCPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/Shared/%.o: ../src/Shared/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


