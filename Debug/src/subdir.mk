################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buffFile.c \
../src/builtins.c \
../src/errorHandler.c \
../src/expr.c \
../src/gerator3AC.c \
../src/hashTable.c \
../src/iFunction.c \
../src/ial.c \
../src/ifj.c \
../src/ifjTypes.c \
../src/instruction.c \
../src/interpret.c \
../src/lexParser.c \
../src/parserTables.c \
../src/strRoutines.c \
../src/syntaxAnalyzer.c \
../src/testsinterpret.c \
../src/tokenBuff.c \
../src/tokenMap.c 

OBJS += \
./src/buffFile.o \
./src/builtins.o \
./src/errorHandler.o \
./src/expr.o \
./src/gerator3AC.o \
./src/hashTable.o \
./src/iFunction.o \
./src/ial.o \
./src/ifj.o \
./src/ifjTypes.o \
./src/instruction.o \
./src/interpret.o \
./src/lexParser.o \
./src/parserTables.o \
./src/strRoutines.o \
./src/syntaxAnalyzer.o \
./src/testsinterpret.o \
./src/tokenBuff.o \
./src/tokenMap.o 

C_DEPS += \
./src/buffFile.d \
./src/builtins.d \
./src/errorHandler.d \
./src/expr.d \
./src/gerator3AC.d \
./src/hashTable.d \
./src/iFunction.d \
./src/ial.d \
./src/ifj.d \
./src/ifjTypes.d \
./src/instruction.d \
./src/interpret.d \
./src/lexParser.d \
./src/parserTables.d \
./src/strRoutines.d \
./src/syntaxAnalyzer.d \
./src/testsinterpret.d \
./src/tokenBuff.d \
./src/tokenMap.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


