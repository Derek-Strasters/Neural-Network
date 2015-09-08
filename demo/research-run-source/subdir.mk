################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../research-run-source/Research-Run.cpp 

OBJS += \
./research-run-source/Research-Run.o 

CPP_DEPS += \
./research-run-source/Research-Run.d 


# Each subdirectory must supply rules for building sources it contributes
research-run-source/Research-Run.o: ../research-run-source/Research-Run.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/paracite/Research/NeuralNet" -O0 -g3 -pg -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"research-run-source/Research-Run.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


