# Minimal Makefile for testing EADK compilation
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS += -Os -std=c99 -nostdlib -nostartfiles
LDFLAGS = -Wl,--entry=main -Wl,--relocatable

test.nwa: minimal_test.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f test.nwa

.PHONY: clean
