################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/UnitTest/UnitTest.cpp \
../src/UnitTest/UnitTestRunner.cpp 

OBJS += \
./src/UnitTest/UnitTest.o \
./src/UnitTest/UnitTestRunner.o 

CPP_DEPS += \
./src/UnitTest/UnitTest.d \
./src/UnitTest/UnitTestRunner.d 


# Each subdirectory must supply rules for building sources it contributes
src/UnitTest/%.o: ../src/UnitTest/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


