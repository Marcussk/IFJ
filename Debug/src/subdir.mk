################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/errorHandler.o \
../src/gerator3AC.o \
../src/lexParser.o 

C_SRCS += \
../src/buffFile.c \
../src/builtins.c \
../src/errorHandler.c \
../src/gerator3AC.c \
../src/hashTable.c \
../src/ifj.c \
../src/ifjTypes.c \
../src/instruction.c \
../src/interpret.c \
../src/lexParser.c \
../src/stack.c \
../src/strRoutines.c \
../src/syntaxAnalyzer.c \
../src/tokenMap.c 

OBJS += \
./src/buffFile.o \
./src/builtins.o \
./src/errorHandler.o \
./src/gerator3AC.o \
./src/hashTable.o \
./src/ifj.o \
./src/ifjTypes.o \
./src/instruction.o \
./src/interpret.o \
./src/lexParser.o \
./src/stack.o \
./src/strRoutines.o \
./src/syntaxAnalyzer.o \
./src/tokenMap.o 

C_DEPS += \
./src/buffFile.d \
./src/builtins.d \
./src/errorHandler.d \
./src/gerator3AC.d \
./src/hashTable.d \
./src/ifj.d \
./src/ifjTypes.d \
./src/instruction.d \
./src/interpret.d \
./src/lexParser.d \
./src/stack.d \
./src/strRoutines.d \
./src/syntaxAnalyzer.d \
./src/tokenMap.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


