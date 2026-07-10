CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS  = $(MCU) -Wall -g -O2 -ffreestanding
LDFLAGS = $(MCU) -T linker.ld -nostdlib

SRCS   = main.c startup.s
OBJS   = main.o startup.o
TARGET = firmware

all: $(TARGET).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
