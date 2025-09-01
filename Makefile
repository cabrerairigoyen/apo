# EADK Makefile for Pi Stream External App

Q ?= @
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Compiler flags for EADK
CPPFLAGS = 
CXXFLAGS = -std=c++14 -fno-exceptions -fno-rtti
CFLAGS = -std=c99
LDFLAGS = -nostdlib -Wl,--entry=main

# Common flags for ARM Cortex-M7 (NumWorks N0110)
SFLAGS = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS += $(SFLAGS) -Os -Wall -Wextra -fno-builtin
CXXFLAGS += $(SFLAGS) -Os -Wall -Wextra -fno-builtin
LDFLAGS += $(SFLAGS)

# Target
TARGET = pi_stream
SOURCES = pi_stream_eadk.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET).nwa

# Build the .nwa file
$(TARGET).nwa: $(TARGET).elf
	$(Q) echo "Creating NWA: $@"
	$(Q) $(OBJCOPY) -O binary $< $@

# Build the ELF file
$(TARGET).elf: $(OBJECTS)
	$(Q) echo "Linking: $@"
	$(Q) $(LD) $(LDFLAGS) $^ -o $@

# Compile C files
%.o: %.c
	$(Q) echo "Compiling: $<"
	$(Q) $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Clean
clean:
	$(Q) rm -f $(OBJECTS) $(TARGET).elf $(TARGET).nwa

.PHONY: all clean
