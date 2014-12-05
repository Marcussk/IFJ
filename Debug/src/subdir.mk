################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buff_file.c \
../src/builtins.c \
../src/error_handler.c \
../src/expr_parser.c \
../src/expr_token.c \
../src/hash_table.c \
../src/i_function.c \
../src/ial.c \
../src/ifj.c \
../src/ifj_types.c \
../src/instruction.c \
../src/interpret.c \
../src/lex_parser.c \
../src/parser_tables.c \
../src/semantic_analyzer.c \
../src/str_routines.c \
../src/syntax_analyzer.c \
../src/token_buff.c \
../src/token_map.c 

OBJS += \
./src/buff_file.o \
./src/builtins.o \
./src/error_handler.o \
./src/expr_parser.o \
./src/expr_token.o \
./src/hash_table.o \
./src/i_function.o \
./src/ial.o \
./src/ifj.o \
./src/ifj_types.o \
./src/instruction.o \
./src/interpret.o \
./src/lex_parser.o \
./src/parser_tables.o \
./src/semantic_analyzer.o \
./src/str_routines.o \
./src/syntax_analyzer.o \
./src/token_buff.o \
./src/token_map.o 

C_DEPS += \
./src/buff_file.d \
./src/builtins.d \
./src/error_handler.d \
./src/expr_parser.d \
./src/expr_token.d \
./src/hash_table.d \
./src/i_function.d \
./src/ial.d \
./src/ifj.d \
./src/ifj_types.d \
./src/instruction.d \
./src/interpret.d \
./src/lex_parser.d \
./src/parser_tables.d \
./src/semantic_analyzer.d \
./src/str_routines.d \
./src/syntax_analyzer.d \
./src/token_buff.d \
./src/token_map.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


