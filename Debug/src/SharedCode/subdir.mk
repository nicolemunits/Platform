################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/oran/Desktop/SharedCodeClientServer/src/MTCPListener.cpp \
/home/oran/Desktop/SharedCodeClientServer/src/MThread.cpp \
/home/oran/Desktop/SharedCodeClientServer/src/TCPSocket.cpp \
/home/oran/Desktop/SharedCodeClientServer/src/UDPSocket.cpp 

OBJS += \
./src/SharedCode/MTCPListener.o \
./src/SharedCode/MThread.o \
./src/SharedCode/TCPSocket.o \
./src/SharedCode/UDPSocket.o 

CPP_DEPS += \
./src/SharedCode/MTCPListener.d \
./src/SharedCode/MThread.d \
./src/SharedCode/TCPSocket.d \
./src/SharedCode/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/SharedCode/MTCPListener.o: /home/oran/Desktop/SharedCodeClientServer/src/MTCPListener.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/SharedCode/MThread.o: /home/oran/Desktop/SharedCodeClientServer/src/MThread.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/SharedCode/TCPSocket.o: /home/oran/Desktop/SharedCodeClientServer/src/TCPSocket.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/SharedCode/UDPSocket.o: /home/oran/Desktop/SharedCodeClientServer/src/UDPSocket.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


