################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../demo-source/Demo.cpp 

OBJS += \
./demo-source/Demo.o 

CPP_DEPS += \
./demo-source/Demo.d 


# Each subdirectory must supply rules for building sources it contributes
demo-source/Demo.o: ../demo-source/Demo.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I"/home/thehuman/Code Repos/NeuralNet" -O2 -g -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -v -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"demo-source/Demo.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


