################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client/Client.cpp \
../src/Client/ClientCLI.cpp \
../src/Client/DebugEventHandler.cpp \
../src/Client/Display.cpp \
../src/Client/Game.cpp \
../src/Client/TCPClient.cpp \
../src/Client/UDPClient.cpp \
../src/Client/UDPSocket.cpp 

OBJS += \
./src/Client/Client.o \
./src/Client/ClientCLI.o \
./src/Client/DebugEventHandler.o \
./src/Client/Display.o \
./src/Client/Game.o \
./src/Client/TCPClient.o \
./src/Client/UDPClient.o \
./src/Client/UDPSocket.o 

CPP_DEPS += \
./src/Client/Client.d \
./src/Client/ClientCLI.d \
./src/Client/DebugEventHandler.d \
./src/Client/Display.d \
./src/Client/Game.d \
./src/Client/TCPClient.d \
./src/Client/UDPClient.d \
./src/Client/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/Client/%.o: ../src/Client/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


