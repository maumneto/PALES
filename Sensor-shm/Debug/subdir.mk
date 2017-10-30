################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1.c \
../2.c \
../CH10.c \
../Pacotes.c \
../Sink.c \
../symbols.c 

OBJS += \
./1.o \
./2.o \
./CH10.o \
./Pacotes.o \
./Sink.o \
./symbols.o 

C_DEPS += \
./1.d \
./2.d \
./CH10.d \
./Pacotes.d \
./Sink.d \
./symbols.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/rodrigo/contiki/core -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


