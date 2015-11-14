################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../research-run-source/Research-Run.cpp \
../research-run-source/sha256.cpp 

OBJS += \
./research-run-source/Research-Run.o \
./research-run-source/sha256.o 

CPP_DEPS += \
./research-run-source/Research-Run.d \
./research-run-source/sha256.d 


# Each subdirectory must supply rules for building sources it contributes
research-run-source/Research-Run.o: ../research-run-source/Research-Run.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -I"/home/thehuman/Code Repos/NeuralNet" -O2 -g -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -v -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"research-run-source/Research-Run.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

research-run-source/%.o: ../research-run-source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++98 -O2 -g -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -v -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


