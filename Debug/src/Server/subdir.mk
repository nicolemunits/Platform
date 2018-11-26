################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Server/Dispatcher.cpp \
../src/Server/LoginAndRegister.cpp \
../src/Server/Md5.cpp \
../src/Server/PeerName.cpp \
../src/Server/Server.cpp \
../src/Server/ServerCLI.cpp 

OBJS += \
./src/Server/Dispatcher.o \
./src/Server/LoginAndRegister.o \
./src/Server/Md5.o \
./src/Server/PeerName.o \
./src/Server/Server.o \
./src/Server/ServerCLI.o 

CPP_DEPS += \
./src/Server/Dispatcher.d \
./src/Server/LoginAndRegister.d \
./src/Server/Md5.d \
./src/Server/PeerName.d \
./src/Server/Server.d \
./src/Server/ServerCLI.d 


# Each subdirectory must supply rules for building sources it contributes
src/Server/%.o: ../src/Server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


