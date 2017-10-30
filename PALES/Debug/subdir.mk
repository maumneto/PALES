################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ch.c \
../no.c \
../semantik.c \
../sink.c \
../symbols.c \
../util.c 

OBJS += \
./ch.o \
./no.o \
./semantik.o \
./sink.o \
./symbols.o \
./util.o 

C_DEPS += \
./ch.d \
./no.d \
./semantik.d \
./sink.d \
./symbols.d \
./util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/rodrigo/contiki/core -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


