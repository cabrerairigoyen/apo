# EADK Makefile for Pi Stream External App

Q ?= @
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

EADK_PATH ?= ./epsilon/eadk

# Compiler flags for EADK
CPPFLAGS = -I$(EADK_PATH)/include
CXXFLAGS = -std=c++14 -fno-exceptions -fno-rtti
CFLAGS = -std=c99
LDFLAGS = -Wl,--relocatable -nostartfiles

# Common flags
SFLAGS = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS += $(SFLAGS) -Os -Wall -Wextra
CXXFLAGS += $(SFLAGS) -Os -Wall -Wextra  
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

# EADK dependency
epsilon:
	$(Q) if [ ! -d "epsilon" ]; then \
		echo "Cloning epsilon for EADK..."; \
		git clone --depth 1 https://github.com/numworks/epsilon.git; \
	fi

.PHONY: all clean epsilon

# Ensure epsilon is available before building
$(OBJECTS): | epsilon
